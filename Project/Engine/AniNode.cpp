#include "pch.h"
#include "AniNode.h"
#include "CAnimation3D.h"
#include "CAnimation3DShader.h"
#include "CResMgr.h"
void CAniNode::finaltick()
{
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

		pUpdateShader->SetFrameDataBuffer(m_pMotionClip->m_pClip->GetBoneFrameBuffer());
		pUpdateShader->SetOffsetMatBuffer(m_pMotionClip->m_pClip->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(_finalMat);

		UINT iBoneCount = m_pMotionClip->m_pClip->GetBoneCount();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_pMotionClip->m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_pMotionClip->m_iNextFrameIdx);
		pUpdateShader->SetBlendingFlag(_finalMat);
		pUpdateShader->SetFrameRatio(m_pMotionClip->m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_pMotionClip->m_bFinalMatUpdate = true;
	}
}

CAniNode::CAniNode()
{
}

CAniNode::~CAniNode()
{
}
