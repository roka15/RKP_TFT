#include "pch.h"
#include "CBaseCharacterScript.h"
#include "CAttroxMachineScript.h"
#include "CCharacterTrigger.h"
CBaseCharacterScript::CBaseCharacterScript() :
	CScript((UINT)SCRIPT_TYPE::BASECHARACTERSCRIPT),
	m_ChState{ true,false,false,false,false,false }
{
}

CBaseCharacterScript::CBaseCharacterScript(SCRIPT_TYPE _eType) :
	CScript((UINT)_eType)
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
	KEY eInput = KEY::END;

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
	m_pTarget = nullptr;
}

bool CBaseCharacterScript::IsWithinAttackRange()
{
	if (m_pTarget == nullptr)
		return false;
	Vec3 position = m_pTarget->GetRelativePos();

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
	CTransform* otherTF = _Other->Transform();
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
	}
}

