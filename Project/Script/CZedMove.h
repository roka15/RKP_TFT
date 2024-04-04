#pragma once
#include "CCharacterState.h"
class CZedMove :
    public CCharacterState
{
protected:
public:
    DECLARE_NAME(L"CZedMove")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override;
    CLONE(CZedMove)
public:
    CZedMove();
    virtual ~CZedMove();

};

