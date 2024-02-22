#pragma once
#include "CState.h"
#define DECLARE_NAME(n) virtual wstring GetStateName(){return (n);}
class CCharacterState
    :public CState
{
public:
    virtual wstring GetStateName() = 0;
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
public:
    CCharacterState();
    CCharacterState(SCRIPT_TYPE _eType);
    virtual ~CCharacterState();
};

