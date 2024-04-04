#pragma once
#include "CCharacterState.h"
class CZedAttack :
    public CCharacterState
{
protected:
public:
    DECLARE_NAME(L"CZedAttack")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override;
    CLONE(CZedAttack)
public:
    CZedAttack();
    virtual ~CZedAttack();
};

