#pragma once
#include "UI.h"
class AniControllerEditParamUI :
    public UI
{
private:
public:
    virtual void init();
    virtual void tick();
    virtual void finaltick();
    virtual int render_update();
public:
    AniControllerEditParamUI();
    virtual ~AniControllerEditParamUI();
};

