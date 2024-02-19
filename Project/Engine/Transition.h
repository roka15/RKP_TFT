#pragma once
#include "CEntity.h"
class CAniNode;
class CAnimatorController;

class CTransition :
    public CEntity
{
private:
    bool                    m_bHasExitTime;
    map<wstring, int>       m_mapIntConditions;
    map<wstring, float>     m_mapFloatConditions;
    map<wstring, bool>      m_mapBoolConditions;
    map<wstring, bool>      m_mapTriggerConditions;
    CAniNode*               m_pConnectNode;
    CAniNode*               m_pOwner;
public:
    bool IsActive();
    void RegisterCondition(wstring _Key, int _iValue);
    void RegisterCondition(wstring _Key, float _fValue);
    void RegisterCondition(wstring _Key, bool _bValue,bool _bTrigger);
    void SetOwner(CAniNode* _pOwnerNode) { m_pOwner = _pOwnerNode; }
    CAniNode* GetOwner() { return m_pOwner; }
    
    void RegisterCurNode(CAnimatorController* _pController);
    void SetConnectNode(CAniNode* _pConnectNode);
  
    CLONE(CTransition)
public:
    CTransition();
    virtual ~CTransition();
};

