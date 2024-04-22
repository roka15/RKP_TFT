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
	int					m_iAtkTargetNum;
	bool                m_bMove;
	Vec2                m_v2Dir;
	Vec3				m_v3PrevDir;
	CGameObject* m_pTargetObj;
private:
	void ChangeTransInfo();
	void UpdateCharacterUI();
	void AddULTGauge();
	void LookAtTarget(CGameObject* _pTarget);
public:
	virtual void start()override;
	virtual void tick()override;
	virtual void BeginOverlap(CCollider* _Other)override;
	virtual void OnOverlap(CCollider* _Other)override;
	virtual void EndOverlap(CCollider* _Other)override;
	virtual void RegisterFuncPtr()override;
public:
	const bool& IsWait();
	const bool& IsUlt();
	const bool& IsMove();
	const bool& IsAttack();
	const bool& IsDance();
	const bool& IsDeath();
	const bool& IsEnd();
	int  GetAtkNumber();


	void SetWaitingSeat(bool _flag) { m_ChState.bWaiting = _flag; }
	virtual void WaitTileTick();
	virtual void BattleTileTick();
	virtual void UltGaugeUp();
	virtual void UltGaugeDown();
	virtual void AttackOFF() { m_ChState.bAttack = false; }

	virtual void SetTarget(CCollider* _Other);
	virtual void SendDamage();
	virtual void RecvDamage(float _damage);
	virtual void Death();
public:
	void SetMove(bool _flag);
	void SetWait(bool _flag);
	void SetAtk(bool _flag);
	void SetUlt(bool _flag);
	void SetAtkNumber(int _iNumber);
	void SetDeath(bool _flag);
	void CurStartTile();
	void Reset();
	void BattleStateReset();

protected:
	void Battle(CGameObject* _pTileObj);
public:
	CBaseCharacterScript();
	CBaseCharacterScript(SCRIPT_TYPE _eType);
	CBaseCharacterScript(const CBaseCharacterScript& _ref);
	virtual ~CBaseCharacterScript();
	CLONE(CBaseCharacterScript)
};
