#include "pch.h"
#include "CItem.h"

int CItem::Buy(const int& _iMoney)
{
	return _iMoney - m_Cost;
}

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
