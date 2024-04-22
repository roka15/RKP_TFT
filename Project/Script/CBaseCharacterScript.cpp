#include "pch.h"
#include "CTileMgr.h"
#include "CAStarMgr.h"
#include "CBaseCharacterScript.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CTileScript.h"
#include "CPlayerScript.h"
#include  "CGameMgr.h"
#include <Engine\CAnimation3D.h>
#include <Engine\CAnimator3D.h>




CBaseCharacterScript::CBaseCharacterScript() :
	CItem(SCRIPT_TYPE::BASECHARACTERSCRIPT),
	m_ChState{ true,false,false,false,false,false },
	m_bMove(false),
	m_pTargetObj(nullptr)
{
	m_ChStatus.fAttackRange = 1;
	m_ChStatus.iHp = 40;
	m_ChStatus.iMaxHp = 40;
	m_ChStatus.dCurUltGauge = 0;
	m_ChStatus.dAddUltGauge = 10;
	m_ChStatus.dMinusUltGauge = 10;
	m_ChStatus.dMaxUltGauge = 50;


	SetCost(10);
}

CBaseCharacterScript::CBaseCharacterScript(SCRIPT_TYPE _eType) :
	CItem(_eType),
	m_pTargetObj(nullptr)
{
	m_ChStatus.fAttackRange = 1;
	m_ChStatus.iHp = 40;
	m_ChStatus.iMaxHp = 40;
	m_ChStatus.dCurUltGauge = 0;
	m_ChStatus.dAddUltGauge = 10;
	m_ChStatus.dMinusUltGauge = 10;
	m_ChStatus.dMaxUltGauge = 50;
	SetCost(10);
}

CBaseCharacterScript::CBaseCharacterScript(const CBaseCharacterScript& _ref)
	:CItem(_ref),
	m_ChState{ true,false,false,false,false,false },
	m_bMove(false),
	m_pTargetObj(nullptr)
{
	m_ChStatus.fAttackRange = 1;
	m_ChStatus.iHp = 40;
	m_ChStatus.iMaxHp = 40;
	m_ChStatus.dCurUltGauge = 0;
	m_ChStatus.dAddUltGauge = 10;
	m_ChStatus.dMinusUltGauge = 10;
	m_ChStatus.dMaxUltGauge = 50;
	SetCost(10);
}

CBaseCharacterScript::~CBaseCharacterScript()
{
}


void CBaseCharacterScript::start()
{
	RegisterFuncPtr();
	m_v3PrevDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP);
	m_ChStatus.iHp = m_ChStatus.iMaxHp;
	m_ChStatus.dCurUltGauge = 0;
	BattleStateReset();
}

void CBaseCharacterScript::tick()
{
	CGameObject* tile = GetOwner()->GetParent();
	if (tile == nullptr)
		return;
	CTileScript* tileScript = tile->GetScript<CTileScript>();
	if (tileScript == nullptr)
		return;
	TILE_TYPE eType = tileScript->GetType();
	switch (eType)
	{
	case TILE_TYPE::WAIT:
		WaitTileTick();
		break;
	case TILE_TYPE::BATTLE:
		BattleTileTick();
		break;
	}
}

void CBaseCharacterScript::BeginOverlap(CCollider* _Other)
{
	SetTarget(_Other);

}

void CBaseCharacterScript::OnOverlap(CCollider* _Other)
{
	SetTarget(_Other);
}

void CBaseCharacterScript::EndOverlap(CCollider* _Other)
{
	//m_pTarget = nullptr;
}

void CBaseCharacterScript::RegisterFuncPtr()
{
	CGameObject* pOwner = GetOwner();
	{
		pOwner->RegisterFucnPtrVOID(L"SendDamage", std::bind(&CBaseCharacterScript::SendDamage, this));
		pOwner->RegisterFucnPtrINT(L"SetAtkNumber", std::bind(&CBaseCharacterScript::SetAtkNumber, this, std::placeholders::_1));
	}
}

const bool& CBaseCharacterScript::IsWait()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetBoolParam(L"Battle");
	return !flag;
}

const bool& CBaseCharacterScript::IsUlt()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetBoolParam(L"ULT");
	return flag;
}

