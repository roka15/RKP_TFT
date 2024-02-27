#pragma once
#include "CCharacterState.h"
class CAttroxUltMove :
    public CCharacterState
{
protected:
public:
    DECLARE_NAME(L"CAttroxUltMove")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override;
    CLONE(CAttroxUltMove)
public:
    CAttroxUltMove();
    virtual ~CAttroxUltMove();
};

