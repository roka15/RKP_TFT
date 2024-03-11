#include "pch.h"
#include "CCollider3D.h"
#include "components.h"
#include "CScript.h"

void CCollider3D::finaltick()
{
}

void CCollider3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_v3OffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_eShape, sizeof(UINT), 1, _File);
}

void CCollider3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_v3OffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_eShape, sizeof(UINT), 1, _File);
}

CCollider3D::CCollider3D():CCollider(COMPONENT_TYPE::COLLIDER3D)
{
}

CCollider3D::~CCollider3D()
{
}
