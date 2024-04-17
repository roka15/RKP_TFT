#include "pch.h"
#include "CItem.h"

CItem::CItem() :
	CScript((UINT)SCRIPT_TYPE::ITEM)
{
}

CItem::CItem(SCRIPT_TYPE _eType)
	: CScript((UINT)_eType)
{
}

CItem::CItem(const CItem& _ref)
	:CScript(_ref),
	m_pPlayer(nullptr)
{
}

CItem::~CItem()
{
}
