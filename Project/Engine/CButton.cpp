#include "pch.h"
#include "CButton.h"
#include "CMouseMgr.h"
void CButton::finaltick()
{

}
void CButton::DownEvent(PointerEventData _data)
{
	int a = 0;
}
CButton::CButton():CUI(COMPONENT_TYPE::BUTTON)
{
}

CButton::CButton(const CButton& _ref) : CUI(_ref)
{
}

CButton::~CButton()
{
}
