#pragma once
#include "Ptr.h"
#include "CAniClip.h"

class CAniNode;

class CAnimation3D :
    public CEntity
{
private:
    Ptr<CAniClip>   m_pClip;

    CAniNode*       m_pOwner;
    double          m_dCurTime;
    double          m_dUpdateTime;
    int             m_iFrameIdx;
    int             m_iNextFrameIdx;
    float           m_fRatio;
    int             m_iFrameCount;
    bool            m_bFinalMatUpdate;
    bool            m_bFinish;
    vector<bool>    m_bEvents;
public:
    void finaltick();
    void Reset();
    void SetClip(const std::wstring& _strKey);
    Ptr<CAniClip> GetClip() { return m_pClip; }
    void SetOwner(CAniNode* _pAni) { m_pOwner = _pAni; }
    CAniNode* GetOwner() { return m_pOwner; }
    bool IsFinish() { return m_bFinish; }
    bool IsLoop() { return m_pClip->m_tInfo.bLoop; }
    CLONE(CAnimation3D);
public:
    CAnimation3D();
    CAnimation3D(Ptr<CAniClip> _clip);
    ~CAnimation3D();

    friend class CAnimator3D;
    friend class CAniNode;
};

