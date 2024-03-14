#include "pch.h"
#include "CTileMgr.h"
#include <Engine\CGameObject.h>
#include <Engine\CResMgr.h>
#include "CTileScript.h"


int  CTileMgr::s_number = 0;
CTileMgr::CTileMgr()
{

}
CTileMgr::~CTileMgr()
{

}

void CTileMgr::BattleSetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start)
{
	m_Offset = _offset;
	m_Size = _size;
	m_Count = _count;
	m_StartPos = _start;
}

void CTileMgr::WaitSetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start)
{
	m_WaitOffset = _offset;
	m_WaitSize = _size;
	m_WaitCount = _count;
	m_WaitStartPos = _start;
}


CGameObject* CTileMgr::CreateTile(TILE_OWNER_TYPE _type)
{
	Vec3 v2Pos = {};
	CGameObject* pEmpty = new CGameObject;
	pEmpty->SetName(L"TileMgr");
	pEmpty->AddComponent(new CTransform);

	float WaitOffset = m_WaitOffset.y;

	int zMax = m_Count.y + m_WaitCount.y;

	for (int z = 0; z < zMax; ++z)
	{
		v2Pos.z = (z * (m_Size.y + m_Offset.y));
		int iXLoop = m_Count.x;
		if (z == 0)
		{
			iXLoop = m_WaitCount.x;
			v2Pos.z -= WaitOffset;
		}

		for (int x = 0; x < iXLoop; ++x)
		{
			if (z % 2 == 0 && z != 0)
			{
				v2Pos.x = (x * (m_Size.x + m_Offset.x)) - (m_Offset.x / 2.f);
				v2Pos.y = 0.0f;
			}
			else if (z == 0)
			{
				v2Pos.x = (x * (m_Size.x + m_WaitOffset.x)) + m_WaitStartPos.x;
				v2Pos.y = m_WaitStartPos.y;
			}
			else
			{
				v2Pos.x = (x * (m_Size.x + m_Offset.x));
				v2Pos.y = 0.0f;
			}
			CGameObject* pNewObj = new CGameObject;
			wstring Name = L"Tile" + to_wstring(s_number);
			pNewObj->SetName(Name);
			pNewObj->AddComponent(new CTransform);
			pNewObj->AddComponent(new CMeshRender);
			pNewObj->AddComponent(new CBoxCollider);
			CBoxCollider* collider = (CBoxCollider*)pNewObj->GetComponent(COMPONENT_TYPE::COLLIDER3D);
			collider->SetTrigger(true);
			collider->SetOffsetScale(Vec3{ 100.f,100.f,100.f });

			CTileScript* tile = (CTileScript*)CScriptMgr::GetScript(SCRIPT_TYPE::TILESCRIPT);
			pNewObj->AddComponent(tile);

			TILE_TYPE eType = TILE_TYPE::END;
			CMeshRender* meshRender = pNewObj->MeshRender();
			if (z == 0)
			{
				eType = TILE_TYPE::WAIT;
				meshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"mesh\\square indicator.mesh"));
			}
			else
			{
				eType = TILE_TYPE::BATTLE;
				meshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"mesh\\indicator hexagon.mesh"));
			}
			
			tile->SetTileInfo(eType,_type, s_number);

			meshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"InActiveTileMtrl"), 0);
			
			pNewObj->Transform()->SetRelativeScale(Vec3(m_Size.x, 1.0f, m_Size.y));
			pNewObj->Transform()->SetRelativePos(Vec3(v2Pos.x, v2Pos.y, v2Pos.z));
			pNewObj->Transform()->SetRelativeRot(Vec3(-90 * XM_PI / 180, 0.0f, 0.0f));
			pEmpty->AddChild(pNewObj);
			m_vecTile.push_back(pNewObj);

			++s_number;
		}
	}
	pEmpty->Transform()->SetRelativePos(m_StartPos);
	return pEmpty;
}

vector<int> CTileMgr::Find(int _istart, int _iend)
{
	return vector<int>();
}


Vec2 CTileMgr::GetTilePos(int _itile)
{
	return Vec2();
}
