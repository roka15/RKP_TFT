#pragma once
#include "CEntity.h"
class CAniNode;
class CAnimatorController;

class CTransition :
    public CEntity
{
private:
    bool                             m_bHasExitTime;
    map<wstring, int>                m_mapIntConditions;
    map<wstring, float>              m_mapFloatConditions;
    map<wstring, bool>               m_mapBoolConditions;
    map<wstring, bool>               m_mapTriggerConditions;
    map<wstring, COMPARISON_TYPE>    m_mapComparisonConditions;
    CAniNode*                        m_pConnectNode;
    CAniNode*                        m_pOwner;
    //Load때 사용되는 변수.
    wstring                          m_strConnectNode;
    wstring                          m_strOwnerNode;
private:
    template <typename T>
    bool ComparisonCalculator(T& data1, const T& data2 ,COMPARISON_TYPE _tType);
public:
    int Save(FILE* _pFile);
    int Load(FILE* _pFile);
    bool IsActive(Ptr<CAnimatorController> _pController);
    bool IsExitTime() { return m_bHasExitTime; }
    void SetExitTime(bool _flag) { m_bHasExitTime = _flag; }
    void RegisterCondition(wstring _Key, int _iValue,COMPARISON_TYPE _tComparison);
    void RegisterCondition(wstring _Key, float _fValue, COMPARISON_TYPE _tComparison);
    void RegisterCondition(wstring _Key, bool _bValue,bool _bTrigger, COMPARISON_TYPE _tComparison);
    void SetOwner(CAniNode* _pOwnerNode) { m_pOwner = _pOwnerNode; }
    CAniNode* GetOwner() { return m_pOwner; }
    
    bool RegisterCurNode(Ptr<CAnimatorController> _pController);
    void SetConnectNode(CAniNode* _pConnectNode);
    wstring GetConnectNodeName() { return m_strConnectNode; }
    wstring GetOwnerNodeName() { return m_strOwnerNode; }
    CLONE(CTransition)
public:
    CTransition();
    virtual ~CTransition();
};

template<typename T>
inline bool CTransition::ComparisonCalculator(T& data1, const T& data2, COMPARISON_TYPE _tType)
{
    switch (_tType)
    {
    case COMPARISON_TYPE::EQUAL:
        return data1 == data2;
    case COMPARISON_TYPE::GREATER:
        return data1 < data2;
    case COMPARISON_TYPE::LESS:
        return data1 > data2;
    }
    return false;
}
