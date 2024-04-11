#include "pch.h"
#include "CImage.h"
#include "CResMgr.h"
#include "CMeshRender.h"

void CImage::UpdateData()
{
	if (m_bActiveImage == false)
	{
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(m_NormalTexKey));
	}
	else
	{
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(m_EnableTexKey));
	}
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_vec4Color);
}

CImage::CImage():CUI(COMPONENT_TYPE::IMAGE),
m_bActiveImage(false)
{
}

CImage::CImage(const CImage& _ref) : CUI(_ref),
m_bActiveImage(false)
{
}

CImage::~CImage()
{
}
