#include "pch.h"
#include "CState.h"
#include "CStateMachineScript.h"


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
