#pragma once
#include <Engine\CScript.h>
class CCharacterScript;
class CStateScript;
class CTriggerScript;
class CStateMachineScript :
    public CScript
{
protected:
    CStateScript* m_pState;
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider2D* _Other)override {}
    virtual void OnOverlap(CCollider2D* _Other)override {}
    virtual void EndOverlap(CCollider2D* _Other)override {}
public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}
public:
    virtual void transition(CStateScript* _pState);
    virtual void notify(CTriggerScript* _pTrigger);
    virtual CStateScript* get_state(void)const;

    CLONE(CStateMachineScript)
public:
    CStateMachineScript();
    CStateMachineScript(SCRIPT_TYPE _eType);
    virtual ~CStateMachineScript();
};

