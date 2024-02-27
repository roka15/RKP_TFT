#include "pch.h"
#include "CAttroxUltMove.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CCharacterState.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\CKeyMgr.h>


void CAttroxUltMove::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
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
	trigger.SetEvtType(TRIGGER_TYPE::UMOVE);
	_pSMachine->notify(&trigger);
}

void CAttroxUltMove::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//��� param�� condition ��Ȱ��ȭ.
	pController->SetIntParam(L"ULT", 0);
	pController->SetIntParam(L"Move", 0);
	//pController->SetTriggerParam(L"UltOut", true);
}

void CAttroxUltMove::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
		pMachine->transition(STATE_TYPE::BIDLE);
		break;
	case TRIGGER_TYPE::BMOVE:
		pMachine->transition(STATE_TYPE::BMOVE);
		break;
	case TRIGGER_TYPE::BATTACK:
		pMachine->transition(STATE_TYPE::BATTACK);
		break;
	case TRIGGER_TYPE::UIDLE:
		pMachine->transition(STATE_TYPE::UIDLE);
		break;
	case TRIGGER_TYPE::UMOVE:
		pController->SetIntParam(L"ULT", 1);
		pController->SetIntParam(L"Move", 1);
		break;
	case TRIGGER_TYPE::UATTACK:
		pMachine->transition(STATE_TYPE::UATTACK);
		break;
	case TRIGGER_TYPE::DANCE:
		pMachine->transition(STATE_TYPE::DANCE);
		break;
	}
}

CAttroxUltMove::CAttroxUltMove()
	:CCharacterState()
{
}
CAttroxUltMove::~CAttroxUltMove()
{
}
