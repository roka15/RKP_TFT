#include "pch.h"
#include "AniNode.h"
#include "CAnimation3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
#include "CAniClip.h"
#include "Transition.h"
#include "CStructuredBuffer.h"

//OutConditions : node 에서 삭제하기
//InConditions  : node 에서 참조 끊기.
CAniNode::CAniNode(wstring _strClipName):
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
bool CAniNode::NextNode(int _iOutSize)
{
	for (int i = 0; i < _iOutSize; ++i)
	{
		CTransition* pOutTransition = m_vecOutConditions[i];
		bool bActive = pOutTransition->IsActive();
		if (bActive)
		{
			pOutTransition->RegisterCurNode(m_pController);
			return true;
		}
	}
	return false;
}
UINT CAniNode::GetBoneCount()
{
	return m_pMotionClip->m_pClip->GetBoneCount();
}
void CAniNode::finaltick()
{
	bool bNullClip = m_pMotionClip == nullptr; // Entry,Exit,AniState 같은 노드는 Clip이 없다.
	bool bFinish = false;
	if (bNullClip == false)
		bFinish = m_pMotionClip->IsFinish();

	if (bFinish || bNullClip)
	{
		size_t iOutSize = m_vecOutConditions.size();
		if (iOutSize == 0)
		{
			bool bLoop = m_pMotionClip->IsLoop();
			if (bLoop)
				m_pMotionClip->Reset();
		}
		else
		{
			bool bNextNode = NextNode(iOutSize);
			if (bNextNode)
				return;
		}
	}
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
