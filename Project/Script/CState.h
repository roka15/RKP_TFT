#pragma once
#include <Engine\CEntity.h>
class CStateMachineScript;
class CTrigger;
class  CState : public CEntity
{
public:
	virtual void OnEntry(CStateMachineScript* _pSMachine, CState* _pState) = 0;
	virtual void OnExit(CStateMachineScript* _pSMachine, CState* _pState) = 0;
	virtual void OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger) = 0;
	virtual void tick(CStateMachineScript* _pSMachine) {}
public:
};

