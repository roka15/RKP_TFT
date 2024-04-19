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
	m_bMove(false)
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
	CItem(_eType)
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
	m_bMove(false)
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
	CGameObject* Target = CTileMgr::GetInst()->GetItem(m_iAtkTargetNum);
	if (Target == nullptr)
		return;
	CBaseCharacterScript* pCharacterScript = Target->GetScript<CBaseCharacterScript>();
	if (pCharacterScript == nullptr)
		return;
	pCharacterScript->RecvDamage(10);
	if (m_ChState.bUlt)
	{
		m_ChStatus.dCurUltGauge -= m_ChStatus.dMinusUltGauge;
		if (m_ChStatus.dCurUltGauge <= 0)
		{
			m_ChStatus.dCurUltGauge = 0;
			m_ChState.bUlt = false;
			m_ChState.bWaiting = false;
		}
	}
	else
	{
		m_ChStatus.dCurUltGauge += m_ChStatus.dAddUltGauge;
		if (m_ChStatus.dCurUltGauge >= m_ChStatus.dMaxUltGauge)
		{
			m_ChStatus.dCurUltGauge = m_ChStatus.dMaxUltGauge;
			m_ChState.bUlt = true;
			m_ChState.bWaiting = false;
		}
	}

}

void CBaseCharacterScript::RecvDamage(float _damage)
{
	m_ChStatus.iHp -= _damage;
	if (m_ChStatus.iHp <= 0)
	{
		//죽음 처리
		m_ChState.bDeath = true;
	}
}

void CBaseCharacterScript::Death()
{
	m_ChState.bDeath = false;
	m_ChState.bMove = false;
	m_ChState.bAttack = false;
	m_ChState.bUlt = false;
	m_ChState.iAtkNum = 0;
}

void CBaseCharacterScript::CurStartTile()
{
	CTileScript* pTileScript = GetOwner()->GetParent()->GetScript<CTileScript>();
	if (pTileScript == nullptr)
		return;
	int iNumber = pTileScript->GetNumber();
	SetStartTileNum(iNumber);
}

void CBaseCharacterScript::Reset()
{
	CGameObject* pOwner = GetOwner();
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
	//적이 있고 사거리내에 도착했다면 Atk 상태로 변경
	if (m_ChStatus.fAttackRange >= distance && m_bMove == false)
	{
		//attack
		//방향은 end 방향을 바라보기
		SetMove(false);
		SetAtk(true);
	}
	else
	{
		SetAtk(false);
		int BattleOffset = CTileMgr::GetInst()->GetWaitCount().x;
		bool bClientFlag = CGameMgr::GetInst()->IsSamePlayer(pPlayer);
		//test를 위해 항상 검사 - 경로 타일 색깔 바뀌게 하기 위함
		vector<int> Route = CAStarMgr::GetInst()->GetNextNodeAStar(startNumber, endNumber);
		//if (bClientFlag)
		CTileMgr::GetInst()->BattleRouteRender(m_iMoveTargetNum - BattleOffset);
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
		}
	}
}
