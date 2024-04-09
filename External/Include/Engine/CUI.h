#pragma once
#include "CComponent.h"
class CUI :
    public CComponent
{
public:
    virtual void finaltick()override {};
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _FILE)override {};
public:

public:
    CLONE(CUI);
    CUI();
    CUI(COMPONENT_TYPE _eType);
    CUI(const CUI& _ref);
    virtual ~CUI();
};

