#pragma once
#include "CComponent.h"
class CRayCast :
	public CComponent
{
private:

public:
	virtual void begin()override {}
	virtual void tick()override {}
	virtual void finaltick()override;
public:
	virtual void SaveToLevelFile(FILE* _File)override;
	virtual void LoadFromLevelFile(FILE* _FILE) override;

	void RayCastCheck(Vec3 _pos, Vec3 _dirdistance, Vec3 _color, float _time);
public:
	CLONE(CRayCast)
public:
	CRayCast();
	virtual ~CRayCast();
};

