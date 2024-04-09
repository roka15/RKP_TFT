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
	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D 업데이트
	if (Animator3D())
	{
		if (Animator3D()->IsActiveAni())
		{
			Animator3D()->UpdateData();

			for (UINT i = 0; i < GetMtrlCount(); ++i)
			{
				if (nullptr == GetMaterial(i))
					continue;

				GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
				GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
			}
		}
	}

	// 렌더
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// 사용할 재질 업데이트
			GetMaterial(i)->UpdateData();

			// 사용할 메쉬 업데이트 및 렌더링
			GetMesh()->render(i);
		}
	}

	// Animation 관련 정보 제거
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

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D 업데이트
	if (Animator3D())
	{
		if (Animator3D()->IsActiveAni())
		{
			Animator3D()->UpdateData();
			GetMaterial(_iSubset)->SetAnim3D(true); // Animation Mesh 알리기
			GetMaterial(_iSubset)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	// 사용할 재질 업데이트
	GetMaterial(_iSubset)->UpdateData();

	// 사용할 메쉬 업데이트 및 렌더링
	GetMesh()->render(_iSubset);

	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

