#include "pch.h"
#include "CAttroxIdleScript.h"
#include "CCharacterTriggerScript.h"
#include "CAttroxMachineScript.h"
void CAttroxIdleScript::OnEntry(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
}

void CAttroxIdleScript::OnExit(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
}

void CAttroxIdleScript::OnEvent(CStateMachineScript* _pSMachine, CTriggerScript* _pTrigger)
{
	CCharacterTriggerScript* pTrigger = dynamic_cast<CCharacterTriggerScript*>(_pTrigger);
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

CAttroxIdleScript::CAttroxIdleScript():CCharacterStateScript(SCRIPT_TYPE::ATTROXIDLESCRIPT)
{
}

CAttroxIdleScript::~CAttroxIdleScript()
{
}
