#pragma once
#include "CStateMachineScript.h"
#include "CZedIdle.h"
#include "CZedAttack.h"
#include "CZedMove.h"
class CZedMachineScript :
    public CStateMachineScript
{
private:
public:
    virtual void transition(UINT _iType)override;
    CLONE(CZedMachineScript)
public:
    CZedMachineScript();
    CZedMachineScript(const CZedMachineScript& _ref);
    virtual ~CZedMachineScript();
};

