#include "pch.h"
#include "Transition.h"
#include "AniNode.h"
#include "AnimatorController.h"
#include "ptr.h"

CTransition::CTransition():
	m_bHasExitTime(false)
{
}

CTransition::~CTransition()
{
}

bool CTransition::IsActive(Ptr<CAnimatorController> _pController)
{
	wstring strKey = {};
	bool bActive = false;
	bool bFail = false;
	for (auto itr = m_mapIntConditions.begin(); itr != m_mapIntConditions.end(); ++itr)
	{
		bFail = false;
		strKey = itr->first;
		int iCondition = itr->second;
		int iValue = _pController->GetIntParam(strKey, bFail);
		if (bFail)
		{
			//이곳에는 등록되어있는 param이 controller에 등록되지 않을 수 없기 때문에 이런 경우 에러 띄운다.
			assert(nullptr);
		}
		COMPARISON_TYPE tComparisonType = m_mapComparisonConditions.find(strKey)->second;
		
		bActive = ComparisonCalculator(iCondition, iValue, tComparisonType);
	}

	for (auto itr = m_mapFloatConditions.begin(); itr != m_mapFloatConditions.end(); ++itr)
	{
		bFail = false;
		strKey = itr->first;
		float fCondition = itr->second;
		float fValue = _pController->GetFloatParam(strKey, bFail);
		if (bFail)
		{
			assert(nullptr);
		}
		COMPARISON_TYPE tComparisonType = m_mapComparisonConditions.find(strKey)->second;

		bActive = ComparisonCalculator(fCondition, fValue, tComparisonType);
	}

	for (auto itr = m_mapBoolConditions.begin(); itr != m_mapBoolConditions.end(); ++itr)
	{
		bFail = false;
		strKey = itr->first;
		bool bCondition = itr->second;
		bool bValue = _pController->GetBoolParam(strKey, bFail);
		if (bFail)
		{
			assert(nullptr);
		}
		COMPARISON_TYPE tComparisonType = m_mapComparisonConditions.find(strKey)->second;

		bActive = ComparisonCalculator(bCondition, bValue, tComparisonType);
	}

	for (auto itr = m_mapTriggerConditions.begin(); itr != m_mapTriggerConditions.end(); ++itr)
	{
		bFail = false;
		strKey = itr->first;
		bool bCondition = itr->second;
		bool bValue = _pController->GetTriggerParam(strKey, bFail);
		if (bFail)
		{
			assert(nullptr);
		}
		COMPARISON_TYPE tComparisonType = m_mapComparisonConditions.find(strKey)->second;

		bActive = ComparisonCalculator(bCondition, bValue, tComparisonType);
	}

	return bActive;
}

void CTransition::RegisterCondition(wstring _Key, int _iValue, COMPARISON_TYPE _tComparison)
{
	m_mapIntConditions.insert(std::make_pair(_Key, _iValue));
	m_mapComparisonConditions.insert(std::make_pair(_Key, _tComparison));
}

void CTransition::RegisterCondition(wstring _Key, float _fValue, COMPARISON_TYPE _tComparison)
{
	m_mapFloatConditions.insert(std::make_pair(_Key, _fValue));
	m_mapComparisonConditions.insert(std::make_pair(_Key, _tComparison));
}

void CTransition::RegisterCondition(wstring _Key, bool _bValue, bool _bTrigger, COMPARISON_TYPE _tComparison)
{
	if (_bTrigger)
		m_mapTriggerConditions.insert(std::make_pair(_Key, _bValue));
	else
		m_mapBoolConditions.insert(std::make_pair(_Key, _bValue));

	m_mapComparisonConditions.insert(std::make_pair(_Key, _tComparison));
}

void CTransition::RegisterCurNode(Ptr<CAnimatorController> _pController)
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


