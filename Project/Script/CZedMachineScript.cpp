#include "pch.h"
#include "CZedMachineScript.h"
CZedMachineScript::CZedMachineScript() :CStateMachineScript(SCRIPT_TYPE::ZEDMACHINESCRIPT)
{
	m_pStateList.resize((UINT)STATE_TYPE::END);
	add_state(new CZedIdle(),(UINT)STATE_TYPE::NIDLE);
	add_state(new CZedAttack(), (UINT)STATE_TYPE::BATTACK);
	add_state(new CZedMove(), (UINT)STATE_TYPE::BMOVE);
	add_state(new CZedDeath(), (UINT)STATE_TYPE::DEATH);

	m_pState = m_pStateList[(UINT)STATE_TYPE::NIDLE];
}
CZedMachineScript::CZedMachineScript(const CZedMachineScript& _ref) : CStateMachineScript(SCRIPT_TYPE::ZEDMACHINESCRIPT)
{
	m_pStateList.resize((UINT)STATE_TYPE::END);
	add_state(new CZedIdle(), (UINT)STATE_TYPE::NIDLE);
	add_state(new CZedAttack(), (UINT)STATE_TYPE::BATTACK);
	add_state(new CZedMove(), (UINT)STATE_TYPE::BMOVE);
	add_state(new CZedDeath(), (UINT)STATE_TYPE::DEATH);

	m_pState = m_pStateList[(UINT)STATE_TYPE::NIDLE];
}

CZedMachineScript::~CZedMachineScript()
{
}


void CZedMachineScript::transition(UINT _iType)
{
	CStateMachineScript::transition(m_pStateList[_iType]);
}


