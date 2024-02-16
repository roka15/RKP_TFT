#include "pch.h"
#include "Transition.h"

bool CTransition::IsActive(CAnimatorController* _pController)
{
    return false;
}

void CTransition::RegisterCondition(wstring _Key, int _iValue)
{
}

void CTransition::RegisterCondition(wstring _Key, float _fValue)
{
}

void CTransition::RegisterCondition(wstring _Key, bool _bValue, bool _bTrigger)
{
}

CTransition::CTransition()
{
}

CTransition::~CTransition()
{
}
