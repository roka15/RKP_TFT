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
    map<int, CTransition*>  m_mapIDTransition;
    wstring             m_LoadEditTxtPath;
    int                 m_iFlowLinkID;
private:
    vector<wstring> GetParamNames();
    PARAM_TYPE GetParamType(wstring _strName);
    void RegisterIDNode(const int& _iID, CAniNode* _pNode);
    void RegisterIDTransition(const int& _iId, CTransition* _pTransition);
    vector<CAniNode*> GetAllNode();
    const wstring& GetEditPath() { return m_LoadEditTxtPath; }
    void EditInfoClear();
public:
    virtual int Load(const wstring& _strFilePath)override;
    virtual int Save(const wstring& _strFilePath)override;
public:
    void Init();
    CAniNode* CreateNode(wstring _strName, wstring _strClipName,int _iId);
    void SetNodeInfo(wstring _strName, wstring _strClipName);
    void DestroyNode(wstring _strName);
    void DestroyNode(const int& _iID);
    void CreateTransition(const int& _startID,const int& _endID, const int& _linkID);
    CTransition* CreateTransition(wstring _strName,CAniNode* _pInNode, CAniNode* _pOutNode,bool _bExitTime=false);
    void DestroyTransition(wstring _strName);
    void DestroyTransition(const int& _iID);
    void RegisterParam(wstring _strName, int _iValue);
    void RegisterParam(wstring _strName, float _fValue);
    void RegisterParam(wstring _strName, bool _bValue , bool _bTrigger);
    
    CAniNode* GetNode(const wstring _strName);
    CAniNode* GetNode(const int& _iID);
    CTransition* GetTransition(const int& _iLinkID);
    CTransition* GetTransition(const int& _startNodeId, const int& _endNodeId);
    void ChangeNode(const wstring _strName, int _iID);
    bool IsChangeNodeName(const wstring _strName, int _iID);
    ANI_NODE_RETURN NextNode(bool _bFinish,bool _bLoop,wstring _strCurName, CAnimator3D* _pAnimator);
    void CopyParams(CAnimator3D* _pAnimator);
   
    void SetFlowLinkID(const int& _iID) { m_iFlowLinkID = _iID; }
    const int& GetFlowLinkID() { return m_iFlowLinkID; }
private:
    friend class AniControllerInspector;
    friend class AniControllerEditUI;
public:
    CAnimatorController();
    virtual ~CAnimatorController();
};

