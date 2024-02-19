#include "pch.h"
#include "Transition.h"
#include "AniNode.h"
#include "AnimatorController.h"

bool CTransition::IsActive()
{
    return true;
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

void CTransition::RegisterCurNode(CAnimatorController* _pController)
{
    _pController->SetCurNode(m_pConnectNode);
}

void CTransition::SetConnectNode(CAniNode* _pConnectNode)
{
    if (m_pConnectNode != nullptr)
    {
        if (_pConnectNode == nullptr)
        {
            m_pConnectNode->RemoveInTransition(this);
        }
    }
    m_pConnectNode = _pConnectNode; 
}

CTransition::CTransition()
{
}

CTransition::~CTransition()
{
}
