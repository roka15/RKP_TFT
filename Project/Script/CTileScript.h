#pragma once
#include <Engine\CScript.h>
#include <Engine\IMouseEvent.h>
enum class TILE_TYPE;
enum class TILE_OWNER_TYPE;

class CTileScript :
    public CScript , public IMouseDownEvent, public IMouseUpEvent
{
private:
    TILE_TYPE            m_Type;
    TILE_OWNER_TYPE      m_OwnerType;
    int                  m_Number;
    Vec4                 m_Color;
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider* _Other)override;
    virtual void OnOverlap(CCollider* _Other)override;
    virtual void EndOverlap(CCollider* _Other) override;

    virtual void DownEvent(PointerEventData)override;
    virtual void UpEvent(PointerEventData) override;

    void SetTileInfo(TILE_TYPE _type, TILE_OWNER_TYPE _ownerType, int _num) { m_Type = _type; m_OwnerType = _ownerType;  m_Number = _num; }
    const TILE_TYPE& GetType()const { return m_Type; }
    const TILE_OWNER_TYPE& GetOwnerType()const { return m_OwnerType; }
    int GetNumber() { return m_Number; }
    void AddItem(CGameObject* _obj);
    void ChangeItemState(bool _flag);
    void HighlightColor(bool _flag);
public:
    CLONE(CTileScript)
    CTileScript();
    virtual ~CTileScript();
};

