#include "pch.h"
#include "CStateScript.h"
#include "CStateMachineScript.h"

void CStateScript::tick()
{
}

void CStateScript::Transition(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
	if (_pSMachine)
	{
		_pSMachine->transition(_pState);
	}
}

CStateScript::CStateScript():CScript(SCRIPT_TYPE::STATESCRIPT)
{
}

CStateScript::CStateScript(SCRIPT_TYPE _eType) : CScript(_eType)
{
}

CStateScript::~CStateScript()
{
}
