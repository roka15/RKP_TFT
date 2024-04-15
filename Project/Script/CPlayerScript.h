#pragma once
#include <Engine\CScript.h>

class CBaseCharacterScript;
class CPlayerScript :
    public CScript
{
private:
    float                           m_fSpeed;
    UINT                            m_GameState;
    int                             m_GameKey;
    int                             m_Money;
    vector<CGameObject*>            m_vecItem;
    PLAYER_TYPE                     m_ePlayerType;
    int                             m_CurExp;
    int                             m_Level;
public:
    virtual void start()override;
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider* _Other) override;
    virtual void RegisterFuncPtr()override;
private:
    void BuyExp();

    void VoidTest();
    void IntTest(int _int);
    void FloatTest(float _float);
    void StringTest(string _str);
    void ObjTest(CGameObject* _pObj);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CPlayerScript);
public:
	void SetGameState(UINT _iMode) { m_GameState = _iMode; }
    void SetGameID(UINT _iID) { m_GameKey = _iID; }
    int GetGameID() { return m_GameKey; }
    const int& GetMoney() { return m_Money; }
    void AddItem(CGameObject* _pObj) { m_vecItem.push_back(_pObj); }
    vector<Vec3> GetItemPos();
    void SetPlayerType(PLAYER_TYPE _eType) { m_ePlayerType = _eType; }
    PLAYER_TYPE GetPlayerType() { return m_ePlayerType; }
	void SetGameStateInfo();
    const int& GetLevel() { return m_Level; }
    const int& GetExp() { return m_CurExp; }
public:
    CPlayerScript();
    ~CPlayerScript();
};

