#pragma once
#include "CUI.h"
#include "IMouseEvent.h"

class CButton :
    public CUI, public IMouseDownEvent
{
private:
    std::function<void()> m_FuncPtr;
public:
    virtual void finaltick()override;
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _FILE)override {};
public:
    virtual void DownEvent(PointerEventData)override;
    void RegisterLinkFuncPtr(std::function<void()> _FuncPtr);
public:
    CLONE(CButton);
    CButton();
    CButton(const CButton& _ref);
    virtual ~CButton();
};

