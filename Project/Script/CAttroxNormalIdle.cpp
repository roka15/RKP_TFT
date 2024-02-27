#include "pch.h"
#include "CAttroxNormalIdle.h"
#include "CCharacterTrigger.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CCharacterState.h"
#include <Engine\CTimeMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\CKeyMgr.h>

void CAttroxNormalIdle::OnEntry(CStateMachineScript* _pSMachine, CState* _pState)
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

	CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::NIDLE);
	_pSMachine->notify(&trigger);
}

void CAttroxNormalIdle::OnExit(CStateMachineScript* _pSMachine, CState* _pState)
{
	CAnimator3D* pAni = _pSMachine->Animator3D();
	if (pAni == nullptr)
		return;
	Ptr<CAnimatorController> pController = pAni->GetController();
	if (pController == nullptr)
		return;
}

void CAttroxNormalIdle::OnEvent(CStateMachineScript* _pSMachine, CTrigger* _pTrigger)
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
	case TRIGGER_TYPE::NIDLE:
		pController->SetTriggerParam(L"Normal", true);
		break;
	case TRIGGER_TYPE::BIDLE:
		pMachine->transition((UINT)STATE_TYPE::BIDLE);
		break;
	}
}
CAttroxNormalIdle::CAttroxNormalIdle()
{

}
CAttroxNormalIdle::~CAttroxNormalIdle()
{
}
