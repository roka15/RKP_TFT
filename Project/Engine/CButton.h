#pragma once
#include "CUI.h"
class CButton :
    public CUI
{
public:
    virtual void finaltick()override {}
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _FILE)override {};
public:
    CLONE(CButton);
    CButton();
    CButton(const CButton& _ref);
    virtual ~CButton();
};

