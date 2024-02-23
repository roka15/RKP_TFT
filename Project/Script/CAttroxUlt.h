#pragma once
#include "CCharacterState.h"
class CAttroxUlt :
    public CCharacterState
{
protected:
    double m_dTime;
    double m_dNextTime;
public:
    DECLARE_NAME(L"Normal")
    virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState)override;
    virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)override;
    virtual void tick(CStateMachineScript* _pSMachine)override;
    CLONE(CAttroxUlt)
public:
    CAttroxUlt();
    virtual ~CAttroxUlt();
};

