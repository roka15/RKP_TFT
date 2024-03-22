#pragma once
#include <Engine\CSingleton.h>
#include <Engine\CClientMgr.h>
enum class TILE_TYPE
{
    WAIT,
    BATTLE,
    END,
};
enum class TILE_OWNER_TYPE
{
    PLAYER,
    ENEMY,
    END
};
class CTileScript;

class CTileMgr :
    public CSingleton<CTileMgr>, public IClientManager
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
    vector<CGameObject*>    m_vecBattleTile;
public:
    virtual void init()override;
    virtual void tick()override;
private:
    CTileScript* GetTile(int _iid);
public:
    //get set
    Vec3 GetBattleTileWorldPos(int _itile);
    const TILE_OWNER_TYPE& GetTileOwnerType(int _iTileNum);
    void RegisterItem(int _tileNum, CGameObject* _pObj);
    bool EmptyBattleTile(int _iid);
    void EnableSelectBattleTile(bool _flag);
    Vec2 GetBattleCount() { return m_Count; }
    Vec2 GetWaitCount() { return m_WaitCount; }
    vector<int> GetAnotherPlayer(TILE_OWNER_TYPE _eType);
    void BattleRouteRender(vector<int> _vecRoute);
    int FindEmptyWaitTile();
public:
    void BattleSetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start);
    void WaitSetInfo(const Vec2& _offset, const Vec2& _size, const Vec2& _count, const Vec3& _start);
    CGameObject* CreateTile(TILE_OWNER_TYPE _type);
   
};

