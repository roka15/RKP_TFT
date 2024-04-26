#pragma once
#include "UI.h"
class CAnimator3D;
class AniControllerEditParamUI :
    public UI
{
private:
    CAnimator3D* m_pLink;
    bool         m_bFlow;
public:
    virtual void init();
    virtual void tick();
    virtual void finaltick();
    virtual int render_update();
    void SetLink(CAnimator3D* _pAni) { m_pLink = _pAni; }
    const bool& IsFlow() { return m_bFlow; }
public:
    AniControllerEditParamUI();
    virtual ~AniControllerEditParamUI();
};

