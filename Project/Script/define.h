#pragma once
enum class CHARACTER_TYPE
{
	ATTROX,
    SLOT_ITEM,
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
    END,
};