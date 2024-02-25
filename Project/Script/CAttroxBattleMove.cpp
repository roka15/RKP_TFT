#include "pch.h"
#include "CAttroxBattleMove.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>

void CAttroxBattleMove::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
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

	//��� param�� condition ��Ȱ��ȭ.
	pController->SetIntParam(L"ULT", 0);
	pController->SetIntParam(L"Normal", 0);

	CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::BMOVE);
	_pSMachine->notify(&trigger);
}

void CAttroxBattleMove::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//��� param�� condition ��Ȱ��ȭ.
	pController->SetIntParam(L"Move", 0);
}

void CAttroxBattleMove::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
	case TRIGGER_TYPE::BATTACK:
		pMachine->transition(STATE_TYPE::BATTACK);
		break;
	case TRIGGER_TYPE::BIDLE:
		pMachine->transition(STATE_TYPE::BIDLE);
		break;
	case TRIGGER_TYPE::BMOVE:
		pController->SetIntParam(L"Move", 1);
		break;
	case TRIGGER_TYPE::UMOVE:
		pMachine->transition(STATE_TYPE::UMOVE);
		break;
	case TRIGGER_TYPE::DANCE:
		pMachine->transition(STATE_TYPE::DANCE);
		break;
	}
}

void CAttroxBattleMove::tick(CStateMachineScript* _pSMachine)
{
	CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_1) == KEY_STATE::TAP)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::BIDLE);
		pMachine->notify(&trigger);
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_2) == KEY_STATE::TAP)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
		pMachine->notify(&trigger);
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_3) == KEY_STATE::TAP)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::DANCE);
		pMachine->notify(&trigger);
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::F) == KEY_STATE::TAP)
	{
		CCharacterTrigger trigger;
		trigger.SetEvtType(TRIGGER_TYPE::UMOVE);
		pMachine->notify(&trigger);
	}
}
CAttroxBattleMove::CAttroxBattleMove()
{

}
CAttroxBattleMove::~CAttroxBattleMove()
{
}
