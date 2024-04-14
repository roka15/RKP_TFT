#include "pch.h"
#include "CButton.h"
#include "CMouseMgr.h"
#include "CImage.h"
void CButton::finaltick()
{

}
void CButton::DownEvent(PointerEventData _data)
{
	if (m_ClickTexKey.size() != 0)
	{
		GetOwner()->Image()->SetNormalTexKey(m_ClickTexKey);
	}

	if (m_FuncPtr == nullptr)
		return;

	m_FuncPtr();
}
void CButton::UpEvent(PointerEventData  _data)
{
}
void CButton::EnterEvent(PointerEventData _data)
{
	if (m_EnterTexKey.size() != 0)
	{
		GetOwner()->Image()->SetNormalTexKey(m_EnterTexKey);
	}
}
void CButton::ExitEvent(PointerEventData)
{
	if (m_ExitTexKey.size() != 0)
	{
		GetOwner()->Image()->SetNormalTexKey(m_ExitTexKey);
	}
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
