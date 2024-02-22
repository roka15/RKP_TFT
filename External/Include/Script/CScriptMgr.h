#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ATTROXIDLESCRIPT,
	ATTROXMACHINESCRIPT,
	CAMERAMOVESCRIPT,
	CHARACTERSCRIPT,
	CHARACTERSTATESCRIPT,
	CHARACTERTRIGGERSCRIPT,
	GRAVITYSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	PLAYERSCRIPT,
	STATEMACHINESCRIPT,
	STATESCRIPT,
	TESTSCRIPT,
	TRIGGERSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
