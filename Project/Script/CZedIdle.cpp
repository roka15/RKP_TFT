#include "pch.h"
#include "CZedIdle.h"
#include "CZedMachineScript.h"
#include "CBaseCharacterScript.h"
#include "CCharacterTrigger.h"
void CZedIdle::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CZedIdle::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CZedIdle::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
	CCharacterTrigger* pTrigger = dynamic_cast<CCharacterTrigger*>(_pTrigger);
	if (pTrigger == nullptr)
		return;
	TRIGGER_TYPE eType = pTrigger->GetEvtType();
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	CZedMachineScript* pMachine = dynamic_cast<CZedMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	switch (eType)
	{
	case TRIGGER_TYPE::BMOVE:
		pMachine->transition((UINT)STATE_TYPE::BMOVE);
		break;
	case TRIGGER_TYPE::BATTACK:
		pMachine->transition((UINT)STATE_TYPE::BATTACK);
		break;
	case TRIGGER_TYPE::DEATH:
		pMachine->transition((UINT)STATE_TYPE::DEATH);
		break;
	}
}

void CZedIdle::tick(CStateMachineScript* _pSMachine)
{
	CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	if (pChScript == nullptr)
		return;

	bool bMove = pChScript->IsMove();
	bool bAtk = pChScript->IsAttack();
	int  bAtkNum = pChScript->GetAtkNumber();
	bool bDeath = pChScript->IsDeath();
	bool bChange = false;


	CCharacterTrigger trigger;
	if (bDeath)
	{
		trigger.SetEvtType(TRIGGER_TYPE::DEATH);
		bChange = true;
	}
	else if (bMove)
	{
		trigger.SetEvtType(TRIGGER_TYPE::BMOVE);
		bChange = true;
	}
	else if (bAtk)
	{
		trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
		bChange = true;
	}
	

	if (bChange)
		_pSMachine->notify(&trigger);
}
CZedIdle::CZedIdle() : CCharacterState()
{
}
CZedIdle::~CZedIdle()
{
}
