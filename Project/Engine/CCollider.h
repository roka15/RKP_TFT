#pragma once
#include "CComponent.h"
class CCollider :
    public CComponent
{
protected:
    int             m_iCollisionCount;
public:
    virtual void finaltick()override;
    void BeginOverlap(CCollider* _Other);
    void OnOverlap(CCollider* _Other);
    void EndOverlap(CCollider* _Other);

    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _File) override {};
public:

    CCollider(COMPONENT_TYPE _eType);
    virtual ~CCollider();
};

