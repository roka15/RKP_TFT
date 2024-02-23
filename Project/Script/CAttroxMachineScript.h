#pragma once
#include "CStateMachineScript.h"
#include "CAttroxIdle.h"
#include "CAttroxBattle.h"
#include "CAttroxNormal.h"
#include "CAttroxUlt.h"
class CAttroxMachineScript :
    public CStateMachineScript
{
private:
    static CAttroxIdle sIdleState;
    static CAttroxNormal sNormalState;
    static CAttroxBattle sBattleState;
    static CAttroxUlt    sUltState;
public:
    virtual void transition(STATE_TYPE _eState);
    CLONE(CAttroxMachineScript)
public:
    CAttroxMachineScript();
    virtual ~CAttroxMachineScript();
};

