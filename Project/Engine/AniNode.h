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
private:
    ANI_NODE_RETURN NextNode(int _iOutSize, bool _bFinish, bool _bCurNullNode, bool _bLoop, CAnimator3D* _pAnimator);
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
    ANI_NODE_RETURN NextNode(bool _bFinish ,bool _bLoop, CAnimator3D* _pAnimator);

public:
    CLONE(CAniNode)
public:
    CAniNode();
    CAniNode(wstring _strClipName);
    virtual ~CAniNode();
};

