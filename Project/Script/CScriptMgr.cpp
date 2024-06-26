#include "pch.h"
#include "CScriptMgr.h"

#include "CAttroxMachineScript.h"
#include "CBaseCharacterScript.h"
#include "CCameraMoveScript.h"
#include "CCard.h"
#include "CChMinionScript.h"
#include "CCursor.h"
#include "CGravityScript.h"
#include "CItem.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CStateMachineScript.h"
#include "CTestScript.h"
#include "CTileScript.h"
#include "CZedMachineScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttroxMachineScript");
	_vec.push_back(L"CBaseCharacterScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCard");
	_vec.push_back(L"CChMinionScript");
	_vec.push_back(L"CCursor");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CItem");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CStateMachineScript");
	_vec.push_back(L"CTestScript");
	_vec.push_back(L"CTileScript");
	_vec.push_back(L"CZedMachineScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttroxMachineScript" == _strScriptName)
		return new CAttroxMachineScript;
	if (L"CBaseCharacterScript" == _strScriptName)
		return new CBaseCharacterScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCard" == _strScriptName)
		return new CCard;
	if (L"CChMinionScript" == _strScriptName)
		return new CChMinionScript;
	if (L"CCursor" == _strScriptName)
		return new CCursor;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CItem" == _strScriptName)
		return new CItem;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CStateMachineScript" == _strScriptName)
		return new CStateMachineScript;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	if (L"CTileScript" == _strScriptName)
		return new CTileScript;
	if (L"CZedMachineScript" == _strScriptName)
		return new CZedMachineScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTROXMACHINESCRIPT:
		return new CAttroxMachineScript;
		break;
	case (UINT)SCRIPT_TYPE::BASECHARACTERSCRIPT:
		return new CBaseCharacterScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CARD:
		return new CCard;
		break;
	case (UINT)SCRIPT_TYPE::CHMINIONSCRIPT:
		return new CChMinionScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSOR:
		return new CCursor;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEM:
		return new CItem;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::STATEMACHINESCRIPT:
		return new CStateMachineScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	case (UINT)SCRIPT_TYPE::TILESCRIPT:
		return new CTileScript;
		break;
	case (UINT)SCRIPT_TYPE::ZEDMACHINESCRIPT:
		return new CZedMachineScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTROXMACHINESCRIPT:
		return L"CAttroxMachineScript";
		break;

	case SCRIPT_TYPE::BASECHARACTERSCRIPT:
		return L"CBaseCharacterScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CARD:
		return L"CCard";
		break;

	case SCRIPT_TYPE::CHMINIONSCRIPT:
		return L"CChMinionScript";
		break;

	case SCRIPT_TYPE::CURSOR:
		return L"CCursor";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::ITEM:
		return L"CItem";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::STATEMACHINESCRIPT:
		return L"CStateMachineScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	case SCRIPT_TYPE::TILESCRIPT:
		return L"CTileScript";
		break;

	case SCRIPT_TYPE::ZEDMACHINESCRIPT:
		return L"CZedMachineScript";
		break;

	}
	return nullptr;
}