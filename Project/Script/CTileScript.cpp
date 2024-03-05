#include "pch.h"
#include "CTileScript.h"
#include <Engine\CTimeMgr.h>


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
	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_Color);
}
