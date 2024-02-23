#include "pch.h"
#include "CCharacterState.h"
#include "CState.h"
#include "CCharacterTrigger.h"
void CCharacterState::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CCharacterState::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CCharacterState::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
	CCharacterTrigger* pTrigger = dynamic_cast<CCharacterTrigger*>(_pTrigger);
	if (pTrigger == nullptr)
		return;
	m_ePrevType = (UINT)pTrigger->GetEvtType();
}

CCharacterState::CCharacterState():CState()
{
}
CCharacterState::~CCharacterState()
{
}
