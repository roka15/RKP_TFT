#include "pch.h"
#include "CAttroxBattleIdle.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>
#include "CBaseCharacterScript.h"

void CAttroxBattleIdle::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
	CCharacterState* pState = dynamic_cast<CCharacterState*>(_pState);
	if (pState == nullptr)
		return;

	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::BIDLE);
	_pSMachine->notify(&trigger);
}

void CAttroxBattleIdle::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pController->SetIntParam(L"Battle", 0);
}

void CAttroxBattleIdle::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
	CCharacterTrigger* pTrigger = dynamic_cast<CCharacterTrigger*>(_pTrigger);
	if (pTrigger == nullptr)
		return;
	TRIGGER_TYPE eType = pTrigger->GetEvtType();
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;
	CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	switch (eType)
	{
	case TRIGGER_TYPE::NIDLE:
		pMachine->transition((UINT)STATE_TYPE::NIDLE);
		break;
	case TRIGGER_TYPE::BIDLE:
		pController->SetIntParam(L"Battle", 1);
		break;
	case TRIGGER_TYPE::BMOVE:
		pMachine->transition((UINT)STATE_TYPE::BMOVE);
		break;

	case TRIGGER_TYPE::BATTACK:
		pMachine->transition((UINT)STATE_TYPE::BATTACK);
		break;
	case TRIGGER_TYPE::UATTACK:
		pMachine->transition((UINT)STATE_TYPE::UATTACK);
		break;
	case TRIGGER_TYPE::DANCE:
		pMachine->transition((UINT)STATE_TYPE::DANCE);
		break;
	}
}

void CAttroxBattleIdle::tick(CStateMachineScript* _pSMachine)
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

	CCharacterTrigger trigger;
	bool bChange = false;

	if (bDance)
	{
		trigger.SetEvtType(TRIGGER_TYPE::DANCE);
		bChange = true;
	}

	if (bUlt)
	{
		if (bAttack)
		{
			trigger.SetEvtType(TRIGGER_TYPE::UATTACK);
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
			bChange = true;
		}
		else if (bWait)
		{
			trigger.SetEvtType(TRIGGER_TYPE::NIDLE);
			bChange = true;
		}
	}
	if (bChange)
		pMachine->notify(&trigger);
}

CAttroxBattleIdle::CAttroxBattleIdle()
	:CCharacterState()
{
}

CAttroxBattleIdle::~CAttroxBattleIdle()
{
}
