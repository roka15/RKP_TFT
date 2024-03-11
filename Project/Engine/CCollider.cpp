#include "pch.h"
#include "CCollider.h"
#include "CScript.h"
void CCollider::finaltick()
{
	int a = 0;
}

void CCollider::BeginOverlap(CCollider* _Other)
{
	m_iCollisionCount += 1;

	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider::OnOverlap(CCollider* _Other)
{
	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider::EndOverlap(CCollider* _Other)
{
	m_iCollisionCount -= 1;

	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

CCollider::CCollider(COMPONENT_TYPE _eType)
	:
	CComponent(_eType),
	m_iCollisionCount(0)
{
}

CCollider::~CCollider()
{
}