const bool& CBaseCharacterScript::IsMove()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetBoolParam(L"Move");
	return flag;
}

const bool& CBaseCharacterScript::IsAttack()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetBoolParam(L"Attack");
	return flag;
}

const bool& CBaseCharacterScript::IsDance()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetTriggerParam(L"Dance");
	return flag;
}

const bool& CBaseCharacterScript::IsDeath()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetBoolParam(L"Death");
	return flag;
}

const bool& CBaseCharacterScript::IsEnd()
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	const bool& flag = pAnimator->GetBoolParam(L"End");
	return flag;
}

int CBaseCharacterScript::GetAtkNumber()
{
	return 0;
}



void CBaseCharacterScript::WaitTileTick()
{
	CTileScript* pTileScript = GetOwner()->GetParent()->GetScript<CTileScript>();

	const TILE_TYPE& eType = pTileScript->GetType();
	if (eType != TILE_TYPE::WAIT)
		return;
	const int& iNumber = pTileScript->GetNumber();
	SetStartTileNum(iNumber);
}

void CBaseCharacterScript::BattleTileTick()
{
	CTileScript* pTileScript = GetOwner()->GetParent()->GetScript<CTileScript>();

	const TILE_TYPE& eType = pTileScript->GetType();
	if (eType != TILE_TYPE::BATTLE)
		return;

	CGameObject* pPlayer = GetPlayer();
	if (pPlayer == nullptr)
		return;
	UINT iGameState = CGameMgr::GetInst()->GetState(pPlayer);

	switch (iGameState)
	{
	case 0://select
		break;
	case 1://battle
		Battle(pTileScript->GetOwner());
		break;
	case 2://item
		break;
	}

	UpdateCharacterUI();
}

void CBaseCharacterScript::UltGaugeUp()
{
	m_ChStatus.dCurUltGauge += m_ChStatus.dAddUltGauge;
}

void CBaseCharacterScript::UltGaugeDown()
{
	m_ChStatus.dCurUltGauge -= m_ChStatus.dAddUltGauge;
}



void CBaseCharacterScript::SetTarget(CCollider* _Other)
{
	/*CTransform* otherTF = _Other->Transform();
	if (m_pTarget == nullptr)
		m_pTarget = otherTF;
	else
	{
		Vec3 targetPos = m_pTarget->GetRelativePos();
		Vec3 otherPos = otherTF->GetRelativePos();
		Vec3 thisPos = Transform()->GetRelativePos();

		Vec2 targetDiff = {};
		targetDiff.x = fabs(thisPos.x - targetPos.x);
		targetDiff.y = fabs(thisPos.z - targetPos.z);

		Vec2 otherDiff = {};
		otherDiff.x = fabs(thisPos.x - otherPos.x);
		otherDiff.y = fabs(thisPos.z - otherPos.z);

		double tDiff = targetDiff.x + targetDiff.y;
		double oDiff = otherDiff.x + otherDiff.y;

		if (tDiff > oDiff)
		{
			m_pTarget = otherTF;
		}
	}*/
}

void CBaseCharacterScript::SendDamage()
{
	if (m_iAtkTargetNum < 0)
		return;
	CGameObject* Target = CTileMgr::GetInst()->GetItem(m_iAtkTargetNum);
	if (Target == nullptr)
		return;
	CBaseCharacterScript* pCharacterScript = Target->GetScript<CBaseCharacterScript>();
	if (pCharacterScript == nullptr)
		return;
	pCharacterScript->RecvDamage(10);
	if (IsUlt())
	{
		m_ChStatus.dCurUltGauge -= m_ChStatus.dMinusUltGauge;
		if (m_ChStatus.dCurUltGauge <= 0)
		{
			m_ChStatus.dCurUltGauge = 0;
			SetUlt(false);
		}
	}
	else
	{
		m_ChStatus.dCurUltGauge += m_ChStatus.dAddUltGauge;
		if (m_ChStatus.dCurUltGauge >= m_ChStatus.dMaxUltGauge)
		{
			m_ChStatus.dCurUltGauge = m_ChStatus.dMaxUltGauge;
			SetUlt(true);
		}
	}

}

