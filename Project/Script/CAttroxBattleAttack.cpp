#include "pch.h"
#include "CAttroxBattleAttack.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CBaseCharacterScript.h"
#include <Engine\CTimeMgr.h>


void CAttroxBattleAttack::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
	CCharacterState* pState = dynamic_cast<CCharacterState*>(_pState);
	if (pState == nullptr)
		return;
	
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;

	pAni->SetIntParam(L"Battle", 1);
	pAni->SetBoolParam(L"Attack", true);

	//CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	//if (pChScript == nullptr)
	//	return;
	//pChScript->AttackOFF();
}

void CAttroxBattleAttack::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pAni->SetBoolParam(L"Attack", false);

}

void CAttroxBattleAttack::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
	CCharacterTrigger* pTrigger = dynamic_cast<CCharacterTrigger*>(_pTrigger);
	if (pTrigger == nullptr)
		return;
	TRIGGER_TYPE eType = pTrigger->GetEvtType();
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	switch (eType)
	{
	case TRIGGER_TYPE::BIDLE:
		pMachine->transition((UINT)STATE_TYPE::BIDLE);
		break;
	case TRIGGER_TYPE::BMOVE:
		pMachine->transition((UINT)STATE_TYPE::BMOVE);
		break;
	case TRIGGER_TYPE::UIDLE:
		pMachine->transition((UINT)STATE_TYPE::UIDLE);
		break;
	case TRIGGER_TYPE::UMOVE:
		pMachine->transition((UINT)STATE_TYPE::UMOVE);
		break;
	case TRIGGER_TYPE::UATTACK:
		pMachine->transition((UINT)STATE_TYPE::UATTACK);
		break;
	case TRIGGER_TYPE::DANCE:
		pMachine->transition((UINT)STATE_TYPE::DANCE);
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

void CAttroxBattleAttack::tick(CStateMachineScript* _pSMachine)
{
	CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	if (pChScript == nullptr)
		return;
	bool bUlt = pChScript->IsUlt();
	bool bWait = pChScript->IsWait();
	bool bMove = pChScript->IsMove();
	bool bAttack = pChScript->IsAttack();
	bool bDance = pChScript->IsDance();
	int  bAtkNum = pChScript->GetAtkNumber();

	CCharacterTrigger trigger;
	bool bChange = false;

	if (bDance)
	{
		trigger.SetEvtType(TRIGGER_TYPE::DANCE);
		bChange = true;
	}

	if (bUlt)
	{
		if (bMove)
		{
			trigger.SetEvtType(TRIGGER_TYPE::UMOVE);
			bChange = true;
		}
		else
		{
			trigger.SetEvtType(TRIGGER_TYPE::UIDLE);
			bChange = true;
		}
	}
	else
	{
		if (bMove)
		{
			trigger.SetEvtType(TRIGGER_TYPE::BMOVE);
			bChange = true;
		}
		else if (bAttack)
		{
			trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
			switch (bAtkNum)
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
	    else if (bAttack == false && bWait == false)
		{
			trigger.SetEvtType(TRIGGER_TYPE::BIDLE);
			bChange = true;
		}
	}

	if (bChange)
		pMachine->notify(&trigger);
}

CAttroxBattleAttack::CAttroxBattleAttack()
	:CCharacterState()
{
}

CAttroxBattleAttack::~CAttroxBattleAttack()
{
}
