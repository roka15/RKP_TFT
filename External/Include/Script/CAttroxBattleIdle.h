#pragma once
#include "CCharacterState.h"
class CAttroxBattleIdle:
    public CCharacterState
{
protected:
public:
    DECLARE_NAME(L"CAttroxBattleIdle")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override;
    CLONE(CAttroxBattleIdle)
public:
    CAttroxBattleIdle();
    virtual ~CAttroxBattleIdle();
};

