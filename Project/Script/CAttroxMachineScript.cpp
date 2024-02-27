#include "pch.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>

CAttroxNormalIdle  CAttroxMachineScript::sNIdleState;
CAttroxBattleIdle  CAttroxMachineScript::sBIdleState;
CAttroxUltIdle    CAttroxMachineScript::sUIdleState;
CAttroxBattleMove CAttroxMachineScript::sBMoveState;
CAttroxBattleAttack CAttroxMachineScript::sBAttackState;
CAttroxDance         CAttroxMachineScript::sDanceState;
CAttroxUltMove       CAttroxMachineScript::sUMoveState;
CAttroxUltAttack	 CAttroxMachineScript::sUAttackState;
CAttroxMachineScript::CAttroxMachineScript() :CStateMachineScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT)
{
	m_pState = &sNIdleState;
}

void CAttroxMachineScript::transition(STATE_TYPE _eState)
{
	switch (_eState)
	{
	case STATE_TYPE::NIDLE:
		CStateMachineScript::transition(&sNIdleState);
		break;
	case STATE_TYPE::UIDLE:
		CStateMachineScript::transition(&sUIdleState);
		break;
	case STATE_TYPE::UMOVE:
		CStateMachineScript::transition(&sUMoveState);
		break;
	case STATE_TYPE::UATTACK:
		CStateMachineScript::transition(&sUAttackState);
		break;
	case STATE_TYPE::BIDLE:
		CStateMachineScript::transition(&sBIdleState);
		break;
	case STATE_TYPE::BMOVE:
		CStateMachineScript::transition(&sBMoveState);
		break;
	case STATE_TYPE::BATTACK:
		CStateMachineScript::transition(&sBAttackState);
		break;
	case STATE_TYPE::DANCE:
		CStateMachineScript::transition(&sDanceState);
		break;
	}
}

CAttroxMachineScript::~CAttroxMachineScript()
{
}
