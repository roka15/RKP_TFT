#pragma once
#include "CEntity.h"
class CTransition;
class CAnimatorController;
class CAnimation3D;

class CAniNode :
    public CEntity
{
private:
    vector<CTransition*>    m_vecConditions;
    CAnimatorController*    m_pController;
    CAnimation3D*           m_pMotionClip;
public:
    void finaltick();
    void UpdateData(CStructuredBuffer*& _finalMat);
public:
    CAniNode();
    virtual ~CAniNode();
};

