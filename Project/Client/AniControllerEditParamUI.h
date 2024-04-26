#pragma once
#include "UI.h"
class CAnimator3D;
#define ANI_MAX_PARAM_TYPE 4

struct EditParamInfo
{
    PARAM_TYPE eType;
    wstring    prevName;
    wstring    nextName;

};
class AniControllerEditParamUI :
    public UI
{
private:
    CAnimator3D* m_pLink;
    bool         m_bFlow;
    const char* m_ParamTypeNames[ANI_MAX_PARAM_TYPE];
    int m_intcnt;
    int m_floatcnt;
    int m_boolcnt;
    int m_triggercnt;
public:
    virtual void init();
    virtual void tick();
    virtual void finaltick();
    virtual int render_update();
    void SetLink(CAnimator3D* _pAni); 
    const bool& IsFlow() { return m_bFlow; }
public:
    AniControllerEditParamUI();
    virtual ~AniControllerEditParamUI();
};

