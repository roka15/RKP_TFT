#include "pch.h"
#include "CBillboard.h"
#include "CResMgr.h"
#include "CGameObject.h"
#include "components.h"

CBillboard::CBillboard() :
	CRenderComponent(COMPONENT_TYPE::BILLBOARD),
	m_GaugeType(GAUGE_TYPE::END)
{
	SetActive(true);
	// ���� �޽�
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	// ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BillboardRenderMtrl"), 0);
}

CBillboard::CBillboard(const CBillboard& _ref):
	CRenderComponent(_ref),
	m_GaugeType(_ref.m_GaugeType)
{
	SetActive(true);
}

CBillboard::~CBillboard()
{
}


void CBillboard::finaltick()
{
}

void CBillboard::render()
{
	if (GetActive() == false)
		return;
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;
	if (GetOwner()->Text())
	{
		GetOwner()->Text()->UpdateData();
		return;
	}
	else if (GetOwner()->Image())
	{
		GetOwner()->Image()->UpdateData();
	}
	// Transform �� UpdateData ��û
	Transform()->UpdateData();
	
	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}


	// ����
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	CRenderComponent* renderComp = GetOwner()->GetRenderComponent();
	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			Vec3 v3Scale = Transform()->GetRelativeScale();
			Vec4 v4Scale = Vec4(v3Scale.x, v3Scale.y, v3Scale.z, 0.f);
			renderComp->GetMaterial(i)->SetScalarParam(SCALAR_PARAM::VEC4_0, &v4Scale);
			// ����� ���� ������Ʈ
			GetMaterial(i)->UpdateData();

			// ����� �޽� ������Ʈ �� ������
			GetMesh()->render(i);
		}
	}

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();
}

void CBillboard::render(UINT _iSubset)
{
	if (GetActive() == false)
		return;
	if (nullptr == GetMesh() || nullptr == GetMaterial(_iSubset))
		return;

	if (GetOwner()->Text())
	{
		GetOwner()->Text()->UpdateData();
		return;
	}
	else if (GetOwner()->Image())
	{
		GetOwner()->Image()->UpdateData();
	}

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	Vec3 v3Scale = Transform()->GetRelativeScale();
	Vec4 v4Scale = Vec4(v3Scale.x, v3Scale.y, v3Scale.z, 0.f);
	MeshRender()->GetMaterial(_iSubset)->SetScalarParam(SCALAR_PARAM::VEC4_0, &v4Scale);
	// ����� ���� ������Ʈ
	GetMaterial(_iSubset)->UpdateData();

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render(_iSubset);

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();
}

void CBillboard::SaveToLevelFile(FILE* _File)
{
}

void CBillboard::LoadFromLevelFile(FILE* _File)
{
}

