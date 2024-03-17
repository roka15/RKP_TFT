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
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider* _Other) override;
private:
   

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CPlayerScript);
public:
    void SetGameState(UINT _iMode) { m_GameState = _iMode; }
    void SetGameID(UINT _iID) { m_GameKey = _iID; }
public:
    CPlayerScript();
    ~CPlayerScript();
};

