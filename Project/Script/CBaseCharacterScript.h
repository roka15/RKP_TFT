#pragma once
#include "CItem.h"
#include "CharacterInfo.h"
#include <Engine\CKeyMgr.h>

class CBaseCharacterScript :
	public CItem
{
private:
	tCharacterState     m_ChState;
	tCharacterStatus    m_ChStatus;
	tItemInfo           m_Items[MAX_ITEM];
	Vec3                m_v3TargetPos;
	int                 m_iTargetNum;
	int					m_iStartTileNum;
	bool                m_bMove;
	Vec2                m_v2Dir;
private:
	void ChangeTransInfo();
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
	void SetWait(bool _flag) { m_ChState.bWaiting = _flag; }
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
	void CurStartTile();
	void ResetTile(); 

public:
	CBaseCharacterScript();
	CBaseCharacterScript(SCRIPT_TYPE _eType);
	virtual ~CBaseCharacterScript();
	CLONE(CBaseCharacterScript)
};
