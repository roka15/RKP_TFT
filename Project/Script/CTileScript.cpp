#include "pch.h"
#include "CTileScript.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CMouseMgr.h>

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
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ActiveTileMtrl"), 0);
	}
}

void CTileScript::OnOverlap(CCollider* _Other)
{
}

void CTileScript::EndOverlap(CCollider* _Other)
{
	if (CMouseMgr::GetInst()->GetCursor()->Collider3D() == _Other)
	{
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"InActiveTileMtrl"), 0);
	}
}
