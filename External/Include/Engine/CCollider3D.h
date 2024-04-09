#pragma once
#include "CCollider.h"
class CCollider3D :
    public CCollider
{
protected:
    Vec3            m_v3OffsetPos;
    bool            m_bAbsolute;
    COLLIDER3D_TYPE m_eShape;

    Matrix          m_matCollider3D;
    bool            m_bTrigger;
    bool            m_bView;
public:
    virtual void finaltick()override;
public:
    void SetOffsetPos(Vec3 _vOffsetPos) { m_v3OffsetPos = _vOffsetPos; }
    void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }
    void SetTrigger(bool _bFlag) { m_bTrigger = _bFlag; }
    bool GetTrigger() { return m_bTrigger; }
    void ActiveView(bool _bFlag) { m_bView = _bFlag; }
    const Matrix& GetColliderWorldMat() { return m_matCollider3D; }
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CCollider3D);
public:
    CCollider3D();
    virtual ~CCollider3D();
};

