#pragma once
#include "CCharacterState.h"

class CAttroxIdle :
    public CCharacterState
{
public:
    DECLARE_NAME(L"Idle")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    CLONE(CAttroxIdle)
public:
    CAttroxIdle();
    virtual ~CAttroxIdle();
};

