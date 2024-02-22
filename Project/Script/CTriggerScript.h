#pragma once
#include <Engine\CScript.h>
class CStateMachineScript;
class CStateScript;
class CTriggerScript :
    public CScript
{
private:
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider2D* _Other)override {}
    virtual void OnOverlap(CCollider2D* _Other)override {}
    virtual void EndOverlap(CCollider2D* _Other)override {}
public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}
public:
    virtual void notify(CStateMachineScript* _pSMachine, CStateScript* _pState);

    CLONE(CTriggerScript)
public:
    CTriggerScript();
    CTriggerScript(SCRIPT_TYPE _eType);
    virtual ~CTriggerScript();
};

