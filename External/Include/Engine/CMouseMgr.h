#pragma once
#include "CSingleton.h"
class CGameObject;
class CMouseMgr :
    public CSingleton<CMouseMgr>
{
    SINGLE(CMouseMgr);
private:
    CGameObject* m_pCursor;

public:
    void init();
    void tick();
    void render();
    void RegisterCursor(CGameObject* _pCursor) { m_pCursor = _pCursor; }
    void ClickPlane();
    Vec3 GetMousePos();
    CGameObject* GetCursor() { return m_pCursor; }
};

