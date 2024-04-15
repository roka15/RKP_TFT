#pragma once
#include <Engine\CSingleton.h>
#include <Engine\CClientMgr.h>
struct t_SharedGameInfo
{
    //라운드 정보(미니언 라운드 인지,미니언이면 어느 곳에 몇개 배치할 것인지 등..)
    //지금은 Game에 있음.

    //Level 당 EXP
    map<int, int>        mapLevelMaxExp;
    int                  ExpUpCost;
    int                  RefreshCost;
};
class CGame;
class CPlayerScript;
class CGameMgr :
    public CSingleton<CGameMgr>, public IClientManager
{
    SINGLE(CGameMgr)
private:
    vector<CGame*>      m_vecGames;
    CGameObject*        m_ClientOwner;
    t_SharedGameInfo    m_SharedGameInfo;
private:
    void LoadGameInfo();
public:
    void init();
    void tick();
public:
    void CreateGame();
    bool EnterGame(int _iID, CGameObject* _pObj);
    int GetState(CGameObject* _pObj);
    int GetState();
    CGameObject* GetClient() { return m_ClientOwner; }
    const int& GetExpCost() { return m_SharedGameInfo.ExpUpCost; }
    const int& GetRefreshCost() { return m_SharedGameInfo.RefreshCost; }
    bool BuyItem(int _iGameID, CHARACTER_TYPE _eType, CGameObject* _pPlayer);
    bool BuyExp(int& _iCurExp, int& _iLevel, int& _iMoney);
    const int& GetCurMaxExp(const int& _iLevel) { return m_SharedGameInfo.mapLevelMaxExp[_iLevel]; }
    void DeathMinion(int _iGameID, CGameObject* _pMinion);
    void SetClientOwner(CGameObject* _pObj) { m_ClientOwner = _pObj; }
    void CreateCharacterPrefabs();
    void RegisterUImap(int _iGameID, wstring _Key, CGameObject* _pObj);
    bool IsSamePlayer(CGameObject* _pObj); //client 와 일치하는지 체크
};

