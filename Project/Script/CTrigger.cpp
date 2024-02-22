#include "pch.h"
#include "CTrigger.h"
#include "CStateMachineScript.h"
#include "CState.h"

void CTrigger::notify(CStateMachineScript* _pSMachine, CState* _pState)
{
	if (_pState)
	{
		_pState->OnEvent(_pSMachine, this);
	}
}

CTrigger::CTrigger()
{
}

CTrigger::~CTrigger()
{
}
