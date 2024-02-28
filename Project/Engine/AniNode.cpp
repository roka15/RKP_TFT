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
	if (len != 0)
	{
		Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(_strClipName);
		pAni = new CAnimation3D(pClip);
	}

	m_pMotionClip = pAni;
}

CAniNode::~CAniNode()
{
	RemoveAllInTransition();
	RemoveAllOutTransition();
}

void CAniNode::check_bone(CStructuredBuffer*& _finalMat)
{
	if (m_pMotionClip == nullptr)
		return;
	UINT iBoneCount = m_pMotionClip->m_pClip->GetBoneCount();
	if (_finalMat->GetElementCount() != iBoneCount)
	{
		_finalMat->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

int CAniNode::Save(FILE* _pFile)
{
	int loopSize = m_vecOutConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		m_vecOutConditions[i]->Save(_pFile);
	}
	wstring strClipKey = {};
	if (m_pMotionClip == nullptr)
		strClipKey = L"";
	else
		strClipKey = m_pMotionClip->GetClip()->GetName();
	int strLen = strClipKey.size() * UNICODELEN;
	fwrite(&strLen, sizeof(int), 1, _pFile);
	fwrite(strClipKey.c_str(), strLen, 1, _pFile);
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
	if (strLen != 0)
	{
		strBuff[strLen + 1] = '\0';
		Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(strBuff);
		m_pMotionClip = new CAnimation3D(pClip);
	}
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
			_pTransition->SetConnectNode(nullptr);
			delete pOutTransition;
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
		delete m_vecOutConditions[i];
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
bool CAniNode::NextNode(int _iOutSize, bool _bFinish, bool _bCurNullNode)
{
	wstring CurName = GetName();
	if (CurName.compare(L"Exit") == 0)
	{
		CAniNode* Entry = m_pController->GetNode(L"Entry");
		m_pController->SetCurNode(Entry);
		return true;
	}

	//다음 transition이 없다면 loop를 수행할지 말지 정한다.
	if (_bFinish == true && _iOutSize == 0)
	{
		bool bLoop = m_pMotionClip->IsLoop();
		if (bLoop)
			m_pMotionClip->Reset();

		return false;
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
		}

		if (_bFinish)
		{
			if (bExitTime)
			{
				if (m_pMotionClip)
					m_pMotionClip->Reset();
				pOutTransition->RegisterCurNode(m_pController);
				return true;
			}
			else
			{
				//loop 의 값과 무관하게 같은 애니를 반복한다.
				if (m_pMotionClip)
					m_pMotionClip->Reset();
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
				if (CurName.compare(L"AnyState") == 0)
					int a = 0;
				//재생을 완료하지 않았어도 Condition을 만족하면 다음 노드로 이동한다.
				bool bActive = pOutTransition->IsActive(m_pController);
				//condition active 되면 넘어감.
				if (bActive)
				{
					if (CurName.compare(L"AnyState"))
						int a = 0;
					if (m_pMotionClip)
						m_pMotionClip->Reset();
					pOutTransition->RegisterCurNode(m_pController);
					return true;
				}
			}
		}
	}
	return false;
}
UINT CAniNode::GetBoneCount()
{
	if (m_pMotionClip == nullptr)
		return -1;
	return m_pMotionClip->m_pClip->GetBoneCount();
}
void CAniNode::finaltick()
{
	bool bNullClip = m_pMotionClip == nullptr; // Entry,Exit,AniState 같은 노드는 Clip이 없다.
	bool bFinish = false;
	if (bNullClip == false)
		bFinish = m_pMotionClip->IsFinish();

	size_t iOutSize = m_vecOutConditions.size();
	bool bNextNode = NextNode(iOutSize, bFinish, bNullClip);
	if (bNextNode)
		return;


	if (m_pMotionClip != nullptr)
		m_pMotionClip->finaltick();
}

void CAniNode::UpdateData(CStructuredBuffer*& _finalMat)
{
	if (m_pMotionClip == nullptr)
		return;
	if (m_pMotionClip->m_bFinalMatUpdate == false)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();
		check_bone(_finalMat);
		pUpdateShader->SetFrameDataBuffer(m_pMotionClip->m_pClip->GetBoneFrameBuffer());
		pUpdateShader->SetOffsetMatBuffer(m_pMotionClip->m_pClip->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(_finalMat);

		UINT iBoneCount = m_pMotionClip->m_pClip->GetBoneCount();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_pMotionClip->m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_pMotionClip->m_iNextFrameIdx);
		pUpdateShader->SetBlendingFlag(m_bBlending);
		pUpdateShader->SetFrameRatio(m_pMotionClip->m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_pMotionClip->m_bFinalMatUpdate = true;
	}
}
