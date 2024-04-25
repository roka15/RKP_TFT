#include "pch.h"
#include "AniNode.h"
#include "CAnimation3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CAniClip.h"
#include "Transition.h"
#include "CStructuredBuffer.h"

CAniNode::CAniNode() :
	m_bBlending(false)
{
}
//OutConditions : node 에서 삭제하기
//InConditions  : node 에서 참조 끊기.
CAniNode::CAniNode(wstring _strClipName) :
	m_bBlending(false)
{
	CAnimation3D* pAni = nullptr;
	size_t len = _strClipName.size();
	m_strAniKey = _strClipName;
}

CAniNode::~CAniNode()
{
	RemoveAllInTransition();
	RemoveAllOutTransition();
}


int CAniNode::Save(FILE* _pFile)
{
	int loopSize = m_vecOutConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		m_vecOutConditions[i]->Save(_pFile);
	}
	int strLen = m_strAniKey.size() * UNICODELEN;
	fwrite(&strLen, sizeof(int), 1, _pFile);
	fwrite(m_strAniKey.c_str(), strLen, 1, _pFile);
	fwrite(&m_bBlending, sizeof(bool), 1, _pFile);
	return S_OK;
}

int CAniNode::Load(FILE* _pFile)
{
	int loopSize = 0;
	fread(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		CTransition* pTransition = new CTransition();
		pTransition->Load(_pFile);
		m_vecOutConditions.push_back(pTransition);
	}
	int strLen = 0;
	wchar_t strBuff[MAXLEN] = {};
	fread(&strLen, sizeof(int), 1, _pFile);
	fread(strBuff, strLen, 1, _pFile);
	m_strAniKey = strBuff;
	fread(&m_bBlending, sizeof(bool), 1, _pFile);
	return 0;
}

int CAniNode::LoadAfterProcess()
{
	for (int i = 0; i < m_vecOutConditions.size(); ++i)
	{
		wstring strKey = m_vecOutConditions[i]->GetConnectNodeName();
		CAniNode* pNode = m_pController->GetNode(strKey);
		CTransition* pTransition = m_vecOutConditions[i];

		m_vecOutConditions[i]->SetConnectNode(pNode);
		m_vecOutConditions[i]->SetOwner(this);
		pNode->m_vecInConditions.push_back(pTransition);
	}
	return S_OK;
}

void CAniNode::AddOutTransition(CTransition* _pTransition)
{
	m_vecOutConditions.push_back(_pTransition);
}
void CAniNode::AddInTransition(CTransition* _pTransition)
{
	m_vecInConditions.push_back(_pTransition);
}

void CAniNode::RemoveOutTransition(CTransition* _pTransition)
{
	for (auto itr = m_vecOutConditions.begin(); itr < m_vecOutConditions.end(); ++itr)
	{
		CTransition* pOutTransition = *itr;
		if (pOutTransition == _pTransition)
		{
			m_vecOutConditions.erase(itr);
			pOutTransition = nullptr;
			return;
		}
	}
}
void CAniNode::RemoveInTransition(CTransition* _pTransition)
{
	for (auto itr = m_vecInConditions.begin(); itr < m_vecInConditions.end(); ++itr)
	{
		CTransition* pInTransition = *itr;
		if (pInTransition == _pTransition)
		{
			m_vecInConditions.erase(itr);
			pInTransition = nullptr;
			return;
		}
	}
}
void CAniNode::RemoveAllInTransition()
{
	for (int i = 0; i < m_vecInConditions.size(); ++i)
	{
		if (m_vecInConditions[i] == nullptr)
			continue;
		m_vecInConditions[i] = nullptr;
	}
	m_vecInConditions.clear();
}
void CAniNode::RemoveAllOutTransition()
{
	for (int i = 0; i < m_vecOutConditions.size(); ++i)
	{
		if (m_vecOutConditions[i] == nullptr)
			continue;
		m_vecOutConditions[i] = nullptr;
	}
	m_vecOutConditions.clear();
}
void CAniNode::Destory()
{
	for (int i = 0; i < m_vecInConditions.size(); ++i)
	{
		CTransition* pDelTransition = m_vecInConditions[i];
		CAniNode* pOwnerNode = pDelTransition->GetOwner();
		pOwnerNode->RemoveOutTransition(pDelTransition);
		m_vecInConditions[i] = nullptr;
	}

	RemoveAllOutTransition();
}
ANI_NODE_RETURN CAniNode::NextNode(bool _bFinish, bool _bLoop, CAnimator3D* _pAnimator)
{
	wstring strName = GetName();
	bool NoClipNode = false;
	if (strName.compare(L"Entry") == 0 || strName.compare(L"Exit") == 0 || strName.compare(L"AnyState") == 0)
		NoClipNode = true;

	size_t iOutSize = m_vecOutConditions.size();
	return NextNode(iOutSize, _bFinish, NoClipNode, _bLoop, _pAnimator);
}
ANI_NODE_RETURN CAniNode::NextNode(int _iOutSize, bool _bFinish, bool _bCurNullNode, bool _bLoop, CAnimator3D* _pAnimator)
{
	wstring CurName = GetName();
	if (CurName.compare(L"Exit") == 0)
	{
		return ANI_NODE_RETURN::ENTRY;
	}

	//다음 transition이 없다면 loop를 수행할지 말지 정한다.
	if (_bFinish == true && _iOutSize == 0)
	{
		if (_bLoop)
		{
			return  ANI_NODE_RETURN::RESET;
		}
		return  ANI_NODE_RETURN::NOTHING;
	}

	for (int i = 0; i < _iOutSize; ++i)
	{
		CTransition* pOutTransition = m_vecOutConditions[i];

		//현재 노드가 Entry,AnyState,Exit 같은 노드인 경우.
		/*if (_bCurNullNode)
		{
			pOutTransition->RegisterCurNode(m_pController);
			return true;
		}*/
		//Condition 검사를 하지 않고 애니 재생을 완료 했는지 여부로 다음 재생을 결정한다.
		bool bExitTime = pOutTransition->IsExitTime();
		if (_bCurNullNode)
		{
			if (bExitTime)
			{
				_bFinish = true;
			}
			else
			{
				_bFinish = false;
			}
		}

		if (_bFinish)
		{
			if (bExitTime)
			{
				bool bChange = pOutTransition->RegisterCurNode(_pAnimator);
				if (bChange)
					return ANI_NODE_RETURN::CHANGE;
				else
					return ANI_NODE_RETURN::NOTHING;
			}
			else
			{
				//loop 의 값과 무관하게 같은 애니를 반복한다.
				return ANI_NODE_RETURN::RESET;
			}
		}
		else
		{
			if (bExitTime)
			{
				//condition active 되어도 finish가 아니기 때문에 넘어가지 않음.
			}
			else
			{
				//재생을 완료하지 않았어도 Condition을 만족하면 다음 노드로 이동한다.
				bool bActive = pOutTransition->IsActive(m_pController,_pAnimator);
				
				
				//condition active 되면 넘어감.
				if (bActive)
				{
					if (pOutTransition->GetConnectNodeName().compare(L"Exit") == 0)
						return ANI_NODE_RETURN::EXIT;

					bool bChange = pOutTransition->RegisterCurNode(_pAnimator);
					if (bChange)
						return ANI_NODE_RETURN::CHANGE;
					else
						return ANI_NODE_RETURN::NOTHING;
				}
			}
		}
	}
	return ANI_NODE_RETURN::NOTHING;
}

