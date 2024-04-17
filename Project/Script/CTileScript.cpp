#include "pch.h"
#include "CTileScript.h"
#include "CTileMgr.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CMouseMgr.h>
#include "CBaseCharacterScript.h"
#include "CCursor.h"


CTileScript::CTileScript():CScript((UINT)SCRIPT_TYPE::TILESCRIPT),
m_Type(TILE_TYPE::END),
m_Number(-1),
m_Color{}
{
}
CTileScript::~CTileScript()
{
}
void CTileScript::tick()
{
}

void CTileScript::BeginOverlap(CCollider* _Other)
{
	if (CMouseMgr::GetInst()->GetCursor(CURSOR_TYPE::CURSOR_3D)->Collider3D() == _Other)
	{
		HighlightColor(true);
	}
}

void CTileScript::OnOverlap(CCollider* _Other)
{
}

void CTileScript::EndOverlap(CCollider* _Other)
{
	if (CMouseMgr::GetInst()->GetCursor(CURSOR_TYPE::CURSOR_3D)->Collider3D() == _Other)
	{
		HighlightColor(false);
	}
}

void CTileScript::DownEvent(PointerEventData _data)
{
	vector<CGameObject*> objs = GetOwner()->GetChild();
	if (objs.size() != 0)
	{
		CGameObject* child = objs[0];
		CCursor* cursor = (CCursor*)CMouseMgr::GetInst()->GetCursor(CURSOR_TYPE::CURSOR_3D)->GetScript<CCursor>();
		cursor->RegisterDragItem(child);
		//Tile Mgr 호출해서 Tile 외각선 표시 ON
		CTileMgr::GetInst()->TileRender(true);
	}
}

void CTileScript::UpEvent(PointerEventData _data)
{
	vector<CGameObject*> vecChilds = GetOwner()->GetChild();
	if (vecChilds.size() == 0)
		return;
	CBaseCharacterScript* pCharacterScript = vecChilds[0]->GetScript<CBaseCharacterScript>();
	switch (m_Type)
	{
	case TILE_TYPE::BATTLE:
		pCharacterScript->SetWait(false);
		break;
	case TILE_TYPE::WAIT:
		pCharacterScript->SetWait(true);
		break;
	}
	//Tile Mgr 호출해서 Tile 외각선 표시 OFF
	CTileMgr::GetInst()->TileRender(false);
}

void CTileScript::AddItem(CGameObject* _obj)
{
	GetOwner()->AddChild(_obj,true);
}



void CTileScript::ChangeTileState(bool _flag)
{
	if (m_Type == TILE_TYPE::BATTLE)
	{
		CGameObject* owner = GetOwner();
		owner->Collider3D()->SetActive(_flag);
	}
}

void CTileScript::HighlightColor(bool _flag)
{
	CMeshRender* pMeshRender = GetOwner()->MeshRender();
	if (pMeshRender == nullptr)
		return;
	wstring Key;
	if (_flag)
	{
		Key = L"ActiveTileMtrl";
	}
	else
	{
		Key = L"InActiveTileMtrl";
	}
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(Key), 0);
}
