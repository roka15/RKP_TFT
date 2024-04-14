#include "pch.h"
#include "CChMinionScript.h"
#include "CGameMgr.h"
#include "CPlayerScript.h"

void CChMinionScript::start()
{
	CBaseCharacterScript::start();
	CGameObject* pOwner = GetOwner();
	pOwner->RegisterFucnPtrVOID(L"Death", std::bind(&CBaseCharacterScript::Death, this));
}

void CChMinionScript::Death()
{
	CBaseCharacterScript::Death();
	CPlayerScript* pPlayer = GetPlayer()->GetScript<CPlayerScript>();
	CGameMgr::GetInst()->DeathMinion(pPlayer->GetGameID(), GetOwner());
}

CChMinionScript::CChMinionScript() : CBaseCharacterScript(SCRIPT_TYPE::CHMINIONSCRIPT)
{
}

CChMinionScript::CChMinionScript(SCRIPT_TYPE _eType) : CBaseCharacterScript(_eType)
{
}

CChMinionScript::CChMinionScript(const CChMinionScript& _ref) : CBaseCharacterScript(_ref)
{
}

CChMinionScript::~CChMinionScript()
{
}
