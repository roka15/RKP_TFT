#include "pch.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>

CAttroxMachineScript::CAttroxMachineScript() :CStateMachineScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT)
{
	//STATE_TYPE 순서 지킬 것.
	add_state(new CAttroxNormalIdle());
	add_state(new CAttroxBattleIdle());
	add_state(new CAttroxUltIdle());
	add_state(new CAttroxBattleMove());
	add_state(new CAttroxUltMove());
	add_state(new CAttroxBattleAttack());
	add_state(new CAttroxUltAttack());
	add_state(new CAttroxDance());

	m_pState = m_pStateList[(UINT)STATE_TYPE::NIDLE];
}

void CAttroxMachineScript::transition(UINT _iType)
{
	CStateMachineScript::transition(m_pStateList[_iType]);
}

CAttroxMachineScript::~CAttroxMachineScript()
{
}
