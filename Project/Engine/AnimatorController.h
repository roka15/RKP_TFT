#pragma once
#include "CRes.h"

class CStructuredBuffer;
class CAniNode;
class CTransition;
class CAnimatorController :
    public CRes
{
private:
    map<wstring, int>   m_mapIntParams;
    map<wstring, float> m_mapFloatParams;
    map<wstring, bool>  m_mapBoolParams;
    map<wstring, bool>  m_mapTriggerParams;
    CAniNode*           m_pEntryNode;
    CAniNode*           m_pExitNode;
    CAniNode*           m_pAnyStateNode;
    CAniNode*           m_pCurNode;
    map<wstring, CAniNode*> m_mapNode;
public:
    virtual int Load(const wstring& _strFilePath)override;
    virtual int Save(const wstring& _strFilePath)override;
public:
    void Init();
    void finaltick();
    void UpdateData(CStructuredBuffer*& _finalMat);
    CAniNode* CreateNode(wstring _strName, wstring _strClipName);
    void DestroyNode(wstring _strName);
    CTransition* CreateTransition(wstring _strName,CAniNode* _pInNode, CAniNode* _pOutNode,bool _bExitTime=false);
    void DestroyTransition(wstring _strName);
    void RegisterParam(wstring _strName, int _iValue);
    void RegisterParam(wstring _strName, float _fValue);
    void RegisterParam(wstring _strName, bool _bValue , bool _bTrigger);
    void DeleteParam(PARAM_TYPE _eType, wstring _strName);
    int GetIntParam(wstring _strName, bool& _bfail);
    float GetFloatParam(wstring _strName, bool& _bfail);
    bool GetBoolParam(wstring _strName,bool& _bfail);
    bool GetTriggerParam(wstring _strName, bool& _bfail);
    bool SetIntParam(wstring _strName,int _iValue);
    bool SetFloatParam(wstring _strName,float _fValue);
    bool SetTriggerParam(wstring _strName,bool _bValue);
    bool SetBoolParam(wstring _strName, bool _bValue);

    void SetCurNode(CAniNode* _pCurNode) { m_pCurNode = _pCurNode; }
    CAniNode* GetNode(const wstring _strName) { return m_mapNode[_strName]; }
    UINT GetBoneCount();
public:
    CAnimatorController();
    virtual ~CAnimatorController();
};

