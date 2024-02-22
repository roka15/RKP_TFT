#pragma once
#include "CCharacterStateScript.h"

class CAttroxIdleScript :
    public CCharacterStateScript , IChState
{
public:
    DECLARE_NAME(L"Idle")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CStateScript* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CStateScript* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTriggerScript* _pTrigger)override;
    CLONE(CAttroxIdleScript)
public:
    CAttroxIdleScript();
    virtual ~CAttroxIdleScript();
};

