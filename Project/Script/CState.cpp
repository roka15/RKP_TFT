#include "pch.h"
#include "CState.h"
#include "CStateMachineScript.h"
#include "CCharacterTrigger.h"

void CState::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
}
void CState::Transition(CStateMachineScript* _pSMachine, CState* _pState)
{
	if (_pSMachine)
	{
		_pSMachine->transition(_pState);
	}
}

CState::CState()
{
}

CState::~CState()
{
}
