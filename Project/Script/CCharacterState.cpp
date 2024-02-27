#include "pch.h"
#include "CCharacterState.h"
#include "CState.h"
#include "CCharacterTrigger.h"
#include "CBaseCharacterScript.h"
#include "CStateMachineScript.h"
void CCharacterState::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CCharacterState::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CCharacterState::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
}
void CCharacterState::tick(CStateMachineScript* _pSMachine)
{
	/*CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	if (pChScript == nullptr)
		return;

	bool bUlt = pChScript->IsUlt();
	bool bWait = pChScript->IsWait();
	bool bMove = pChScript->IsMove();
	bool bAttack = pChScript->IsAttack();
	bool bDance = pChScript->IsDance();
	bool bEnd = pChScript->IsEnd();

	if (bWait)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::NIDLE);
		_pSMachine->notify(&trigger);
		return;
	}
	if (bEnd)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::END);
		_pSMachine->notify(&trigger);
		return;
	}
	if (bDance)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::DANCE);
		_pSMachine->notify(&trigger);
		return;
	}

	if (bUlt)
	{
		if (bMove)
		{
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::UMOVE);
			_pSMachine->notify(&trigger);
		}
		else if (bAttack)
		{
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::UATTACK);
			_pSMachine->notify(&trigger);
		}
		else if (bWait == false)
		{
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::UIDLE);
			_pSMachine->notify(&trigger);
		}
	}
	else
	{
		if (bMove)
		{
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::BMOVE);
			_pSMachine->notify(&trigger);
		}
		else if (bAttack)
		{
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
			_pSMachine->notify(&trigger);
		}
		else if (bWait == false)
		{
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::BIDLE);
			_pSMachine->notify(&trigger);
		}
	}*/
}
CCharacterState::CCharacterState():CState()
{
}
CCharacterState::~CCharacterState()
{
}
