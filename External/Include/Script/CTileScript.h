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
    Vec4      m_Color;
public:
    virtual void tick()override;
    void SetTileInfo(TILE_TYPE _type, int _num) { m_Type = _type; m_Number = _num; }
    TILE_TYPE GetType() { return m_Type; }
    int GetNumber() { return m_Number; }
    Vec4 GetColor() { return m_Color; }
    void SetColor(const Vec4& _color) { m_Color = _color; }
public:
    CLONE(CTileScript)
    CTileScript();
    virtual ~CTileScript();
};

