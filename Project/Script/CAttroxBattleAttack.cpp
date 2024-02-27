#include "pch.h"
#include "CAttroxBattleAttack.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>

void CAttroxBattleAttack::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
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
	trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
	_pSMachine->notify(&trigger);
}

void CAttroxBattleAttack::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pController->SetIntParam(L"Battle", 0);
	pController->SetIntParam(L"Attack", 0);
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
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
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
	case TRIGGER_TYPE::BATTACK:
		pController->SetIntParam(L"Battle", 1);
		pController->SetIntParam(L"Attack", 1);
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
	}
}

CAttroxBattleAttack::CAttroxBattleAttack()
	:CCharacterState()
{
}

CAttroxBattleAttack::~CAttroxBattleAttack()
{
}
