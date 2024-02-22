#pragma once
#include <Engine\CScript.h>
class CStateMachineScript;
class CCharacterScript : public CScript
{
private:
	CStateMachineScript* m_pSMachine;
public:
	virtual void tick();
	
	CLONE(CCharacterScript);
public:
	CCharacterScript();
	virtual ~CCharacterScript();
};

