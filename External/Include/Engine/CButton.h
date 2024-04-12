#pragma once
#include "CUI.h"
#include "IMouseEvent.h"
class CButton :
    public CUI, public IMouseDownEvent
{
public:
    virtual void finaltick()override;
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _FILE)override {};
public:
    virtual void DownEvent(PointerEventData)override;
public:
    CLONE(CButton);
    CButton();
    CButton(const CButton& _ref);
    virtual ~CButton();
};

