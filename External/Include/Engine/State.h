#pragma once
#include "CEntity.h"
class CTrigger;
class CState :
    public CEntity
{
public:
	virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState) {};
	virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState) {};
	virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger) {};
	virtual void Transition(CStateMachineScript* _pSMachine, CState* _pState);
};

