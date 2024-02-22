#include "pch.h"
#include "CCharacterStateScript.h"
#include "CStateScript.h"

void CCharacterStateScript::OnEntry(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
}

void CCharacterStateScript::OnExit(CStateMachineScript* _pSMachine, CStateScript* _pState)
{
}

CCharacterStateScript::CCharacterStateScript():CStateScript(CHARACTERSTATESCRIPT)
{
}

CCharacterStateScript::CCharacterStateScript(SCRIPT_TYPE _eType):CStateScript(_eType)
{
}

CCharacterStateScript::~CCharacterStateScript()
{
}
