#include "pch.h"
#include "CStateMachineScript.h"
#include "CStateScript.h"
#include "CTriggerScript.h"
void CStateMachineScript::tick()
{
}

void CStateMachineScript::transition(CStateScript* _pState)
{
	CStateScript* previous = m_pState;
	CStateScript* current = _pState;

	if (previous)
	{
		previous->OnExit(this, current);
	}
	m_pState = current;
	if (m_pState)
	{
		m_pState->OnEntry(this, previous);
	}
}

void CStateMachineScript::notify(CTriggerScript* _pTrigger)
{
	if (_pTrigger)
		_pTrigger->notify(this, m_pState);
}

CStateScript* CStateMachineScript::get_state(void) const
{
	return m_pState;
}

CStateMachineScript::CStateMachineScript():CScript(SCRIPT_TYPE::STATEMACHINESCRIPT)
{
}

CStateMachineScript::CStateMachineScript(SCRIPT_TYPE _eType):CScript(_eType)
{
}

CStateMachineScript::~CStateMachineScript()
{
}
