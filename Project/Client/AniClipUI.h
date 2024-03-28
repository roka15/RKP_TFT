#pragma once
#include "ResUI.h"
class AniClipUI :
    public ResUI
{
private:
    UINT    m_curSliderFrame;
public:
  
public:
    AniClipUI();
    virtual int render_update() override;
    virtual ~AniClipUI();
};

