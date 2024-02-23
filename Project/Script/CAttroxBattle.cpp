#include "pch.h"
#include "CAttroxBattle.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>

void CAttroxBattle::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CAttroxBattle::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pController->SetIntParam(L"Battle_Idle", 0);
}

void CAttroxBattle::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
		pController->SetIntParam(L"Battle_Idle", 1);
		break;
	case TRIGGER_TYPE::WALK:
		break;
	case TRIGGER_TYPE::DANCE:
		break;
	}
	m_ePrevType = (UINT)eType;
}

void CAttroxBattle::tick(CStateMachineScript* _pSMachine)
{
	m_dTime += DT;
	if (m_dTime >= m_dNextTime)
	{
		CAttroxMachineScript* pMachine = dynamic_cast<CAttroxMachineScript*>(_pSMachine); 
		if (pMachine == nullptr)
			return;

		pMachine->transition(STATE_TYPE::ULT);
	}
}

CAttroxBattle::CAttroxBattle()
	:CCharacterState(),
	m_dTime(0.0),
	m_dNextTime(10.0)
{
}

CAttroxBattle::~CAttroxBattle()
{
}
