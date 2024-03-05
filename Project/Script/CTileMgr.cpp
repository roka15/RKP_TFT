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

	float WaitOffset = 50.f;
	for (int z = 0; z < m_Count.y; ++z)
	{
		v2Pos.y = (z * (m_Size.y + m_Offset.y));
		if (z == 0)
		{
			v2Pos.y -= WaitOffset;
		}
		else if(z == m_Count.y - 1)
			v2Pos.y += WaitOffset;
		for (int x = 0; x < m_Count.x; ++x)
		{
			if (z % 2 == 0 && z != 0 && z != m_Count.x - 1)
			{
				v2Pos.x = (x * (m_Size.x + m_Offset.x)) - (m_Offset.x/2.f);
			}
			else
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
			CMeshRender* meshRender = pNewObj->MeshRender();
			if (z == 0 || z == m_Count.x - 1)
			{
				eType = TILE_TYPE::WAIT;
				meshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"mesh\\square indicator.mesh"));
			}
			else
			{
				eType = TILE_TYPE::BATTLE;
				meshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"mesh\\indicator hexagon.mesh"));
			}
			Vec4 color = { 6/255.f,132/255.f,200/255.f,1.f };
			tile->SetTileInfo(eType, iIndex);
			tile->SetColor(color);

			//meshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAlphaMtrl"), 0);
			meshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\hp.mtrl"), 0);
			meshRender->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DShader"));
			pNewObj->Transform()->SetRelativeScale(Vec3(m_Size.x, 1.0f, m_Size.y));
			pNewObj->Transform()->SetRelativePos(Vec3(v2Pos.x, 0.0f, v2Pos.y));
			pNewObj->Transform()->SetRelativeRot(Vec3(-90 * XM_PI / 180, 0.0f, 0.0f));
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
