#pragma once
#include "CRes.h"

class CStructuredBuffer;
class CAniNode;

class CAnimatorController :
    public CRes
{
private:
    bool                m_bStart;
    map<wstring, int>   m_mapIntParams;
    map<wstring, float> m_mapFloatParams;
    map<wstring, bool>  m_mapBoolParams;
    map<wstring, bool>  m_mapTriggerParams;
    CAniNode*           m_pCurNode;
    vector<CAniNode*>   m_vecNode;
public:
    virtual int Load(const wstring& _strFilePath)override;
    virtual int Save(const wstring& _strFilePath)override;
public:
    void StartPlayBack() { m_bStart = true; }
    void StopPlayBack() { m_bStart = false; }
    void finaltick();
    void UpdateData(CStructuredBuffer*& _finalMat);
    void CreateNode(wstring _strName, wstring _strClipName);
    void DestroyNode(wstring _strName);
    void CreateTransition(wstring _strName,CAniNode* _pConnectNode);
    void DestroyTransition(wstring _strName);
    void RegisterParam(wstring _strName, int _iValue);
    void RegisterParam(wstring _strName, float _fValue);
    void RegisterParam(wstring _strName, bool _bValue , bool _bTrigger);
    void DeleteParam(PARAM_TYPE _eType, wstring _strName);
public:
    CAnimatorController();
    virtual ~CAnimatorController();
};

