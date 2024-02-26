#pragma once
#include <Engine\CScript.h>
#include "CharacterInfo.h"

class CBaseCharacterScript :
    public CScript
{
private:
    tCharacterState     m_ChState;
    tCharacterStatus    m_ChStatus;
    tItemInfo           m_Items[MAX_ITEM];
    CTransform*         m_pTarget;
public:
    virtual void BeginOverlap(CCollider2D* _Other)override;
    virtual void OnOverlap(CCollider2D* _Other)override;
    virtual void EndOverlap(CCollider2D* _Other)override;
public:
    bool IsWaitingSeat() { return m_ChState.bWaitingSeat; }
    bool IsUlt() { return m_ChState.bUlt; }
    virtual bool IsFindEnemy() { return m_pTarget != nullptr; }
    virtual bool IsWithinAttackRange();
    void SetWaitingSeat(bool _flag) { m_ChState.bWaitingSeat = _flag; }
    virtual void UltGaugeUp();
    virtual void UltGaugeDown();
    virtual void Search();
    virtual void Idle();
    virtual void Move();
    virtual void NormalAttack();
    virtual void UltAttack();
    virtual void Dance();
    virtual void SetTarget(CCollider2D* _Other);
public:
    CBaseCharacterScript();
    CBaseCharacterScript(SCRIPT_TYPE _eType);
    virtual ~CBaseCharacterScript();
    CLONE(CBaseCharacterScript)
};


//virtual bool IsWaitingSeat() = 0;
//virtual bool IsFindEnemy() = 0;
//virtual bool IsWithinAttackRange() = 0;
//virtual bool IsMaxUlt() = 0;
//virtual void UltGaugeUp() = 0;
//virtual void UltGaugeDown() = 0;
//virtual void SetWaitingSeat(bool _flag) = 0;

