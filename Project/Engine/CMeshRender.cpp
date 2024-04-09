#include "pch.h"
#include "CMeshRender.h"


#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CText.h"
#include "CImage.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{
	SetActive(true);
}

CMeshRender::CMeshRender(const CMeshRender& _ref) : CRenderComponent(_ref)
{
	SetActive(true);
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
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

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		if (Animator3D()->IsActiveAni())
		{
			Animator3D()->UpdateData();

			for (UINT i = 0; i < GetMtrlCount(); ++i)
			{
				if (nullptr == GetMaterial(i))
					continue;

				GetMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
				GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
			}
		}
	}

	// ����
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// ����� ���� ������Ʈ
			GetMaterial(i)->UpdateData();

			// ����� �޽� ������Ʈ �� ������
			GetMesh()->render(i);
		}
	}

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::render(UINT _iSubset)
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

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		if (Animator3D()->IsActiveAni())
		{
			Animator3D()->UpdateData();
			GetMaterial(_iSubset)->SetAnim3D(true); // Animation Mesh �˸���
			GetMaterial(_iSubset)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	// ����� ���� ������Ʈ
	GetMaterial(_iSubset)->UpdateData();

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render(_iSubset);

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

