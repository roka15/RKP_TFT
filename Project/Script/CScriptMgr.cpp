#include "pch.h"
#include "CScriptMgr.h"

#include "CAttroxIdleScript.h"
#include "CAttroxMachineScript.h"
#include "CCameraMoveScript.h"
#include "CCharacterScript.h"
#include "CCharacterStateScript.h"
#include "CCharacterTriggerScript.h"
#include "CGravityScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CStateMachineScript.h"
#include "CStateScript.h"
#include "CTestScript.h"
#include "CTriggerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttroxIdleScript");
	_vec.push_back(L"CAttroxMachineScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCharacterScript");
	_vec.push_back(L"CCharacterStateScript");
	_vec.push_back(L"CCharacterTriggerScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CStateMachineScript");
	_vec.push_back(L"CStateScript");
	_vec.push_back(L"CTestScript");
	_vec.push_back(L"CTriggerScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttroxIdleScript" == _strScriptName)
		return new CAttroxIdleScript;
	if (L"CAttroxMachineScript" == _strScriptName)
		return new CAttroxMachineScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCharacterScript" == _strScriptName)
		return new CCharacterScript;
	if (L"CCharacterStateScript" == _strScriptName)
		return new CCharacterStateScript;
	if (L"CCharacterTriggerScript" == _strScriptName)
		return new CCharacterTriggerScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CStateMachineScript" == _strScriptName)
		return new CStateMachineScript;
	if (L"CStateScript" == _strScriptName)
		return new CStateScript;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	if (L"CTriggerScript" == _strScriptName)
		return new CTriggerScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTROXIDLESCRIPT:
		return new CAttroxIdleScript;
		break;
	case (UINT)SCRIPT_TYPE::ATTROXMACHINESCRIPT:
		return new CAttroxMachineScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERSCRIPT:
		return new CCharacterScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERSTATESCRIPT:
		return new CCharacterStateScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERTRIGGERSCRIPT:
		return new CCharacterTriggerScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
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
	case (UINT)SCRIPT_TYPE::STATESCRIPT:
		return new CStateScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	case (UINT)SCRIPT_TYPE::TRIGGERSCRIPT:
		return new CTriggerScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTROXIDLESCRIPT:
		return L"CAttroxIdleScript";
		break;

	case SCRIPT_TYPE::ATTROXMACHINESCRIPT:
		return L"CAttroxMachineScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHARACTERSCRIPT:
		return L"CCharacterScript";
		break;

	case SCRIPT_TYPE::CHARACTERSTATESCRIPT:
		return L"CCharacterStateScript";
		break;

	case SCRIPT_TYPE::CHARACTERTRIGGERSCRIPT:
		return L"CCharacterTriggerScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
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

	case SCRIPT_TYPE::STATESCRIPT:
		return L"CStateScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	case SCRIPT_TYPE::TRIGGERSCRIPT:
		return L"CTriggerScript";
		break;

	}
	return nullptr;
}