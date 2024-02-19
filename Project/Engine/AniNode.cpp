#include "pch.h"
#include "AniNode.h"
#include "CAnimation3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CAniClip.h"
#include "Transition.h"
#include "CStructuredBuffer.h"

//OutConditions : node ���� �����ϱ�
//InConditions  : node ���� ���� ����.
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
		m_vecInConditions[i] = nullptr;
	}
	m_vecInConditions.clear();
}
void CAniNode::RemoveAllOutTransition()
{
	for (int i = 0; i < m_vecOutConditions.size(); ++i)
	{
		delete m_vecOutConditions[i];
		m_vecInConditions[i] = nullptr;
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
	//���� transition�� ���ٸ� loop�� �������� ���� ���Ѵ�.
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

		//���� ��尡 Entry,AnyState,Exit ���� ����� ���.
		if (_bCurNullNode)
		{
			pOutTransition->RegisterCurNode(m_pController);
			return true;
		}
		//Condition �˻縦 ���� �ʰ� �ִ� ����� �Ϸ� �ߴ��� ���η� ���� ����� �����Ѵ�.
		bool bExitTime = pOutTransition->IsExitTime();
		if (_bFinish)
		{
			if (bExitTime)
			{
				m_pMotionClip->Reset();
				pOutTransition->RegisterCurNode(m_pController);
				return true;
			}
			else
			{
				//loop �� ���� �����ϰ� ���� �ִϸ� �ݺ��Ѵ�.
				m_pMotionClip->Reset();
			}
		}
		else
		{
			if (bExitTime)
			{
				//condition active �Ǿ finish�� �ƴϱ� ������ �Ѿ�� ����.
			}
			else
			{   
				//����� �Ϸ����� �ʾҾ Condition�� �����ϸ� ���� ���� �̵��Ѵ�.
				bool bActive = pOutTransition->IsActive(m_pController);
				//condition active �Ǹ� �Ѿ.
				if (bActive)
				{
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
	bool bNullClip = m_pMotionClip == nullptr; // Entry,Exit,AniState ���� ���� Clip�� ����.
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

		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_pMotionClip->m_bFinalMatUpdate = true;
	}
}