void CBaseCharacterScript::RecvDamage(float _damage)
{
	m_ChStatus.iHp -= _damage;
	if (m_ChStatus.iHp <= 0)
	{
		//죽음 처리
		SetDeath(true);
		SetAtk(false);
	}
}

void CBaseCharacterScript::Death()
{
	SetDeath(false);
	SetMove(false);
	SetAtk(false);
	SetUlt(false);
	SetAtkNumber(0);
}

void CBaseCharacterScript::SetMove(bool _flag)
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	pAnimator->SetBoolParam(L"Move", _flag);
	return;
}

void CBaseCharacterScript::SetWait(bool _flag)
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	pAnimator->SetBoolParam(L"Battle", !_flag);
	return;
}

void CBaseCharacterScript::SetAtk(bool _flag)
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	pAnimator->SetBoolParam(L"Attack", _flag);
	return;
}

void CBaseCharacterScript::SetUlt(bool _flag)
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	pAnimator->SetBoolParam(L"ULT", _flag);
	return;
}

void CBaseCharacterScript::SetAtkNumber(int _iNumber)
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	pAnimator->SetIntParam(L"Attack_Number", _iNumber);
	return;
}

void CBaseCharacterScript::SetDeath(bool _flag)
{
	CAnimator3D* pAnimator = GetOwner()->Animator3D();
	pAnimator->SetBoolParam(L"Death", _flag);
	return;
}

void CBaseCharacterScript::CurStartTile()
{
	CTileScript* pTileScript = GetOwner()->GetParent()->GetScript<CTileScript>();
	if (pTileScript == nullptr)
		return;
	CPlayerScript* pPlayerScript = GetPlayer()->GetScript<CPlayerScript>();
	if (pPlayerScript->GetPlayerType() == PLAYER_TYPE::AI)
		return;
	int iNumber = pTileScript->GetNumber();
	SetStartTileNum(iNumber);
}

void CBaseCharacterScript::Reset()
{
	CGameObject* pOwner = GetOwner();
	CPlayerScript* pPlayerScript = GetPlayer()->GetScript<CPlayerScript>();
	if (pPlayerScript->GetPlayerType() != PLAYER_TYPE::AI)
		CTileMgr::GetInst()->RegisterItem(GetStartTileNum(), pOwner);
	GetOwner()->Transform()->SetRelativeRot(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(180));
	GetOwner()->Transform()->SetRelativePos(0.f, 0.f, 0.f);
}

void CBaseCharacterScript::BattleStateReset()
{
	m_bMove = false;
	SetAtk(false);
	SetMove(false);
	SetUlt(false);
	CGameObject* pMeshParentObj = GetOwner()->GetChild(L"MeshParent");
	if (pMeshParentObj)
	{
		pMeshParentObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
		pMeshParentObj->Transform()->CancelLerpRot();
	}
	m_iAtkTargetNum = -1;

}


void CBaseCharacterScript::ChangeTransInfo()
{
	TILE_OWNER_TYPE eTileType = CTileMgr::GetInst()->GetTileOwnerType(m_iMoveTargetNum);
	PLAYER_TYPE ePlayerType = GetPlayer()->GetScript<CPlayerScript>()->GetPlayerType();
	switch (eTileType)
	{
	case TILE_OWNER_TYPE::ENEMY:
		switch (ePlayerType)
		{
		case PLAYER_TYPE::CLIENT:
			GetOwner()->Transform()->SetRelativeRot(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(0));
			break;
		case PLAYER_TYPE::ANOTHER:
		case PLAYER_TYPE::AI:
			GetOwner()->Transform()->SetRelativeRot(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(180));
			break;
		}
		break;
	case TILE_OWNER_TYPE::PLAYER:
		switch (ePlayerType)
		{
		case PLAYER_TYPE::CLIENT:
			GetOwner()->Transform()->SetRelativeRot(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(180));
			break;
		case PLAYER_TYPE::ANOTHER:
		case PLAYER_TYPE::AI:
			GetOwner()->Transform()->SetRelativeRot(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(0));
			break;
		}
		break;
	}
}

