#pragma once
#include "CUI.h"
#include "IMouseEvent.h"

class CButton :
    public CUI, public IMouseDownEvent, public IMouseUpEvent, public IMouseEnterEvent,public IMouseExitEvent
{
private:
    std::function<void()> m_FuncPtr;
    wstring               m_EnterTexKey;
    wstring               m_ExitTexKey;
    wstring               m_ClickTexKey;
public:
    virtual void finaltick()override;
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _FILE)override {};
public:
    virtual void DownEvent(PointerEventData)override;
    virtual void UpEvent(PointerEventData)override;
    virtual void EnterEvent(PointerEventData)override;
    virtual void ExitEvent(PointerEventData)override;
    void RegisterLinkFuncPtr(std::function<void()> _FuncPtr);
    void SetEnterTexKey(wstring _wstrKey) { m_EnterTexKey = _wstrKey; }
    void SetExitTexKey(wstring _wstrKey) { m_ExitTexKey = _wstrKey; }
    void SetClickTexKey(wstring _wstrKey) { m_ClickTexKey = _wstrKey; }
public:
    CLONE(CButton);
    CButton();
    CButton(const CButton& _ref);
    virtual ~CButton();
};

