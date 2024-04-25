#pragma once
#include "CEntity.h"
class CTransition;
class CAnimatorController;
class CStructuredBuffer;

class CAniNode :
    public CEntity
{
private:
    vector<CTransition*>        m_vecOutConditions;
    vector<CTransition*>        m_vecInConditions;
    CAnimatorController*        m_pController;
    wstring                     m_strAniKey;
    bool                        m_bBlending;
    int                         m_EditorNodeID;
    Vec2                        m_EditorNodePos;

private:
    ANI_NODE_RETURN NextNode(int _iOutSize, bool _bFinish, bool _bCurNullNode, bool _bLoop, CAnimator3D* _pAnimator);
    vector<CTransition*> GetOutTransitionAll() { return m_vecOutConditions; }
public:
    int Save(FILE* _pFile);
    int Load(FILE* _pFile);
    int LoadAfterProcess();
    CAnimatorController* GetController() { return m_pController; }
    void SetController(CAnimatorController* _pController) { m_pController = _pController; }
    wstring GetAnimationKey() { return m_strAniKey; }
    void AddOutTransition(CTransition* _pTransition);
    void AddInTransition(CTransition* _pTransition);
    void RemoveOutTransition(CTransition* _pTransition);
    void RemoveInTransition(CTransition* _pTransition);
    void RemoveAllInTransition();
    void RemoveAllOutTransition();
    void Destory();
    void SetBlending(bool _flag) { m_bBlending = _flag; }
    void SetAniKey(wstring _AniKey) { m_strAniKey = _AniKey; }
    void SetEditorNodeID(const int& _ID) { m_EditorNodeID = _ID; }
    void SetEditorNodePosition(Vec2 _position) { m_EditorNodePos = _position; }
    
    ANI_NODE_RETURN NextNode(bool _bFinish ,bool _bLoop, CAnimator3D* _pAnimator);
    const int& GetEditorNodeID() { return m_EditorNodeID; }
    const Vec2& GetEditorNodePosition()const { return m_EditorNodePos; }
    CTransition* GetTransition(CAniNode* _pNode);
public:
    CLONE(CAniNode)
public:
    CAniNode();
    CAniNode(wstring _strClipName);
    virtual ~CAniNode();
private:
    friend class AniControllerEditUI;
};

