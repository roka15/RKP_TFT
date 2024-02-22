#include "pch.h"
#include "CStateMachineScript.h"
#include "CState.h"
#include "CTrigger.h"
void CStateMachineScript::tick()
{
}

void CStateMachineScript::transition(CState* _pState)
{
	CState* previous = m_pState;
	CState* current = _pState;

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

void CStateMachineScript::notify(CTrigger* _pTrigger)
{
	if (_pTrigger)
		_pTrigger->notify(this, m_pState);
}

CState* CStateMachineScript::get_state(void) const
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
