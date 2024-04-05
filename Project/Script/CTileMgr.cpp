#include "pch.h"
#include "CTileMgr.h"
#include <Engine\CGameObject.h>
#include <Engine\CResMgr.h>
#include "CTileScript.h"
#include "CGameMgr.h"
#include "CItem.h"

int  CTileMgr::s_number = 0;
CTileMgr::CTileMgr()
{

}
CTileMgr::~CTileMgr()
{

}
void CTileMgr::init()
{
	BattleSetInfo(Vec2{ 304,245 }, Vec2{ 1.15f,1.15f }, Vec2{ 7,4 }, Vec3{ 525,-70,-106 });
	WaitSetInfo(Vec2{ 252.0f,82.5f }, Vec2{ 1.5f,1.5f }, Vec2{ 9,1 }, Vec3{ -240.f,100.0f,0.0f });
}
void CTileMgr::tick()
{
	int iGameState = CGameMgr::GetInst()->GetState();
	if (iGameState == 1)//battle
	{
		for (int i = 0; i < m_vecBattleTile.size(); ++i)
		{
			m_vecBattleTile[i]->GetScript<CTileScript>()->HighlightColor(false);
		}
	}
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

	if (_type == TILE_OWNER_TYPE::ENEMY)
	{
		for (int z = zMax - 1; z >= 0; --z)
		{
			v2Pos.z = (z * (m_Size.y + m_Offset.y));
			int iXLoop = m_Count.x;

			if (z == 0)
			{
				iXLoop = m_WaitCount.x;
				v2Pos.z -= WaitOffset;
			}

			for (int x = iXLoop - 1; x >= 0; --x)
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

				tile->SetTileInfo(eType, _type, s_number);

				meshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"InActiveTileMtrl"), 0);

				pNewObj->Transform()->SetRelativeScale(Vec3(m_Size.x, 1.0f, m_Size.y));
				pNewObj->Transform()->SetRelativePos(Vec3(v2Pos.x, v2Pos.y, v2Pos.z));
				pNewObj->Transform()->SetRelativeRot(Vec3(-90 * XM_PI / 180, 0.0f, 0.0f));
				pEmpty->AddChild(pNewObj);
				m_vecTile.push_back(pNewObj);
				if (eType == TILE_TYPE::BATTLE)
				{
					m_vecBattleTile.push_back(pNewObj);
				}
				++s_number;
			}
		}
	}
	else
	{
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

				tile->SetTileInfo(eType, _type, s_number);

				meshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"InActiveTileMtrl"), 0);

				pNewObj->Transform()->SetRelativeScale(Vec3(m_Size.x, 1.0f, m_Size.y));
				pNewObj->Transform()->SetRelativePos(Vec3(v2Pos.x, v2Pos.y, v2Pos.z));
				pNewObj->Transform()->SetRelativeRot(Vec3(-90 * XM_PI / 180, 0.0f, 0.0f));
				pEmpty->AddChild(pNewObj);
				m_vecTile.push_back(pNewObj);
				if (eType == TILE_TYPE::BATTLE)
				{
					m_vecBattleTile.push_back(pNewObj);
				}
				++s_number;
			}
		}
	}
	pEmpty->Transform()->SetRelativePos(m_StartPos);
	return pEmpty;
}

vector<int> CTileMgr::SearchEnemyTile(CGameObject* _ownerPlayer)
{
	vector<int> vecEnemyNums;
	for (int i = 0; i < m_vecBattleTile.size(); ++i)
	{
		vector<CGameObject*> vecChilds = m_vecBattleTile[i]->GetChild();
		if (vecChilds.size() == 0)
			continue;
		CGameObject* pItemObj = vecChilds[0];
	
		CItem* pItemScript = pItemObj->GetScript<CItem>();
		CGameObject* pPlayer = pItemScript->GetPlayer();
		bool bSame = _ownerPlayer == pPlayer;//CGameMgr::GetInst()->IsSamePlayer(pPlayer);
		if (bSame == false)
		{
			CTileScript* script = m_vecBattleTile[i]->GetScript<CTileScript>();
			if (script == nullptr)
				continue;
			int Number = script->GetNumber();
			vecEnemyNums.push_back(Number);
		}
	}
	return vecEnemyNums;
}


Vec3 CTileMgr::GetBattleTileWorldPos(int _itile)
{
	return  m_vecBattleTile[_itile]->Transform()->GetWorldPos();
}

const TILE_OWNER_TYPE& CTileMgr::GetTileOwnerType(int _iTileNum)
{
	return GetTile(_iTileNum)->GetOwnerType();
}

CTileScript* CTileMgr::GetTile(int _iid)
{
	return m_vecTile[_iid]->GetScript<CTileScript>();
}

void CTileMgr::RegisterItem(int _iTileNum, CGameObject* _pObj)
{
	CTileScript* script = GetTile(_iTileNum);
	script->AddItem(_pObj);
}

CGameObject* CTileMgr::GetItem(int _tileNum)
{
	vector<CGameObject*> vecObj = m_vecTile[_tileNum]->GetChild();
	if (vecObj.size() == 0)
		return nullptr;
	
	return vecObj[0];
}

bool CTileMgr::EmptyBattleTile(int _iid)
{
	return m_vecBattleTile[_iid]->GetChild().size() == 0;
}

void CTileMgr::EnableSelectBattleTile(bool _flag)
{
	for (int i = 0; i < m_vecTile.size(); ++i)
	{
		CTileScript* tile = m_vecTile[i]->GetScript<CTileScript>();
		if (tile == nullptr)
			continue;
		TILE_TYPE type = tile->GetType();
		if (type == TILE_TYPE::BATTLE)
			m_vecTile[i]->GetScript<CTileScript>()->ChangeTileState(_flag);
	}
}

vector<int> CTileMgr::GetAnotherPlayer(TILE_OWNER_TYPE _eType)
{
	int size = m_vecBattleTile.size();
	vector<int> vecEnemyNums = {};
	int Start = 0;
	int Loop = 0;
	switch (_eType)
	{
	case TILE_OWNER_TYPE::PLAYER:
		Start = 0;
		Loop = (size / 2);
		break;
	case TILE_OWNER_TYPE::ENEMY:
		Start = (size / 2);
		Loop = size;
		break;
	}

	for (int i = Start; i < Loop; ++i)
	{
		vector<CGameObject*> vecChild = m_vecBattleTile[i]->GetChild();
		if (vecChild.size() == 0)
			continue;
		else
		{
			CTileScript* script = m_vecBattleTile[i]->GetScript<CTileScript>();
			if (script == nullptr)
				continue;

			TILE_TYPE eType = script->GetType();
			if (eType != TILE_TYPE::BATTLE)
				continue;
			int Number = script->GetNumber();
			vecEnemyNums.push_back(Number);
		}
	}
	return vecEnemyNums;
}

void CTileMgr::BattleRouteRender(vector<int> _vecRoute)
{
	int Offset = m_WaitCount.x;
	for (int i = 0; i < _vecRoute.size(); ++i)
	{
		int Idx = _vecRoute[i] +Offset;
		GetTile(Idx)->HighlightColor(true);
	}
	
}

void CTileMgr::BattleRouteRender(int _iNextNode)
{
	int Offset = m_WaitCount.x;
	int Idx = _iNextNode + Offset;
	GetTile(Idx)->HighlightColor(true);
}

int CTileMgr::FindEmptyWaitTile()
{
	for (int i = 0; i < m_WaitCount.x; ++i)
	{
		int size = m_vecTile[i]->GetChild().size();
		if (size == 0)
			return i;
	}
	return -1;
}


