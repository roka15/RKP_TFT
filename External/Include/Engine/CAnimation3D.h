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
    int             m_fRatio;
    int             m_iFrameCount;
    bool            m_bFinalMatUpdate;
    
    bool            m_bFinish;
public:
    void finaltick();
    void Reset();
    void SetClip(const std::wstring& _strKey);

    CLONE(CAnimation3D);
public:
    CAnimation3D();
    CAnimation3D(Ptr<CAniClip> _clip);
    ~CAnimation3D();

    friend class CAnimator3D;
};

