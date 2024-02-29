#pragma once
#include <Engine\CScript.h>
enum class TILE_TYPE
{
    WAIT,
    BATTLE,
    END,
};
class CTileScript :
    public CScript
{
private:
    TILE_TYPE m_Type;
    int       m_Number;
public:
    void SetTileInfo(TILE_TYPE _type, int _num) { m_Type = _type; m_Number = _num; }
    TILE_TYPE GetType() { return m_Type; }
    int GetNumber() { return m_Number; }
public:
    CLONE(CTileScript)
    CTileScript();
    virtual ~CTileScript();
};

