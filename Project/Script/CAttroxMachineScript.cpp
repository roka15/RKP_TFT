#include "pch.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include <Engine\CTimeMgr.h>

CAttroxIdle CAttroxMachineScript::sIdleState;
CAttroxMachineScript::CAttroxMachineScript() :CStateMachineScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT),
m_dUltGauge(0.0),
m_bUlt(false),
m_dUltMaxCondition(20.0),
m_dUltMinCondition(0.0)
{
	m_pState = &sIdleState;
}
void CAttroxMachineScript::tick()
{
	if (m_bUlt)
	{
		m_dUltGauge -= DT;
		if (m_dUltGauge <= m_dUltMinCondition)
		{
			m_bUlt = false;
			m_dUltGauge = 0.0;	
			//transition(&sIdleState);
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::NORMAL);
			notify(&trigger);
		}
	}
	else
	{
		m_dUltGauge += DT;
		if (m_dUltGauge >= m_dUltMaxCondition)
		{
			m_bUlt = true;
			CCharacterTrigger trigger;
			trigger.SetEvtType(TRIGGER_TYPE::ULT);
			notify(&trigger);
		}
	}
	
	
}
CAttroxMachineScript::~CAttroxMachineScript()
{
}
