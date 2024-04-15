#include "pch.h"
#include "CImage.h"
#include "CResMgr.h"
#include "CGameObject.h"
#include "CRenderComponent.h"

void CImage::UpdateData()
{
	CRenderComponent* RenderCom = GetOwner()->GetRenderComponent();
	if (m_bActiveImage == false)
	{
		RenderCom->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(m_NormalTexKey));
	}
	else
	{
		RenderCom->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(m_EnableTexKey));
	}
	RenderCom->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_vec4Color);
}

CImage::CImage():CUI(COMPONENT_TYPE::IMAGE),
m_bActiveImage(false)
{
}

CImage::CImage(const CImage& _ref) : CUI(_ref),
m_bActiveImage(false),
m_NormalTexKey(_ref.m_NormalTexKey),
m_EnableTexKey(_ref.m_EnableTexKey)
{
}

CImage::~CImage()
{
}
