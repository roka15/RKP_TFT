#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)
	, m_vRelativeDir{
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f) }
	, m_bLerpFlag(false)
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}

void CTransform::RequestLerpRot(Vec3 _Rot)
{
	if (_Rot == m_vRelativeRot)
		return;
	m_v3LerpGoalRot = _Rot;
	m_bLerpFlag = true;
	m_fLerpRatio = 0.f;
	m_v3OriginRot = m_vRelativeRot;
}

void CTransform::CancelLerpRot()
{
	m_bLerpFlag = false;
	m_fLerpRatio = 0.f;
}

void CTransform::finaltick()
{
	if (GetOwner()->GetName().compare(L"Attrox") == 0)
		int a = 0;

	if (m_bLerpFlag)
	{
		if (m_fLerpRatio >= 1.f)
		{
			CancelLerpRot();
		}
		else if (m_v3LerpGoalRot == m_vRelativeRot)
		{
			CancelLerpRot();
		}
		else
		{
			m_vRelativeRot = XMVectorLerp(m_v3OriginRot, m_v3LerpGoalRot, m_fLerpRatio);
			m_fLerpRatio += DT;
		}
	}
	m_matWorldScale = XMMatrixIdentity();
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);
	m_matWorldRot = matRot;

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);


	m_matWorld = m_matWorldScale * matRot * matTranslation;

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);
	}

	// 부모 오브젝트 확인
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentRot = pParent->Transform()->m_matWorldRot;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);
			Matrix matParentRotInv = XMMatrixInverse(nullptr, matParentRot);

			// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentRotInv * matParentWorld;
		}
		else
		{
			m_matWorldRot = pParent->Transform()->m_matWorldRot;
			m_matWorldScale = pParent->Transform()->m_matWorldScale;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}


		for (int i = 0; i < 3; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	// 위치값을 상수버퍼에 전달 및 바인딩		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
}
