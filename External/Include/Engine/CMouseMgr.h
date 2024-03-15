#pragma once
#include "CSingleton.h"
class CGameObject;

enum class MOUSE_INPUT_TYPE
{
    LEFT = 0,
    RIGHT,
    MIDDLE,
};
struct PointerEventData
{
    MOUSE_INPUT_TYPE    button;         //클릭 타입
    int                 clickCount;     //클릭 횟수, 더블 클릭
    float               clickTime;      //클릭 이벤트 발생한 시간
    Vec2                delta;          //이전 프레임, 현재 프레임 위치 변화량
    vector<CGameObject*>hovered;        //마우스 올린 상태일 때 해당 포인터에서 오브젝트 목록들.
    CGameObject*        lastPress;      //마지막 마우스 다운 지점
    CGameObject*        pointerPress;   //마우스 다운 지점
    CGameObject*        pointerDrag;    //드래그 중인 오브젝트
    bool                bDrag;          //드래그 중인지.
};
class CMouseMgr :
    public CSingleton<CMouseMgr>
{
    SINGLE(CMouseMgr);
private:
    CGameObject*        m_pCursor;
    PointerEventData    m_BeforEventInfo;
    float               m_fCurTime;
public:
    void init();
    void tick();
    void render();
public:
    void RegisterCursor(CGameObject* _pCursor) { m_pCursor = _pCursor; }
    void ClickPlane();
    Vec3 GetMousePos();
    CGameObject* GetCursor() { return m_pCursor; }

    void MouseDownEvent(vector<CGameObject*>& _pObjs,PointerEventData& _data);
    void MouseDragEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data);
    void MouseUpEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data);
};

