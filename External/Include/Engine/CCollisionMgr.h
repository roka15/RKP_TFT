#pragma once
#include "CSingleton.h"

class CLayer;
class CCollider2D;
class CCollider3D;


union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_matrix[MAX_LAYER];
	map<UINT_PTR, bool>		m_mapColID;
	bool					m_ActiveView;
public:
	void LayerCheck(UINT _left, UINT _right);
	void LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}

	vector<CGameObject*> CursorCollisionTick(int _icursorType);
public:
	void tick();

private:
	void ColliderRenderActive(bool _bflag);
	bool Collision2D(CGameObject* _LeftObject, CGameObject* _RightObject);
	bool Collision3D(CGameObject* _LeftObject, CGameObject* _RightObject);
	void CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer);
	void CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject);
	bool CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool CollisionBtwCollider(CCollider3D* _pLeft, CCollider3D* _pRight);

	vector<CGameObject*> CollisionBtwLayerCursor(CLayer* _Layer, CGameObject* _Cursor);
};

