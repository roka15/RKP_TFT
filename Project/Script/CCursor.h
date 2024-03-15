#pragma once
#include <Engine\CScript.h>
#include <Engine\IMouseEvent.h>
class CCursor :
    public CScript,public IMouseDownEvent, public IMouseUpEvent
{
private:
    CGameObject* m_DragStartObj;
public:
    void RegisterDragItem(CGameObject* _obj);
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider* _Other)override;
    virtual void OnOverlap(CCollider* _Other)override;
    virtual void EndOverlap(CCollider* _Other)override;

    virtual void DownEvent(PointerEventData) override;
    virtual void UpEvent(PointerEventData) override;
public:
    CLONE(CCursor);
    CCursor();
    virtual ~CCursor();
};

