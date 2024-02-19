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
	, m_bBlending(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pBoneFinalMatBuffer(nullptr)
	, m_iCurIdx(0)
	, m_bBlending(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}


void CAnimator3D::finaltick()
{
	if (m_pController == nullptr)
		return;

	m_pController->finaltick();
}

void CAnimator3D::UpdateData()
{
	if (m_pController == nullptr)
		return;

	m_pController->UpdateData(m_pBoneFinalMatBuffer);

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	return;
}

void CAnimator3D::SetController(wstring _strName)
{
	m_pController = CResMgr::GetInst()->FindRes<CAnimatorController>(_strName);
}
const wstring& CAnimator3D::GetCurControllerName()
{
	return m_pController->GetName();
}
UINT CAnimator3D::GetBoneCount()
{
	return m_pController->GetBoneCount();
}
void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	if (GetOwner()->MeshRender() == nullptr)
		return;
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

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}