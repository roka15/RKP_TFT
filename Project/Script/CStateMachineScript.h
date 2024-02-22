#pragma once
#include <Engine\CScript.h>
class CCharacterScript;
class CState;
class CTrigger;
class CStateMachineScript :
    public CScript
{
protected:
    CState* m_pState;
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider2D* _Other)override {}
    virtual void OnOverlap(CCollider2D* _Other)override {}
    virtual void EndOverlap(CCollider2D* _Other)override {}
public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}
public:
    virtual void transition(CState* _pState);
    virtual void notify(CTrigger* _pTrigger);
    virtual CState* get_state(void)const;

    CLONE(CStateMachineScript)
public:
    CStateMachineScript();
    CStateMachineScript(SCRIPT_TYPE _eType);
    virtual ~CStateMachineScript();
};

