#pragma once
#include "CRes.h"

class CStructuredBuffer;
class CAniNode;
class CTransition;
class CAnimator3D;
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
    map<wstring, CAniNode*> m_mapNode;
    map<int, CAniNode*>     m_mapIDNode;
public:
    virtual int Load(const wstring& _strFilePath)override;
    virtual int Save(const wstring& _strFilePath)override;
public:
    void Init();
    CAniNode* CreateNode(wstring _strName, wstring _strClipName,int _iId);
    void SetNodeInfo(wstring _strName, wstring _strClipName);
    void DestroyNode(wstring _strName);
    void DestroyNode(const int& _iID);
    CTransition* CreateTransition(wstring _strName,CAniNode* _pInNode, CAniNode* _pOutNode,bool _bExitTime=false);
    void DestroyTransition(wstring _strName);
    void RegisterParam(wstring _strName, int _iValue);
    void RegisterParam(wstring _strName, float _fValue);
    void RegisterParam(wstring _strName, bool _bValue , bool _bTrigger);
    
    CAniNode* GetNode(const wstring _strName);
    CAniNode* GetNode(const int& _iID);
    void ChangeNode(const wstring _strName, int _iID);
    bool IsChangeNodeName(const wstring _strName, int _iID);
    ANI_NODE_RETURN NextNode(bool _bFinish,bool _bLoop,wstring _strCurName, CAnimator3D* _pAnimator);
    void CopyParams(CAnimator3D* _pAnimator);
public:
    CAnimatorController();
    virtual ~CAnimatorController();
};

