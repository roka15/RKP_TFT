#pragma once
#include "CTrigger.h"

enum class TRIGGER_TYPE
{
    NORMAL,
    ULT,
    BATTLE,
    DEBUFF,
    BUFF,
    END,
};
class CState;
class CCharacterTrigger : public CTrigger
{
private:
    TRIGGER_TYPE    m_eType;
public:
    void SetEvtType(TRIGGER_TYPE _eType) { m_eType = _eType; }
    const TRIGGER_TYPE& GetEvtType() { return m_eType; }
    virtual void notify(CStateMachineScript* _pSMachine, CState* _pState)override;
    CLONE(CCharacterTrigger)
public:
    CCharacterTrigger();
    virtual ~CCharacterTrigger();
};
