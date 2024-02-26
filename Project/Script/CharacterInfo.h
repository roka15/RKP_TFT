#pragma once
//���߿� item ����� �ű�� �ű��.
#define MAX_ITEM 3
struct tItemInfo
{
	float	 fATK;				//���ݷ�
	float	 fDEF;				//����
	float	 fAttackRange;		//��Ÿ�
	int		 iActiveStack;		//�߰� ����
};
struct tCharacterState
{
	bool	 bWaiting;
	bool	 bAttack;
	bool	 bUlt;
	bool	 bMove;
	bool	 bDance;
	//�̰� ���߿� ���� ������ �־����.
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
	float	 fATK;				//���ݷ�
	float	 fDEF;				//����
	float	 fAttackRange;		//��Ÿ�
	float	 fMoveSpeed;		//�̵� �ӵ�
};
