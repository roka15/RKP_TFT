#include "pch.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>

CAttroxIdle CAttroxMachineScript::sIdleState;
CAttroxNormal  CAttroxMachineScript::sNormalState;
CAttroxBattle  CAttroxMachineScript::sBattleState;
CAttroxUlt     CAttroxMachineScript::sUltState;
CAttroxMachineScript::CAttroxMachineScript() :CStateMachineScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT)
{
	m_pState = &sBattleState;
}

void CAttroxMachineScript::transition(STATE_TYPE _eState)
{
	switch (_eState)
	{
	case STATE_TYPE::NORMAL:
		CStateMachineScript::transition(&sNormalState);
		break;
	case STATE_TYPE::ULT:
		CStateMachineScript::transition(&sUltState);
		break;
	case STATE_TYPE::BATTLE:
		CStateMachineScript::transition(&sBattleState);
		break;
	}
}

CAttroxMachineScript::~CAttroxMachineScript()
{
}
