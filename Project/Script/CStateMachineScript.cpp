#include "pch.h"
#include "CStateMachineScript.h"
#include "CState.h"
#include "CTrigger.h"
void CStateMachineScript::tick()
{
	m_pState->tick(this);
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

void CStateMachineScript::add_state(CState* _pState, UINT _iType)
{
	m_pStateList[_iType] = _pState;
}

CStateMachineScript::CStateMachineScript():CScript(SCRIPT_TYPE::STATEMACHINESCRIPT)
{
}

CStateMachineScript::CStateMachineScript(SCRIPT_TYPE _eType):CScript(_eType)
{
}

CStateMachineScript::~CStateMachineScript()
{
	for (int i = 0; i < m_pStateList.size(); ++i)
	{
		if (m_pStateList[i] == nullptr)
			continue;

		delete m_pStateList[i];
		m_pStateList[i] = nullptr;
	}
	m_pStateList.clear();
	m_pState = nullptr;
}
