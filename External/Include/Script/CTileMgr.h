#pragma once
#include <Engine\CSingleton.h>
class CTileMgr :
    public CSingleton<CTileMgr>
{
    SINGLE(CTileMgr)
private:
    Vec2                    m_Offset;
    Vec2                    m_Size;
    Vec2                    m_Count;
    Vec3                    m_StartPos;
    vector<CGameObject*>    m_vecTile;
public:
    void SetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start);
    void CreateTile();
    vector<int> Find(int _istart, int _iend);
    Vec2 GetTilePos(int _itile);
    void SetStartPos(const Vec3& _start) { m_StartPos = _start; }
};

