#pragma once
#include "ComponentUI.h"
class BillbardUI :
    public ComponentUI
{
public:
    virtual int render_update() override;


public:
    void SelectMesh(DWORD_PTR _Key);
    void SelectMaterial(DWORD_PTR _Key);


public:
    BillbardUI();
    ~BillbardUI();
};

