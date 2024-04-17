#pragma once
enum class CHARACTER_TYPE
{
	ATTROX,
    ZED,
    Ahri,
    Ashe,
    Azir,
    Azirsoldier,
    BlitzCrank,
    Gwen,
   
	END,
};
enum class GAME_STATE
{
    SELECT = 0,
    BATTLE,
    ITEM,
	LOADING,
    END,
};
enum class PLAYER_TYPE
{
    CLIENT=0,
    ANOTHER,
    AI,
    END,
};
enum class ITEM_CARD_TYPE
{
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    END
};