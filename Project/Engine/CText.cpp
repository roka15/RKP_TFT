#include "pch.h"
#include "CText.h"
#include "CFontMgr.h"
void CText::finaltick()
{
}

void CText::UpdateData()
{
	CFontMgr::GetInst()->DrawFont(m_Text.c_str(), m_v2Pos.x, m_v2Pos.y, m_fSize, FONT_RGBA(m_v4Color.x, m_v4Color.y, m_v4Color.z, m_v4Color.w));
}

UINT CText::GetColor()
{
	return FONT_RGBA(m_v4Color.x, m_v4Color.y, m_v4Color.z, m_v4Color.w); 
}

CText::CText():CUI(COMPONENT_TYPE::TEXT)
{
}

CText::CText(const CText& _ref) : CUI(_ref)
{
}

CText::~CText()
{
}
