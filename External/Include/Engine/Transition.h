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
public:
    bool IsActive(CAnimatorController* _pController);
    void RegisterCondition(wstring _Key, int _iValue);
    void RegisterCondition(wstring _Key, float _fValue);
    void RegisterCondition(wstring _Key, bool _bValue,bool _bTrigger);
public:
    CTransition();
    virtual ~CTransition();
};

