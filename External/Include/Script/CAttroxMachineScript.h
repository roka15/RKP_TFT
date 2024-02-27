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
public:
    virtual void transition(UINT _iType)override;
    CLONE(CAttroxMachineScript)
public:
    CAttroxMachineScript();
    virtual ~CAttroxMachineScript();
};

