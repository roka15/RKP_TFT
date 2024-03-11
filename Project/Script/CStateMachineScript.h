#pragma once
#include <Engine\CScript.h>
class CCharacterScript;
class CState;
class CTrigger;

class CStateMachineScript :
    public CScript
{
protected:
    vector<CState*> m_pStateList;
    CState* m_pState;
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider* _Other)override {}
    virtual void OnOverlap(CCollider* _Other)override {}
    virtual void EndOverlap(CCollider* _Other)override {}
public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}
public:
    virtual void transition(UINT _iType) {};
    virtual void notify(CTrigger* _pTrigger);
    virtual CState* get_state(void)const;
    virtual void add_state(CState* _pState) { m_pStateList.push_back(_pState); };
protected:
    virtual void transition(CState* _pState);
public:
    CLONE(CStateMachineScript)
    CStateMachineScript();
    CStateMachineScript(SCRIPT_TYPE _eType);
    virtual ~CStateMachineScript();
};

