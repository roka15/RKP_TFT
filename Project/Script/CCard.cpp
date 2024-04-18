#include "pch.h"
#include "CCard.h"
#include "CGameMgr.h"

void CCard::SetInfo(CHARACTER_TYPE _eChType, ITEM_CARD_TYPE _eCardType)
{
	wstring CardImage;
	wstring CardName; 
	wstring CardCostText;

	const t_CharacterInfo& info = CGameMgr::GetInst()->GetChInfo(_eChType);
	CardImage = info.strBaseImage;
	CardName = info.strNameText;
	CardCostText = std::to_wstring(info.iCost);
	
	CGameObject* pObj = GetOwner()->GetChild(L"Name");
	pObj->Text()->SetText(CardName);
	GetOwner()->Image()->SetNormalTexKey(CardImage);
	pObj = GetOwner()->GetChild(L"Cost");
	pObj->Text()->SetText(CardCostText);

	wstring EnterKey;
	wstring ExitKey;
	const t_ItemCardInfo& cardInfo = CGameMgr::GetInst()->GetCardInfo(_eCardType);
	EnterKey = cardInfo.strEnterImage;
	ExitKey = cardInfo.strExitImage;

	pObj = GetOwner()->GetChild(L"Image");
	pObj->Image()->SetNormalTexKey(ExitKey);
	pObj->Image()->SetEnableTexKey(EnterKey);
}

void CCard::EnterEvent(PointerEventData)
{
	CGameObject* pObj = GetOwner()->GetChild(L"Image");
	pObj->Image()->SetActiveImage(true);
}

void CCard::ExitEvent(PointerEventData)
{
	CGameObject* pObj = GetOwner()->GetChild(L"Image");
	pObj->Image()->SetActiveImage(false);
}


CCard::CCard():CScript((UINT)SCRIPT_TYPE::CARD)
{
}

CCard::~CCard()
{
}
