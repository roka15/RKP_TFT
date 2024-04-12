#pragma once
#include "CSingleton.h"
class CGameObject;



class CMouseMgr :
    public CSingleton<CMouseMgr>
{
    SINGLE(CMouseMgr);
private:
    CGameObject*        m_CursorList[(UINT)CURSOR_TYPE::END];
    PointerEventData    m_BeforEventInfo[(UINT)CURSOR_TYPE::END];
    float               m_fCurTime;
public:
    void init();
    void tick();
    void render();
private:
    void CalcuMousePos(CURSOR_TYPE _eType);
    void CalcuMousePos2D();
    void CalcuMousePos3D();
public:
    void RegisterCursor(CURSOR_TYPE _eType,CGameObject* _pCursor) { m_CursorList[(UINT)_eType] = _pCursor; }
    void ClickPlane();
    Vec3 GetMousePos();
    CGameObject* GetCursor(CURSOR_TYPE _eType) { return m_CursorList[(UINT)_eType]; }
    UINT GetCursorCnt() { return (UINT)CURSOR_TYPE::END; }

	void CursorDownEvent(PointerEventData& _data,CURSOR_TYPE _eType);
	void CursorUpEvent(PointerEventData& _data, CURSOR_TYPE _eType);
    void MouseDownEvent(vector<CGameObject*>& _pObjs,PointerEventData& _data, CURSOR_TYPE _eType);
    void MouseDragEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data, CURSOR_TYPE _eType);
    void MouseUpEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data, CURSOR_TYPE _eType);
};

