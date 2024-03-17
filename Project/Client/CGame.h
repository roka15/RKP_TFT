#pragma once
enum class GAME_STATE
{
    SELECT=0,
    BATTLE,
    ITEM,
    END,
};
class CPlayerScript;
class CGame
{
private:
    int                         m_GameID;
    const int                   m_iUserCnt;
    float                       m_fStartTime;
    float                       m_fCurTime;
    float                       m_fSelectTime;
    float                       m_fBettleTime;
    float                       m_fItemTime;
    UINT                        m_iRoundCnt;
    GAME_STATE                  m_eGameState;
    vector<CPlayerScript*>      m_vecUsers;
private:
    void SendGameState(UINT _iState);
public:
    void SetGameID(int _iID) { m_GameID = _iID; }
    const int& GetGameID() { return m_GameID; }
    int  CanEnter();
public:
    void init();
    void tick();
    void RegisterUser(int _iGameID,int _iIdx,CGameObject* _pObj);
public:
    CGame();
    virtual ~CGame();
};

