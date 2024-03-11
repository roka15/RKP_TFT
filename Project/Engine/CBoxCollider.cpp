#include "pch.h"
#include "CBoxCollider.h"
#include "components.h"

void CBoxCollider::finaltick()
{
	assert(0 <= m_iCollisionCount);

	m_matCollider3D = XMMatrixScaling(m_v3OffsetScale.x, m_v3OffsetScale.y, m_v3OffsetScale.z);
	m_matCollider3D *= XMMatrixTranslation(m_v3OffsetPos.x, m_v3OffsetPos.y, m_v3OffsetPos.z);

	const Matrix& matWorld = Transform()->GetWorldMat();
	if (m_bAbsolute)
	{
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider3D = m_matCollider3D * matParentScaleInv * matWorld;
	}
	else
	{
		m_matCollider3D *= matWorld;
	}

	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	DrawDebugCube(m_matCollider3D, vColor, 0.f);
}

void CBoxCollider::SaveToLevelFile(FILE* _File)
{
	CCollider3D::SaveToLevelFile(_File);
	fwrite(&m_v3OffsetScale, sizeof(Vec3), 1, _File);
}

void CBoxCollider::LoadFromLevelFile(FILE* _File)
{
	CCollider3D::LoadFromLevelFile(_File);
	fread(&m_v3OffsetScale, sizeof(Vec3), 1, _File);
}

CBoxCollider::CBoxCollider()
	:
	CCollider3D(),
	m_v3OffsetScale{1.f,1.f,1.f}
{
	m_eShape = COLLIDER3D_TYPE::BOX;
}

CBoxCollider::~CBoxCollider()
{
}
