#include "pch.h"
#include "CTileMgr.h"
#include <Engine\CGameObject.h>
#include <Engine\CResMgr.h>
#include "CTileScript.h"

CTileMgr::CTileMgr()
{

}
CTileMgr::~CTileMgr()
{

}
void CTileMgr::SetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start)
{
	m_Offset = _offset;
	m_Size = _size;
	m_Count = _count;
	m_StartPos = _start;
}

void CTileMgr::CreateTile()
{
	int iIndex = 0;
	Vec2 v2Pos = {};
	CGameObject* pEmpty = new CGameObject;
	pEmpty->SetName(L"TileMgr");
	pEmpty->AddComponent(new CTransform);

	for (int z = 0; z < m_Count.y; ++z)
	{
		v2Pos.y = (z * (m_Size.y + m_Offset.y));
		for (int x = 0; x < m_Count.x; ++x)
		{
			v2Pos.x = (x * (m_Size.x +m_Offset.x));
			CGameObject* pNewObj = new CGameObject;
			wstring Name = L"Tile" + to_wstring(iIndex);
			pNewObj->SetName(Name);
			pNewObj->AddComponent(new CTransform);
			pNewObj->AddComponent(new CMeshRender);
			pNewObj->AddComponent(new CCollider2D);
			CCollider2D* collider = (CCollider2D*)pNewObj->GetComponent(COMPONENT_TYPE::COLLIDER2D);
		
			CTileScript* tile = (CTileScript*)CScriptMgr::GetScript(SCRIPT_TYPE::TILESCRIPT);
			pNewObj->AddComponent(tile);

			TILE_TYPE eType = TILE_TYPE::END;
			if (x == 0 || x == m_Count.x - 1)
				eType = TILE_TYPE::WAIT;
			else
				eType = TILE_TYPE::BATTLE;
			tile->SetTileInfo(eType, iIndex);
			pNewObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
			pNewObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
			Vec4 color = { 0.f,1.f,0.f,1.f };
			pNewObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &color);
			
			pNewObj->Transform()->SetRelativeScale(Vec3(m_Size.x, 1.0f, m_Size.y));
			pNewObj->Transform()->SetRelativePos(Vec3(v2Pos.x, 0.0f, v2Pos.y));
			pEmpty->AddChild(pNewObj);
			m_vecTile.push_back(pNewObj);
			++iIndex;
		}
	}
	SpawnGameObject(pEmpty, m_StartPos, 1);
}

vector<int> CTileMgr::Find(int _istart, int _iend)
{
	return vector<int>();
}

Vec2 CTileMgr::GetTilePos(int _itile)
{
	return Vec2();
}
