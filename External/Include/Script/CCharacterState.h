#pragma once
#include "CState.h"
#define DECLARE_NAME(n) virtual wstring GetStateName(){return (n);}
enum class STATE_TYPE
{
    NIDLE,
    BIDLE,
    UIDLE,
    BMOVE,
    UMOVE,
    BATTACK,
    UATTACK,
    DANCE,
    DEATH,
    END,
};
class CCharacterState
    :public CState
{
public:
    virtual wstring GetStateName() = 0;
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override;
public:
    CCharacterState();
    virtual ~CCharacterState();
};

