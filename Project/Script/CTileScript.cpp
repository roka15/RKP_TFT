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
	if (CMouseMgr::GetInst()->GetCursor()->Collider3D() == _Other)
	{
		HighlightColor(true);
	}
}

void CTileScript::OnOverlap(CCollider* _Other)
{
}

void CTileScript::EndOverlap(CCollider* _Other)
{
	if (CMouseMgr::GetInst()->GetCursor()->Collider3D() == _Other)
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
		CCursor* cursor = (CCursor*)CMouseMgr::GetInst()->GetCursor()->GetScript<CCursor>();
		cursor->RegisterDragItem(child);
	}
}

void CTileScript::UpEvent(PointerEventData _data)
{
}

void CTileScript::AddItem(CGameObject* _obj)
{
	GetOwner()->AddChild(_obj);
}

void CTileScript::ChangeItemState(bool _flag)
{
	if (m_Type == TILE_TYPE::BATTLE)
	{
		CGameObject* owner = GetOwner();
		owner->Collider3D()->SetActive(_flag);
		const vector<CGameObject*>& childs = owner->GetChild();

		for (int i = 0; i < childs.size(); ++i)
		{
			CBaseCharacterScript* character = childs[i]->GetScript<CBaseCharacterScript>();
			if (character == nullptr)
				return;
			if (_flag)
			{
				character->SetWait(true);
			}
			else
			{
				character->SetWait(false);
			}
			
		}
	}
}

void CTileScript::HighlightColor(bool _flag)
{
	wstring Key;
	if (_flag)
	{
		Key = L"ActiveTileMtrl";
	}
	else
	{
		Key = L"InActiveTileMtrl";
	}
	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(Key), 0);
}
