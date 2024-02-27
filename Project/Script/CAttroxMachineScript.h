#pragma once
#include "CStateMachineScript.h"
#include "CAttroxBattleIdle.h"
#include "CAttroxNormalIdle.h"
#include "CAttroxUltIdle.h"
#include "CAttroxUltMove.h"
#include "CAttroxUltAttack.h"
#include "CAttroxBattleMove.h"
#include "CAttroxBattleAttack.h"
#include "CAttroxDance.h"
class CAttroxMachineScript :
    public CStateMachineScript
{
private:
    static CAttroxNormalIdle    sNIdleState;
    static CAttroxBattleIdle    sBIdleState;
    static CAttroxUltIdle       sUIdleState;
    static CAttroxUltMove       sUMoveState;
    static CAttroxUltAttack     sUAttackState;
    static CAttroxBattleMove    sBMoveState;
    static CAttroxBattleAttack  sBAttackState;
    static CAttroxDance         sDanceState;
public:
    virtual void transition(STATE_TYPE _eState);
    CLONE(CAttroxMachineScript)
public:
    CAttroxMachineScript();
    virtual ~CAttroxMachineScript();
};

