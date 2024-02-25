#include "pch.h"
#include "CAttroxDance.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>

void CAttroxDance::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
	CCharacterState* pState = dynamic_cast<CCharacterState*>(_pState);
	if (pState == nullptr)
		return;
	CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::DANCE);
	_pSMachine->notify(&trigger);

	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	pController->SetIntParam(L"Battle", 0);
	pController->SetIntParam(L"Normal", 0);
	pController->SetIntParam(L"ULT", 0);
}

void CAttroxDance::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pController->SetTriggerParam(L"Dance", false);
}

void CAttroxDance::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
	case TRIGGER_TYPE::DANCE:
		pController->SetTriggerParam(L"Dance", true);
		break;
	case TRIGGER_TYPE::END:
		pMachine->transition(STATE_TYPE::NIDLE);
		break;
	}
}

void CAttroxDance::tick(CStateMachineScript* _pSMachine)
{
	CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_1) == KEY_STATE::TAP)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::END);
		pMachine->notify(&trigger);
	}
}
CAttroxDance::CAttroxDance()
{

}
CAttroxDance::~CAttroxDance()
{
}
