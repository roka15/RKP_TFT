#pragma once
#include <Engine\CScript.h>
class CStateMachineScript;
class CTriggerScript;
class CStateScript :
	public CScript
{
private:
public:
	virtual void tick()override;
	virtual void BeginOverlap(CCollider2D* _Other)override {}
	virtual void OnOverlap(CCollider2D* _Other) override {}
	virtual void EndOverlap(CCollider2D* _Other)override {}

public:
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _FILE) override {}
public:
	virtual void OnEntry(CStateMachineScript* _pSMachine, CStateScript* _pState) {};
	virtual void OnExit(CStateMachineScript* _pSMachine, CStateScript* _pState) {};
	virtual void OnEvent(CStateMachineScript* _pSMachine, CTriggerScript* _pTrigger) {};
	virtual void Transition(CStateMachineScript* _pSMachine, CStateScript* _pState);
	CLONE(CStateScript)
public:
	CStateScript();
	CStateScript(SCRIPT_TYPE _eType);
	virtual ~CStateScript();
};

