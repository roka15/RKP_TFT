#pragma once
#include "CCollider3D.h"
class CSphereCollider :
	public CCollider3D
{
private:
	float                m_fRadius;
public:
	virtual void finaltick()override;
public:
	void SetRadius(float _fRadius) { m_fRadius = _fRadius; }
public:
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;
	CLONE(CSphereCollider);
public:
	CSphereCollider();
	virtual ~CSphereCollider();

};

