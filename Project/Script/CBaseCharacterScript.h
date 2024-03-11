#pragma once
#include <Engine\CScript.h>
#include "CharacterInfo.h"
#include <Engine\CKeyMgr.h>
class CBaseCharacterScript :
    public CScript
{
private:
    tCharacterState     m_ChState;
    tCharacterStatus    m_ChStatus;
    tItemInfo           m_Items[MAX_ITEM];
    CTransform*         m_pTarget;
public:
    virtual void tick()override;
    virtual void BeginOverlap(CCollider* _Other)override;
    virtual void OnOverlap(CCollider* _Other)override;
    virtual void EndOverlap(CCollider* _Other)override;
public:
    bool IsWait() { return m_ChState.bWaiting; }
    bool IsUlt() { return m_ChState.bUlt; }
    bool IsMove() { return m_ChState.bMove; }
    bool IsAttack() { return m_ChState.bAttack; }
    bool IsDance() { return m_ChState.bDance; }
    bool IsEnd() { return m_ChState.bEnd; }
    virtual bool IsFindEnemy() { return m_pTarget != nullptr; }
    virtual bool IsWithinAttackRange();
    void SetWaitingSeat(bool _flag) { m_ChState.bWaiting = _flag; }
    virtual void UltGaugeUp();
    virtual void UltGaugeDown();
    virtual void AttackOFF() { m_ChState.bAttack = false; }
    virtual void Search();
    virtual void Wait();
    virtual void Move();
    virtual void Attack();
    virtual void NormalAttack();
    virtual void UltAttack();
    virtual void Dance();
    virtual void SetTarget(CCollider* _Other);
public:
    CBaseCharacterScript();
    CBaseCharacterScript(SCRIPT_TYPE _eType);
    virtual ~CBaseCharacterScript();
    CLONE(CBaseCharacterScript)
};
