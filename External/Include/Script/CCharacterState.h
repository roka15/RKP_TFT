#pragma once
#include "CState.h"
#define DECLARE_NAME(n) virtual wstring GetStateName(){return (n);}
enum class STATE_TYPE
{
    NORMAL,
    ULT,
    BATTLE,
};
class CCharacterState
    :public CState
{
protected:
    UINT m_ePrevType;
public:
    virtual wstring GetStateName() = 0;
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override {}
    UINT GetType() { return m_ePrevType; }
public:
    CCharacterState();
    CCharacterState(SCRIPT_TYPE _eType);
    virtual ~CCharacterState();
};

