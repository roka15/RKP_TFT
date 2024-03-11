#pragma once
#include "CCollider3D.h"
class CBoxCollider :
    public CCollider3D
{
private:
    Vec3                m_v3OffsetScale;
public:
    virtual void finaltick()override;
public:
    void SetOffsetScale(Vec3 _vOffsetScale) { m_v3OffsetScale = _vOffsetScale; }
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CBoxCollider);
public:
    CBoxCollider();
    virtual ~CBoxCollider();
};

