#pragma once

class CPlayerScript;
class CItem;
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
    vector<CGameObject*>                         m_vecMinion;
private:
    void SendGameState(UINT _iState);
    int Buy(CItem* _pItem, CPlayerScript* _pPlayer);
    void CreateMinion();
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

