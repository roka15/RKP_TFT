#include "pch.h"
#include "CButton.h"
#include "CMouseMgr.h"
void CButton::finaltick()
{

}
void CButton::DownEvent(PointerEventData _data)
{
	m_FuncPtr();
}
void CButton::RegisterLinkFuncPtr(std::function<void()> _FuncPtr)
{
	m_FuncPtr = _FuncPtr;
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
