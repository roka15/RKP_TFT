#pragma once
#include "CStateMachineScript.h"
#include "CAttroxIdleScript.h"
class CAttroxMachineScript :
    public CStateMachineScript
{
private:
    static CAttroxIdleScript sIdleState;

    double m_dUltGauge;
    double m_dUltMaxCondition;
    double m_dUltMinCondition;
    bool   m_bUlt;
public:
    virtual void tick()override;

    CLONE(CAttroxMachineScript)
public:
    CAttroxMachineScript();
    virtual ~CAttroxMachineScript();
};

