#pragma once
#include "CCharacterState.h"
class CAttroxUltAttack :
    public CCharacterState
{
protected:
public:
    DECLARE_NAME(L"CAttroxUltAttack")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    CLONE(CAttroxUltAttack)
public:
    CAttroxUltAttack();
    virtual ~CAttroxUltAttack();
};

