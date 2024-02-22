#include "pch.h"
#include "CTriggerScript.h"
#include "CStateMachineScript.h"
#include "CStateScript.h"

void CTriggerScript::tick()
{
}

void CTriggerScript::notify(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
	if (_pState)
	{
		_pState->OnEvent(_pSMachine, this);
	}
}

CTriggerScript::CTriggerScript():CScript(SCRIPT_TYPE::TRIGGERSCRIPT)
{
}

CTriggerScript::CTriggerScript(SCRIPT_TYPE _eType):CScript(_eType)
{
}

CTriggerScript::~CTriggerScript()
{
}
