#include "pch.h"
#include "CAttroxIdle.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
void CAttroxIdle::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CAttroxIdle::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CAttroxIdle::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
	CCharacterTrigger* pTrigger = dynamic_cast<CCharacterTrigger*>(_pTrigger);
	if (pTrigger == nullptr)
		return;

	TRIGGER_TYPE eType = pTrigger->GetEvtType();
	CAnimator3D* pAni = _pSMachine->Animator3D();
	Ptr<CAnimatorController> pController = nullptr;
	if (pAni)
		pController = pAni->GetController();

	if (pController == nullptr)
		return;

	switch (eType)
	{
	case TRIGGER_TYPE::NORMAL:
		pController->SetIntParam(L"Normal_Idle", 0);
		break;
	case TRIGGER_TYPE::ULT:
		pController->SetIntParam(L"ULT_Idle", 1);
		break;
	case TRIGGER_TYPE::BATTLE:
		pController->SetIntParam(L"Battle_Idle", 2);
		break;
	}
}

CAttroxIdle::CAttroxIdle():CCharacterState()
{
}

CAttroxIdle::~CAttroxIdle()
{
}
