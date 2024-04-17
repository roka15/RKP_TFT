#pragma once
struct t_RoundInfo
{
    int                     iMinionCnt;
    vector<int>             vecMinionTileNum;
};
struct t_CharacterInfo
{
	CHARACTER_TYPE  eChType;
	ITEM_CARD_TYPE  eCardType;
	int				iCost;
	wstring	        strNameText;
	wstring			strBaseImage;
};
struct t_ItemCardInfo
{
	wstring			strEnterImage;
	wstring         strExitImage;
};