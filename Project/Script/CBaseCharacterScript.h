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
	int                 m_iMoveTargetNum;
	int					m_iStartTileNum;
	int					m_iAtkTargetNum;
	bool                m_bMove;
	Vec2                m_v2Dir;
private:
	void ChangeTransInfo();

	void AddULTGauge();
public:
	virtual void start()override;
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
	virtual void SendDamage();
	virtual void RecvDamage(float _damage);
public:
	void SetMove(bool _flag) { m_ChState.bMove = _flag; }
	void SetWait(bool _flag) { m_ChState.bWaiting = _flag; }
	void SetAtk(bool _flag) { m_ChState.bAttack = _flag; }
	void CurStartTile();
	void ResetTile(); 

protected:
	void Battle(CGameObject* _pTileObj);
public:
	CBaseCharacterScript();
	CBaseCharacterScript(SCRIPT_TYPE _eType);
	CBaseCharacterScript(const CBaseCharacterScript& _ref);
	virtual ~CBaseCharacterScript();
	CLONE(CBaseCharacterScript)
};
