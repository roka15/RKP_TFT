#pragma once

class CPlayerScript;
class CItem;
struct t_RoundInfo
{
    int                     iMinionCnt;
    vector<int>             vecMinionTileNum;
};
class CGame
{
private:
    int                                          m_GameID;
    const int                                    m_iUserCnt;
    float                                        m_fStartTime;
    float                                        m_fCurTime;
    float                                        m_fSelectTime;
    float                                        m_fBettleTime;
    float                                        m_fItemTime;
	float									     m_fLoadingTime;
    UINT                                         m_iRoundCnt; 
    GAME_STATE                                   m_eGameState;
    vector<CPlayerScript*>                       m_vecUsers;
    map<CHARACTER_TYPE, queue<CGameObject*>>     m_mapShop;
    queue<CGameObject*>                          m_DespawnMinion;
    queue<CGameObject*>                          m_SpawnMinion;
    map<int, t_RoundInfo>                        m_RoundInfo;
    bool                                         m_bFirstLoading;
    CGameObject*                                 m_pAIPlayer;
private:
    void SendGameState(UINT _iState);
    int  Buy(CItem* _pItem, CPlayerScript* _pPlayer);
    void CreateMinion();
    bool SpawnMinion(int _iRound);
    void DespawnMinion();
public:
    void SetGameID(int _iID) { m_GameID = _iID; }
    const int& GetGameID() { return m_GameID; }
    const UINT& GetGameState() { return (UINT)m_eGameState; }
    int  CanEnter();
public:
    void init();
    void tick();
    void RegisterUser(int _iGameID,int _iIdx,CGameObject* _pObj);
    bool BuyItem(CHARACTER_TYPE _eType, CGameObject* _pPlayer);
public:
    CGame();
    virtual ~CGame();
};

