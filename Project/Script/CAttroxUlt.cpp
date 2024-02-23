#include "pch.h"
#include "CAttroxUlt.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CCharacterState.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>


void CAttroxUlt::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
	CCharacterState* pState = dynamic_cast<CCharacterState*>(_pState);
	if (pState == nullptr)
		return;
	TRIGGER_TYPE eType = (TRIGGER_TYPE)(pState->GetType());
	switch (eType)
	{
		case TRIGGER_TYPE::IDLE:
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::IDLE);
			_pSMachine->notify(&trigger);
		break;
	}
}

void CAttroxUlt::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CAttroxUlt::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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

	switch (eType)
	{
	case TRIGGER_TYPE::IDLE:
		pController->SetIntParam(L"ULT_Idle", 1);
		break;
	case TRIGGER_TYPE::WALK:
		break;
	case TRIGGER_TYPE::DANCE:
		break;
	}
	m_ePrevType = (UINT)eType;
}

void CAttroxUlt::tick(CStateMachineScript* _pSMachine)
{
	m_dTime += DT;
	if (m_dTime >= m_dNextTime)
	{
		CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine);
		if (pMachine == nullptr)
			return;

		pMachine->transition(STATE_TYPE::BATTLE);
	}
}
CAttroxUlt::CAttroxUlt()
	:CCharacterState(),
	m_dTime(0.0),
	m_dNextTime(10.0)
{
}
CAttroxUlt::~CAttroxUlt()
{
}
