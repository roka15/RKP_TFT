#include "pch.h"
#include "CCharacterTriggerScript.h"

void CCharacterTriggerScript::notify(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
	switch(m_eType)
	{
	default:
		break;
	}

	CTriggerScript::notify(_pSMachine, _pState);
}

CCharacterTriggerScript::CCharacterTriggerScript()
	:CTriggerScript(SCRIPT_TYPE::CHARACTERTRIGGERSCRIPT)
{
}

CCharacterTriggerScript::~CCharacterTriggerScript()
{
}
