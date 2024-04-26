#pragma once
#include "CEntity.h"
class CAniNode;
class CAnimatorController;
class CAnimator3D;

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
    float                            m_fBlendTime;
    int                              m_iEditID;
    //Load때 사용되는 변수.
    wstring                          m_strConnectNode;
    wstring                          m_strOwnerNode;
private:
    template <typename T>
    bool ComparisonCalculator(T& data1, const T& data2 ,COMPARISON_TYPE _tType);
    vector<wstring> GetConditionNames();
    PARAM_TYPE GetDataType(wstring _Name);
    const int*const GetConditionValueInt(wstring _Name);
	const float*const GetConditionValueFloat(wstring _Name);
    const bool*const GetConditionValueBool(wstring _Name);
    const bool*const GetConditionValueTrigger(wstring _Name);
    void SetConditionValueInt(wstring _Name, const int& _iValue);
    void SetConditionValueFloat(wstring _Name, const float& _fValue);
    void SetConditionValueBool(wstring _Name, const bool& _bValue);
    void SetConditionValueTrigger(wstring _Name, const bool& _bValue);
    const COMPARISON_TYPE*const GetComparisonType(wstring _Name);
    void SetComparisonType(wstring _Name, int _iType);
    void RemoveCondition(wstring _Key);
public:
    int Save(FILE* _pFile);
    int Load(FILE* _pFile);
    bool IsActive(Ptr<CAnimatorController> _pController,CAnimator3D* _pAnimator);
    bool IsExitTime() { return m_bHasExitTime; }
    void SetExitTime(bool _flag) { m_bHasExitTime = _flag; }
    void RegisterCondition(wstring _Key, int _iValue,COMPARISON_TYPE _tComparison);
    void RegisterCondition(wstring _Key, float _fValue, COMPARISON_TYPE _tComparison);
    void RegisterCondition(wstring _Key, bool _bValue,bool _bTrigger, COMPARISON_TYPE _tComparison);
    void SetBlendTime(float _fTime) { m_fBlendTime = _fTime; }
    const float& GetBlendTime() { return m_fBlendTime; }
    void SetOwner(CAniNode* _pOwnerNode) { m_pOwner = _pOwnerNode; }
    CAniNode* GetOwner() { return m_pOwner; }
    void SetEditID(const int& _iId) { m_iEditID = _iId; }
    const int& GetEditID() { return m_iEditID; }
    void ChangeConditionName(PARAM_TYPE _eType, wstring _prev, wstring _next);

    bool RegisterCurNode(CAnimator3D* _pAnimator);
    void SetConnectNode(CAniNode* _pConnectNode);
    CAniNode* GetConnectNode() { return m_pConnectNode; }
    wstring GetConnectNodeName() { return m_strConnectNode; }
    wstring GetOwnerNodeName() { return m_strOwnerNode; }
    CLONE(CTransition)
private:
    friend class AniControllerInspector;
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
