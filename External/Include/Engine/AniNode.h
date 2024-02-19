#pragma once
#include "CEntity.h"
class CTransition;
class CAnimatorController;
class CAnimation3D;
class CStructuredBuffer;

class CAniNode :
    public CEntity
{
private:
    vector<CTransition*>        m_vecOutConditions;
    vector<CTransition*>        m_vecInConditions;
    CAnimatorController*        m_pController;
    CAnimation3D*               m_pMotionClip;
    bool                        m_bBlending;
private:
    void check_bone(CStructuredBuffer*& _finalMat);
public:
    void SetController(CAnimatorController* _pController) { m_pController = _pController; }
    
    void AddOutTransition(CTransition* _pTransition);
    void AddInTransition(CTransition* _pTransition);
    void RemoveOutTransition(CTransition* _pTransition);
    void RemoveInTransition(CTransition* _pTransition);
    void RemoveAllInTransition();
    void RemoveAllOutTransition();
    void Destory();
    void SetBlending(bool _flag) { m_bBlending = _flag; }
    bool NextNode(int _iOutSize,bool _bFinish,bool _bCurNullNode);
    UINT GetBoneCount();
public:
    void finaltick();
    void UpdateData(CStructuredBuffer*& _finalMat);
    CLONE(CAniNode)
public:
    CAniNode(wstring _strClipName);
    virtual ~CAniNode();
};

