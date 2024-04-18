#pragma once
#include "Ptr.h"
#include "CAniClip.h"

class CAnimator3D;

class CAnimation3D :
    public CEntity
{
private:
    Ptr<CAniClip>   m_pClip;

    CAnimator3D*    m_pOwner;
    double          m_dCurTime;
    double          m_dUpdateTime;
    int             m_iFrameIdx;
    int             m_iNextFrameIdx;
    float           m_fRatio;
    int             m_iFrameCount;
    bool            m_bFinalMatUpdate;
    bool            m_bFinish;
    vector<bool>    m_bEvents;
private:
    void check_bone(CStructuredBuffer*& _finalMat);
public:
    void finaltick();
    void UpdateData(CStructuredBuffer*& _bonMat,wstring _strNextClip);
    void Reset();
    void SetClip(const std::wstring& _strKey);
    Ptr<CAniClip> GetClip() { return m_pClip; }
    void SetOwner(CAnimator3D* _pAni) { m_pOwner = _pAni; }
    CAnimator3D* GetOwner() { return m_pOwner; }
    bool IsFinish() { return m_bFinish; }
    bool IsLoop() { return m_pClip->m_tInfo.bLoop; }
    int GetBoneCount() { return m_pClip->GetBoneCount(); }
    CLONE(CAnimation3D);
public:
    CAnimation3D();
    CAnimation3D(const CAnimation3D& _ref);
    CAnimation3D(Ptr<CAniClip> _clip);
    ~CAnimation3D();

    friend class CAnimator3D;
    friend class CAniNode;
};

