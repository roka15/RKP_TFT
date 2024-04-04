#include "pch.h"
#include "CZedMove.h"
#include "CZedMachineScript.h"
#include "CCharacterTrigger.h"
#include "CBaseCharacterScript.h"
void CZedMove::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
	CCharacterState* pState = dynamic_cast<CCharacterState*>(_pState);
	if (pState == nullptr)
		return;

	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;

	pAni->SetIntParam(L"Move", 1);
}

void CZedMove::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;

	//모든 param들 condition 비활성화.
	pAni->SetIntParam(L"Move", 0);
}

void CZedMove::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
{
	CCharacterTrigger* pTrigger = dynamic_cast<CCharacterTrigger*>(_pTrigger);
	if (pTrigger == nullptr)
		return;
	TRIGGER_TYPE eType = pTrigger->GetEvtType();
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	CZedMachineScript* pMachine = dynamic_cast<CZedMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	switch (eType)
	{
	case TRIGGER_TYPE::NIDLE:
		pMachine->transition((UINT)STATE_TYPE::NIDLE);
		break;
	case TRIGGER_TYPE::BATTACK:
		pMachine->transition((UINT)STATE_TYPE::BATTACK);
		break;
	}
}

void CZedMove::tick(CStateMachineScript* _pSMachine)
{
	CZedMachineScript* pMachine = dynamic_cast<CZedMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	if (pChScript == nullptr)
		return;
	bool bMove = pChScript->IsMove();
	bool bAttack = pChScript->IsAttack();
	
	CCharacterTrigger trigger;
	bool bChange = false;
	if (bAttack)
	{
		trigger.SetEvtType(TRIGGER_TYPE::BATTACK);
		bChange = true;
	}
	else if (bMove == false)
	{
		trigger.SetEvtType(TRIGGER_TYPE::NIDLE);
		bChange = true;
	}

	if (bChange)
		pMachine->notify(&trigger);
}
CZedMove::CZedMove():CCharacterState()
{
}

CZedMove::~CZedMove()
{
}
