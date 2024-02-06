#include "pch.h"

#include "CAnimator3D.h"
#include "CAnimation3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	:m_pBoneFinalMatBuffer(nullptr)
	, m_iCurIdx(0)
	,CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pBoneFinalMatBuffer(nullptr)
	, m_iCurIdx(0)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;

	for (int i = 0; i < m_AniList.size(); ++i)
	{
		if (m_AniList[i] != nullptr)
			delete m_AniList[i];
	}
}


void CAnimator3D::finaltick()
{
	m_AniList[m_iCurIdx]->finaltick();
}

void CAnimator3D::SetAnimClip(const vector<wstring>& _vecAnimClipList)
{
	int iClipCount = _vecAnimClipList.size();
	for (int i = 0; i < iClipCount; ++i)
	{
		wstring strName = _vecAnimClipList[i];
		Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(strName);
		CAnimation3D* pAni = new CAnimation3D(pClip);
		m_AniList.push_back(pAni);
	}
}
UINT CAnimator3D::GetBoneCount()
{
	return m_AniList[m_iCurIdx]->m_pClip->GetBoneCount();
}

void CAnimator3D::UpdateData()
{
	if (!m_AniList[m_iCurIdx]->m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		check_bone();

		pUpdateShader->SetFrameDataBuffer(m_AniList[m_iCurIdx]->m_pClip->GetBoneFrameBuffer());
		pUpdateShader->SetOffsetMatBuffer(m_AniList[m_iCurIdx]->m_pClip->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = m_AniList[m_iCurIdx]->m_pClip->GetBoneCount();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_AniList[m_iCurIdx]->m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_AniList[m_iCurIdx]->m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_AniList[m_iCurIdx]->m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_AniList[m_iCurIdx]->m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::check_bone()
{
	UINT iBoneCount = m_AniList[m_iCurIdx]->m_pClip->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}