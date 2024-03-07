#pragma once
#include <Engine\CScript.h>
enum class TILE_TYPE;
enum class TILE_OWNER_TYPE;

class CTileScript :
    public CScript
{
private:
    TILE_TYPE            m_Type;
    TILE_OWNER_TYPE      m_OwnerType;
    int                  m_Number;
    Vec4                 m_Color;
public:
    virtual void tick()override;
    void SetTileInfo(TILE_TYPE _type, TILE_OWNER_TYPE _ownerType, int _num) { m_Type = _type; m_OwnerType = _ownerType;  m_Number = _num; }
    TILE_TYPE GetType() { return m_Type; }
    int GetNumber() { return m_Number; }
    Vec4 GetColor() { return m_Color; }
    void SetColor(const Vec4& _color) { m_Color = _color; }
public:
    CLONE(CTileScript)
    CTileScript();
    virtual ~CTileScript();
};