void CBaseCharacterScript::UpdateCharacterUI()
{
	CGameObject* pHPGauge = GetOwner()->GetChild(L"HPBar");
	if (pHPGauge)
	{
		GAUGE_TYPE eGaugeType = GAUGE_TYPE::RIGHT;
		float fCur = (float)m_ChStatus.iHp;
		float fMax = (float)m_ChStatus.iMaxHp;
		pHPGauge->GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &eGaugeType);
		pHPGauge->GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fCur);
		pHPGauge->GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &fMax);
	}
	CGameObject* pMPGauge = GetOwner()->GetChild(L"MPBar");
	if (pMPGauge)
	{
		GAUGE_TYPE eGaugeType = GAUGE_TYPE::RIGHT;
		float fCur = (float)m_ChStatus.dCurUltGauge;
		float fMax = (float)m_ChStatus.dMaxUltGauge;
		pMPGauge->GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &eGaugeType);
		pMPGauge->GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fCur);
		pMPGauge->GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &fMax);
	}

}

void CBaseCharacterScript::LookAtTarget(CGameObject* _pTarget)
{
	CGameObject* pMeshParentObj = GetOwner()->GetChild(L"MeshParent");
	if (pMeshParentObj->Transform()->GetLerpFlag())
		return;
	CGameObject* pPlayer = GetPlayer();
	bool bClientFlag = CGameMgr::GetInst()->IsSamePlayer(pPlayer);
	int BattleOffset = CTileMgr::GetInst()->GetWaitCount().x;
	int iCurNumber = GetOwner()->GetParent()->GetScript<CTileScript>()->GetNumber() - BattleOffset;
	int iTargetNumber = m_iAtkTargetNum - BattleOffset;
	if (iTargetNumber < 0)
		return;
	Vec3 TargetPos = _pTarget->Transform()->GetWorldPos();//CTileMgr::GetInst()->GetBattleTileWorldPos(iTargetNumber);
	Vec3 CurPos = Transform()->GetWorldPos();

	Vec3 v3Dir = {};
	v3Dir.x = TargetPos.x - CurPos.x;
	v3Dir.z = (TargetPos.z - CurPos.z);
	v3Dir.z *= bClientFlag == true ? 1 : -1;
	v3Dir.Normalize();

	Vec3 PrevDir = Transform()->GetRelativeDir(DIR_TYPE::UP);//Vec3(Transform()->GetRelativeDir(DIR_TYPE::RIGHT).x, 0.f, Transform()->GetRelativeDir(DIR_TYPE::FRONT).z);
	Vec3 NextDir = Vec3(v3Dir.x, 0.f, v3Dir.z);
	float fRadian = TransformFunc::RotationGetRadian(PrevDir, NextDir);
	bool bSign = PrevDir.Cross(NextDir).y < 0 ? true : false;
	if (bClientFlag && bSign)
		fRadian *= -1.f;
	else if (bClientFlag == false && bSign == false)
		fRadian *= -1.f;
	Vec3 v3Rotation = pMeshParentObj->Transform()->GetRelativeRot();
	v3Rotation.y = fRadian;
	pMeshParentObj->Transform()->RequestLerpRot(v3Rotation);

	m_v3PrevDir = v3Dir;
}

