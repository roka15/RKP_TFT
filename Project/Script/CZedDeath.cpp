#include "pch.h"
#include "CZedDeath.h"
#include "CZedMachineScript.h"
#include "CCharacterTrigger.h"
#include "CBaseCharacterScript.h"
void CZedDeath::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CZedDeath::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
}

void CZedDeath::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
	}
}

void CZedDeath::tick(CStateMachineScript* _pSMachine)
{
	CZedMachineScript* pMachine = dynamic_cast<CZedMachineScript*>(_pSMachine);
	if (pMachine == nullptr)
		return;
	CBaseCharacterScript* pChScript = _pSMachine->GetOwner()->GetScript<CBaseCharacterScript>();
	if (pChScript == nullptr)
		return;
	bool bDeath = pChScript->IsDeath();

	CCharacterTrigger trigger;
	bool bChange = false;
	if (bDeath == false)
	{
		trigger.SetEvtType(TRIGGER_TYPE::NIDLE);
		bChange = true;
	}

	if (bChange)
		pMachine->notify(&trigger);
}
CZedDeath::CZedDeath() :CCharacterState()
{

}
CZedDeath::~CZedDeath()
{
}
