#include "pch.h"
#include "CText.h"
#include "CFontMgr.h"
#include "CTransform.h"
#include "CRenderMgr.h"
#include "CCamera.h"
void CText::finaltick()
{
}

void CText::UpdateData()
{
	Vec3 pos = Transform()->GetWorldPos();
	Viewport view;
	CCamera* pUICam = CRenderMgr::GetInst()->GetCam(1);
	view.width = GlobalData.Resolution.x;
	view.height = GlobalData.Resolution.y;
	view.x = 0;
	view.y = 0;
	view.minDepth = 0.0f;
	view.maxDepth = 1.0f;
	pos = view.Project(pos, pUICam->GetProjMat(), pUICam->GetViewMat(), IDENTITY_MAT);
	CFontMgr::GetInst()->DrawFont(m_Text.c_str(), pos.x, pos.y, m_fSize, FONT_RGBA(m_v4Color.x, m_v4Color.y, m_v4Color.z, m_v4Color.w));
}

UINT CText::GetColor()
{
	return FONT_RGBA(m_v4Color.x, m_v4Color.y, m_v4Color.z, m_v4Color.w); 
}

CText::CText():CUI(COMPONENT_TYPE::TEXT)
{
}

CText::CText(const CText& _ref) : CUI(_ref),
m_fSize(_ref.m_fSize),
m_v4Color(_ref.m_v4Color),
m_Text(_ref.m_Text)
{
}

CText::~CText()
{
}
