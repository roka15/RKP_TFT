#include "pch.h"
#include "CSphereCollider.h"
#include "components.h"
void CSphereCollider::finaltick()
{
	assert(0 <= m_iCollisionCount);

	float fScale = m_fRadius * 2;
	m_matCollider3D = XMMatrixScaling(fScale, fScale, fScale);
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

	DrawDebugSphere(m_matCollider3D, vColor, 0.f);
}

void CSphereCollider::SaveToLevelFile(FILE* _File)
{
	CCollider3D::SaveToLevelFile(_File);
	fwrite(&m_fRadius, sizeof(float), 1, _File);
}

void CSphereCollider::LoadFromLevelFile(FILE* _File)
{
	CCollider3D::LoadFromLevelFile(_File);
	fread(&m_fRadius, sizeof(float), 1, _File);
}

CSphereCollider::CSphereCollider()
	:
	CCollider3D(),
	m_fRadius(0.5f)
{
	m_eShape = COLLIDER3D_TYPE::SPHERE;
}

CSphereCollider::~CSphereCollider()
{
}
