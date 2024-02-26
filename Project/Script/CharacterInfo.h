#pragma once
//나중에 item 만들면 거기로 옮기기.
#define MAX_ITEM 3
struct tItemInfo
{
	float	 fATK;				//공격력
	float	 fDEF;				//방어력
	float	 fAttackRange;		//사거리
	int		 iActiveStack;		//추가 성능
};
struct tCharacterState
{
	bool	 bWaiting;
	bool	 bAttack;
	bool	 bUlt;
	bool	 bMove;
	bool	 bDance;
	//이건 나중에 게임 정보에 넣어야함.
	bool	 bEnd;
};
struct tCharacterStatus
{
	float	 fSightRange;
	double	 dMaxUltGauge;
	double	 dCurUltGauge;
	double   dAddUltGauge;
	double	 dMinusUltGauge;
	float    fUltATK;
	float    fUltDEF;
	int		 iMaxAttackType;
	int		 iCurAttackType;
	float	 fATK;				//공격력
	float	 fDEF;				//방어력
	float	 fAttackRange;		//사거리
	float	 fMoveSpeed;		//이동 속도
};
