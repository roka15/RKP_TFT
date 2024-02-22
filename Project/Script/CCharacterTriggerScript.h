#pragma once
#include "CTriggerScript.h"

enum class TRIGGER_TYPE
{
    NORMAL,
    ULT,
    BATTLE,
    DEBUFF,
    BUFF,
    END,
};

class CCharacterTriggerScript :
    public CTriggerScript
{
private:
    TRIGGER_TYPE    m_eType;
public:
    void SetEvtType(TRIGGER_TYPE _eType) { m_eType = _eType; }
    const TRIGGER_TYPE& GetEvtType() { return m_eType; }
    virtual void notify(CStateMachineScript* _pSMachine, CStateScript* _pState)override;
	CLONE(CCharacterTriggerScript)
public:
    CCharacterTriggerScript();
    virtual ~CCharacterTriggerScript();
};

class GlobalTriggerMgr : CSingleton<GlobalTriggerMgr>
{
    SINGLE(GlobalTriggerMgr)
public:
    static vector<CCharacterTriggerScript*> s_vecTriggers;
public:
    static void Init()
    {
        for (int i = (UINT)TRIGGER_TYPE::NORMAL; i < (UINT)TRIGGER_TYPE::END; ++i)
        {
            CCharacterTriggerScript* trigger = new CCharacterTriggerScript();
            trigger->SetEvtType((TRIGGER_TYPE)i);
            s_vecTriggers.push_back(trigger);
        }
    }

    static void Release()
    {
        for (int i = (UINT)TRIGGER_TYPE::NORMAL; i < (UINT)TRIGGER_TYPE::END; ++i)
        {
            CCharacterTriggerScript* trigger = s_vecTriggers[i];
            delete trigger;
        }
        s_vecTriggers.clear();
    }
};
