#pragma once
#include "ComponentUI.h"

class Animator3DUI :
    public ComponentUI
{
private:

public:
    void SelectClip(DWORD_PTR _Key);
    virtual int render_update() override;
public:
    Animator3DUI();
    virtual ~Animator3DUI();
};

