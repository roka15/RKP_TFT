#include "pch.h"
#include "CImage.h"
#include "CResMgr.h"
#include "CMeshRender.h"

void CImage::UpdateData()
{
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(m_TextureKey));
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_vec4Color);
}

CImage::CImage():CUI(COMPONENT_TYPE::IMAGE)
{
}

CImage::CImage(const CImage& _ref) : CUI(_ref)
{
}

CImage::~CImage()
{
}