void CBaseCharacterScript::Battle(CGameObject* _pTileObj)
{
	CTileScript* pTileScript = _pTileObj->GetScript<CTileScript>();
	if (pTileScript == nullptr)
		return;

	int startNumber = pTileScript->GetNumber();
	int distance = 0;
	CGameObject* pPlayer = GetPlayer();
	int endNumber = CAStarMgr::GetInst()->SearchTarget(startNumber, distance, pPlayer);
	const TILE_OWNER_TYPE& eTileType = pTileScript->GetOwnerType();
	Vec2 dir = m_v2Dir;
	if (eTileType == TILE_OWNER_TYPE::PLAYER)
		dir.x *= -1;
	else
		dir.y *= -1;

	//적이 없다면 Battle Idle 상태로 변경.
	if (endNumber == -1)
	{
		SetMove(false);
		return;
	}
	m_iAtkTargetNum = endNumber;
	m_pTargetObj = CTileMgr::GetInst()->GetItem(m_iAtkTargetNum);
	bool bRotLerpFlag = GetOwner()->GetChild(L"MeshParent")->Transform()->GetLerpFlag();
	//적이 있고 사거리내에 도착했다면 Atk 상태로 변경
	if (m_ChStatus.fAttackRange >= distance && m_bMove == false)
	{
		LookAtTarget(m_pTargetObj);
		//attack
		//방향은 end 방향을 바라보기
		if (IsDeath() == false && IsAttack() == false)
		{
			SetAtk(true);
			SetMove(false);
		}
	}
	else
	{
		SetAtk(false);
		int BattleOffset = CTileMgr::GetInst()->GetWaitCount().x;
		bool bClientFlag = CGameMgr::GetInst()->IsSamePlayer(pPlayer);
		//test를 위해 항상 검사 - 경로 타일 색깔 바뀌게 하기 위함
		vector<int> Route = CAStarMgr::GetInst()->GetNextNodeAStar(startNumber, endNumber);
		//if (bClientFlag)
		//CTileMgr::GetInst()->BattleRouteRender(m_iMoveTargetNum - BattleOffset);
		//move
		//방향은 다음 타일을 바라보고 움직이기.
		//목표 지점까지 이동이 완료 될 때까지는 검사 X
		if (m_bMove)
		{
			Vec3 WorldPos = GetOwner()->Transform()->GetWorldPos();
			Vec2 diff = Vec2(abs(WorldPos.x - m_v3TargetPos.x), abs(WorldPos.z - m_v3TargetPos.z));
			Vec3 pos = GetOwner()->Transform()->GetRelativePos();

			if ((diff.x <= abs(m_v2Dir.x) || m_v2Dir.x == 0)
				&& (diff.y <= abs(m_v2Dir.y) || m_v2Dir.y == 0))
			{
				m_bMove = false;
				ChangeTransInfo();
				CAStarMgr::GetInst()->CancelReserve(m_iMoveTargetNum - BattleOffset);
				CTileMgr::GetInst()->RegisterItem(m_iMoveTargetNum, GetOwner());
				pos = Vec3{ 0.f,0.f,pos.z };
			}
			else if (diff.x <= abs(m_v2Dir.x) && m_v2Dir.y != 0)
			{
				WorldPos.y += dir.y;
			}
			else if (diff.y <= abs(m_v2Dir.y) && m_v2Dir.x != 0)
			{
				WorldPos.x += dir.x;
			}
			else
			{
				WorldPos.x += dir.x;
				WorldPos.y += dir.y;
			}
			Matrix worldInvMat = GetOwner()->Transform()->GetWorldInvMat();
			Vec3 localPos = XMVector3TransformCoord(WorldPos, worldInvMat);
			GetOwner()->Transform()->SetRelativePos(pos + localPos);
		}
		else
		{
			int size = Route.size();
			int index = size - 2;
			if (index < 0)
				return;
			int nextTile = Route[index];
			int curTile = GetOwner()->GetParent()->GetScript<CTileScript>()->GetNumber() - BattleOffset;

			//다음 tile의 transform target으로 지정.
			//현재 위치에서 다음 타일 방향으로 현재 오브젝트 회전
			m_iMoveTargetNum = nextTile + BattleOffset;
			CAStarMgr::GetInst()->Reserve(m_iMoveTargetNum - BattleOffset);
			m_v3TargetPos = CTileMgr::GetInst()->GetBattleTileWorldPos(nextTile);
			Vec3 CurPos = CTileMgr::GetInst()->GetBattleTileWorldPos(curTile);


			m_v2Dir.x = m_v3TargetPos.x - CurPos.x;
			m_v2Dir.y = -(m_v3TargetPos.z - CurPos.z);
			m_v2Dir.x *= bClientFlag == true ? 1 : -1;
			//m_v2Dir.y *= bClientFlag == true ? 1 : -1;
			m_v2Dir.Normalize();
			m_bMove = true;
			SetMove(true);
			CGameObject* pNextTile = CTileMgr::GetInst()->GetTile(nextTile + BattleOffset)->GetOwner();
			LookAtTarget(pNextTile);
		}
	}
}
