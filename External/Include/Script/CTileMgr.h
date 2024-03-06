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
    
    Vec2                    m_WaitOffset;
    Vec2                    m_WaitSize;
    Vec2                    m_WaitCount;
    Vec3                    m_WaitStartPos;

    static int              s_number;
    vector<CGameObject*>    m_vecTile;
public:
    void BattleSetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start);
    void WaitSetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start);
    void CreateTile();
    vector<int> Find(int _istart, int _iend);
    Vec2 GetTilePos(int _itile);
    void SetStartPos(const Vec3& _start) { m_StartPos = _start; }
};

