#include "pch.h"
#include "CZedAttack.h"
#include "CZedMachineScript.h"
#include "CCharacterTrigger.h"
#include "CBaseCharacterScript.h"
void CZedAttack::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
	CCharacterState* pState = dynamic_cast<CCharacterState*>(_pState);
	if (pState == nullptr)
		return;

	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;

	pAni->SetBoolParam(L"Attack", true);
}

void CZedAttack::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pAni->SetBoolParam(L"Attack", false);
}

void CZedAttack::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
	case TRIGGER_TYPE::NIDLE:
		pMachine->transition((UINT)STATE_TYPE::NIDLE);
		break;
	case TRIGGER_TYPE::BMOVE:
		pMachine->transition((UINT)STATE_TYPE::BMOVE);
		break;
	case TRIGGER_TYPE::BATTACK:
		pAni->SetIntParam(L"Attack_Number", 0);
		break;
	case TRIGGER_TYPE::BATTACK2:
		pAni->SetIntParam(L"Attack_Number", 1);
		break;
	case TRIGGER_TYPE::BATTACK3:
		pAni->SetIntParam(L"Attack_Number", 2);
		break;
	}
}

void CZedAttack::tick(CStateMachineScript* _pSMachine)
{
	CZedMachineScript* pMachine = dynamic_cast<CZedMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	if (pChScript == nullptr)
		return;

	bool bMove = pChScript->IsMove();
	bool bAttack = pChScript->IsAttack();
	bool bDance = pChScript->IsDance();
	int  iAtkNum = pChScript->GetAtkNumber();

	CCharacterTrigger trigger;
	bool bChange = false;
	if (bMove)
	{
		trigger.SetEvtType(TRIGGER_TYPE::BMOVE);
		bChange = true;
	}
	else if (bAttack)
	{
		switch (iAtkNum)
		{
		case 0:
			trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
			break;
		case 1:
			trigger.SetEvtType(TRIGGER_TYPE::BATTACK2);
			break;
		case 2:
			trigger.SetEvtType(TRIGGER_TYPE::BATTACK3);
			break;
		}

		bChange = true;
	}
	else
	{
		trigger.SetEvtType(TRIGGER_TYPE::NIDLE);
		bChange = true;
	}
	if (bChange)
		pMachine->notify(&trigger);
}
CZedAttack::CZedAttack() : CCharacterState()
{
}
CZedAttack::~CZedAttack()
{
}
