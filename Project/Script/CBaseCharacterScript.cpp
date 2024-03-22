#include "pch.h"
#include "CTileMgr.h"
#include "CAStarMgr.h"
#include "CBaseCharacterScript.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
#include "CTileScript.h"
#include  "CGameMgr.h"



CBaseCharacterScript::CBaseCharacterScript() :
	CItem(SCRIPT_TYPE::BASECHARACTERSCRIPT),
	m_ChState{ true,false,false,false,false,false },
	m_bMove(false)
{
	m_ChStatus.fAttackRange = 1;
	SetCost(10);
}

CBaseCharacterScript::CBaseCharacterScript(SCRIPT_TYPE _eType) :
	CItem(_eType)
{
}

CBaseCharacterScript::~CBaseCharacterScript()
{
}


void CBaseCharacterScript::tick()
{
	//1번키 : waiting on/off
	//2번키 : move    on/off
	//3번키 : attack  on/off
	//4번키 : ult     on/off
	//5번키 : dance   on/off
	/*KEY eInput = KEY::END;

	if (CKeyMgr::GetInst()->GetKeyState(KEY::_1) == KEY_STATE::TAP)
	{
		if (m_ChState.bAttack == false && m_ChState.bDance == false && m_ChState.bMove == false && m_ChState.bUlt == false)
		{
			m_ChState.bWaiting = !m_ChState.bWaiting;
		}
		eInput = KEY::_1;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_2) == KEY_STATE::TAP)
	{
		if (m_ChState.bWaiting == false)
		{
			m_ChState.bMove = !m_ChState.bMove;
		}
		eInput = KEY::_2;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_3) == KEY_STATE::TAP)
	{
		m_ChState.bAttack = !m_ChState.bAttack;
		eInput = KEY::_3;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_4) == KEY_STATE::TAP)
	{
		if (m_ChState.bMove == false)
		{
			m_ChState.bUlt = !m_ChState.bUlt;
		}
		eInput = KEY::_4;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::_5) == KEY_STATE::TAP)
	{
		m_ChState.bDance = !m_ChState.bDance;
		eInput = KEY::_5;
	}

	switch (eInput)
	{
	case KEY::_1:
		Wait();
		break;
	case KEY::_2:
		Move();
		break;
	case KEY::_3:
		Attack();
		break;
	case KEY::_4:
		Move();
		Attack();
		break;
	case KEY::_5:
		Dance();
		break;
	}*/

	CGameObject* tile = GetOwner()->GetParent();
	if (tile == nullptr)
		return;
	CTileScript* tileScript = tile->GetScript<CTileScript>();
	if (tileScript == nullptr)
		return;
	if (tileScript->GetType() != TILE_TYPE::BATTLE)
		return;
	int startNumber = tileScript->GetNumber();
	int distance = 0;
	int endNumber = CAStarMgr::GetInst()->SearchTarget(startNumber, distance);
	if (endNumber == -1)
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
		if (m_ChStatus.fAttackRange == distance)
		{
			//attack
			//방향은 end 방향을 바라보기
		}
		else
		{
			//test를 위해 항상 검사 - 경로 타일 색깔 바뀌게 하기 위함
			vector<int> Route = CAStarMgr::GetInst()->GetNextNodeAStar(startNumber, endNumber);
			CTileMgr::GetInst()->BattleRouteRender(Route);
			//move
			//방향은 다음 타일을 바라보고 움직이기.
			//목표 지점까지 이동이 완료 될 때까지는 검사 X
			int BattleOffset = CTileMgr::GetInst()->GetWaitCount().x;
			if (m_bMove)
			{
				Vec3 WorldPos = GetOwner()->Transform()->GetWorldPos();
				Vec2 diff = Vec2(abs(WorldPos.x - m_v3TargetPos.x), abs(WorldPos.z - m_v3TargetPos.z));
				Vec3 pos = GetOwner()->Transform()->GetRelativePos();
				if (diff.x <= abs(m_v2Dir.x) && diff.y <= abs(m_v2Dir.y))
				{
					m_bMove = false;
					CTileMgr::GetInst()->RegisterItem(m_iTargetNum, GetOwner());
					pos = Vec3{ 0.f,0.f,pos.z };
				}
				else if (diff.x <= abs(m_v2Dir.x))
				{
					pos.y += m_v2Dir.y;
				}
				else if (diff.y <= abs(m_v2Dir.y))
				{
					pos.x += m_v2Dir.x;
				}
				else
				{
					pos.x += m_v2Dir.x;
					pos.y += m_v2Dir.y;
				}

				GetOwner()->Transform()->SetRelativePos(pos);
			}
			else
			{
				/*vector<int> Route = CAStarMgr::GetInst()->GetNextNodeAStar(startNumber, endNumber);
				CTileMgr::GetInst()->BattleRouteRender(Route);*/
				int size = Route.size();
				int nextTile = Route[size - 2];
				int curTile = GetOwner()->GetParent()->GetScript<CTileScript>()->GetNumber() - BattleOffset;
			
				//다음 tile의 transform target으로 지정.
				//현재 위치에서 다음 타일 방향으로 현재 오브젝트 회전
				m_iTargetNum = nextTile + BattleOffset;
				m_v3TargetPos = CTileMgr::GetInst()->GetBattleTileWorldPos(nextTile);
				Vec3 CurPos = CTileMgr::GetInst()->GetBattleTileWorldPos(curTile);
				m_v2Dir.x = m_v3TargetPos.x - CurPos.x;
				m_v2Dir.y = -(m_v3TargetPos.z - CurPos.z);
				m_v2Dir.Normalize();
				m_bMove = true;
			}
		}
		break;
	case 2://item
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

bool CBaseCharacterScript::IsWithinAttackRange()
{
	//if (m_pTarget == nullptr)
	//	return false;
	//Vec3 position = m_pTarget->GetRelativePos();

	return false;
}

void CBaseCharacterScript::UltGaugeUp()
{
	m_ChStatus.dCurUltGauge += m_ChStatus.dAddUltGauge;
}

void CBaseCharacterScript::UltGaugeDown()
{
	m_ChStatus.dCurUltGauge -= m_ChStatus.dAddUltGauge;
}

void CBaseCharacterScript::Search()
{
}

void CBaseCharacterScript::Wait()
{
	if (m_ChState.bMove || m_ChState.bAttack || m_ChState.bDance || m_ChState.bUlt)
	{
		int a;
		return;
	}
	m_ChState.bUlt = false;
	m_ChState.bMove = false;
	m_ChState.bAttack = false;
	m_ChState.bDance = false;
	m_ChState.bEnd = false;

	if (m_ChState.bWaiting)
	{
	}
	else
	{
	}
}

void CBaseCharacterScript::Move()
{
	if (m_ChState.bMove == false)
		return;
	m_ChState.bWaiting = false;
	m_ChState.bAttack = false;
}

void CBaseCharacterScript::Attack()
{
	if (m_ChState.bAttack == false)
		return;
	m_ChState.bWaiting = false;
	m_ChState.bMove = false;

	if (m_ChState.bUlt)
		UltAttack();
	else
		NormalAttack();
}

void CBaseCharacterScript::NormalAttack()
{

}

void CBaseCharacterScript::UltAttack()
{
}

void CBaseCharacterScript::Dance()
{
	if (m_ChState.bDance == false)
		return;
	m_ChState.bAttack = false;
	m_ChState.bMove = false;
	m_ChState.bUlt = false;
	m_ChState.bWaiting = false;
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

