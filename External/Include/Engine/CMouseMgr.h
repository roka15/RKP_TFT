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
    MOUSE_INPUT_TYPE    button;         //Ŭ�� Ÿ��
    int                 clickCount;     //Ŭ�� Ƚ��, ���� Ŭ��
    float               clickTime;      //Ŭ�� �̺�Ʈ �߻��� �ð�
    Vec2                delta;          //���� ������, ���� ������ ��ġ ��ȭ��
    vector<CGameObject*>hovered;        //���콺 �ø� ������ �� �ش� �����Ϳ��� ������Ʈ ��ϵ�.
    CGameObject*        lastPress;      //������ ���콺 �ٿ� ����
    CGameObject*        pointerPress;   //���콺 �ٿ� ����
    CGameObject*        pointerDrag;    //�巡�� ���� ������Ʈ
    bool                bDrag;          //�巡�� ������.
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

