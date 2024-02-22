#include "pch.h"
#include "CCharacterTrigger.h"

void CCharacterTrigger::notify(CStateMachineScript* _pSMachine, CState* _pState)
{
	switch(m_eType)
	{
	default:
		break;
	}

	CTrigger::notify(_pSMachine, _pState);
}

CCharacterTrigger::CCharacterTrigger()
{
}

CCharacterTrigger::~CCharacterTrigger()
{
}
