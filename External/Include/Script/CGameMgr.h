#pragma once
#include <Engine\CSingleton.h>
#include <Engine\CClientMgr.h>

class CGame;

class CGameMgr :
    public CSingleton<CGameMgr> , public IClientManager
{
    SINGLE(CGameMgr)
private:
    vector<CGame*> m_vecGames;
public:
    void init();
    void tick();
public:
    void CreateGame();
    bool EnterGame(int _iID, CGameObject* _pObj);
    int GetState(CGameObject* _pObj);
    bool BuyItem(int _iGameID,CHARACTER_TYPE _eType,CGameObject* _pPlayer);
    void CreateCharacterPrefabs();
};

