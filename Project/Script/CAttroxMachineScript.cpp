#include "pch.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTriggerScript.h"
#include <Engine\CTimeMgr.h>

CAttroxIdleScript CAttroxMachineScript::sIdleState;
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
			CCharacterTriggerScript trigger;
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
			CCharacterTriggerScript trigger;
			trigger.SetEvtType(TRIGGER_TYPE::ULT);
			notify(&trigger);
		}
	}
	
	
}
CAttroxMachineScript::~CAttroxMachineScript()
{
}
