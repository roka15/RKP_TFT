#include "pch.h"
#include "CAttroxUltIdle.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CCharacterState.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\CKeyMgr.h>


void CAttroxUltIdle::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
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
	pController->SetIntParam(L"Normal", 0);
	pController->SetIntParam(L"Battle", 0);

	CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::UIDLE);
	_pSMachine->notify(&trigger);
}

void CAttroxUltIdle::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//모든 param들 condition 비활성화.
	//pController->SetIntParam(L"ULT", 0);
}

void CAttroxUltIdle::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
	case TRIGGER_TYPE::UIDLE:
		pController->SetIntParam(L"ULT", 1);
		break;
	case TRIGGER_TYPE::BIDLE:
		pMachine->transition(STATE_TYPE::BIDLE);
		break;
	case TRIGGER_TYPE::UMOVE:
		pMachine->transition(STATE_TYPE::UMOVE);
		break;
	case TRIGGER_TYPE::DANCE:
		break;
	}
}

void CAttroxUltIdle::tick(CStateMachineScript* _pSMachine)
{
	CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;

	if (CKeyMgr::GetInst()->GetKeyState(KEY::F) == KEY_STATE::TAP)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::BIDLE);
		pMachine->notify(&trigger);
	}
}
CAttroxUltIdle::CAttroxUltIdle()
	:CCharacterState()
{
}
CAttroxUltIdle::~CAttroxUltIdle()
{
}
