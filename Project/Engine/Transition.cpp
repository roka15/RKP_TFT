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

int CTransition::Save(FILE* _pFile)
{
	wstring strName = GetName();
	int strLen = strName.size() * UNICODELEN;
	fwrite(&strLen, sizeof(int), 1, _pFile);
	fwrite(strName.c_str(), strLen, 1, _pFile);

	fwrite(&m_bHasExitTime, sizeof(bool), 1, _pFile);
	int loopSize = m_mapIntConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (auto itr = m_mapIntConditions.begin(); itr != m_mapIntConditions.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		int iValue = itr->second;
		fwrite(&strLen, sizeof(int), 1, _pFile);
		fwrite(strKey.c_str(), strLen, 1, _pFile);
		fwrite(&iValue, sizeof(int), 1, _pFile);
	}
	loopSize = m_mapFloatConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (auto itr = m_mapFloatConditions.begin(); itr != m_mapFloatConditions.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		float fValue = itr->second;
		fwrite(&strLen, sizeof(int), 1, _pFile);
		fwrite(strKey.c_str(), strLen, 1, _pFile);
		fwrite(&fValue, sizeof(float), 1, _pFile);
	}
	loopSize = m_mapBoolConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (auto itr = m_mapBoolConditions.begin(); itr != m_mapBoolConditions.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		bool bValue = itr->second;
		fwrite(&strLen, sizeof(int), 1, _pFile);
		fwrite(strKey.c_str(), strLen, 1, _pFile);
		fwrite(&bValue, sizeof(bool), 1, _pFile);
	}
	loopSize = m_mapTriggerConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (auto itr = m_mapTriggerConditions.begin(); itr != m_mapTriggerConditions.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		bool bValue = itr->second;
		fwrite(&strLen, sizeof(int), 1, _pFile);
		fwrite(strKey.c_str(), strLen, 1, _pFile);
		fwrite(&bValue, sizeof(bool), 1, _pFile);
	}
	loopSize = m_mapComparisonConditions.size();
	fwrite(&loopSize, sizeof(int), 1, _pFile);
	for (auto itr = m_mapComparisonConditions.begin(); itr != m_mapComparisonConditions.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		COMPARISON_TYPE eType = itr->second;
		fwrite(&strLen, sizeof(int), 1, _pFile);
		fwrite(strKey.c_str(), strLen, 1, _pFile);
		fwrite(&eType, sizeof(COMPARISON_TYPE), 1, _pFile);
	}
	wstring strConnectKey = m_pConnectNode->GetName();
	strLen = strConnectKey.size() * UNICODELEN;
	fwrite(&strLen, sizeof(int), 1, _pFile);
	fwrite(strConnectKey.c_str(), strLen, 1, _pFile);
	wstring strOwnerKey = m_pOwner->GetName();
	strLen = strOwnerKey.size() * UNICODELEN;
	fwrite(&strLen, sizeof(int), 1, _pFile);
	strLen = strOwnerKey.size() * UNICODELEN;
	fwrite(strOwnerKey.c_str(), strLen, 1, _pFile);
	return S_OK;
}

int CTransition::Load(FILE* _pFile)
{
	wchar_t strBuff[MAXLEN] = {};
	int strLen = 0;
	fread(&strLen, sizeof(int), 1, _pFile);
	fread(strBuff, strLen, 1, _pFile);
	strBuff[strLen + 1] = L'\0';
	SetName(strBuff);
	ZeroMemory(strBuff, MAXLEN);
	fread(&m_bHasExitTime, sizeof(bool), 1, _pFile);
	int loopSize = 0;
	fread(&loopSize, sizeof(int), 1, _pFile);

	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		fread(&strLen, sizeof(int), 1, _pFile);
		fread(strBuff, strLen, 1, _pFile);
		strBuff[strLen + 1] = '\0';
		wstring strKey = strBuff;
		int iValue = 0;
		fread(&iValue, sizeof(int), 1, _pFile);
		m_mapIntConditions.insert(std::make_pair(strKey,iValue));
	}
	fread(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		fread(&strLen, sizeof(int), 1, _pFile);
		fread(strBuff, strLen, 1, _pFile);
		strBuff[strLen + 1] = '\0';
		wstring strKey = strBuff;
		float fValue = 0;
		fread(&fValue, sizeof(int), 1, _pFile);
		m_mapFloatConditions.insert(std::make_pair(strKey, fValue));
	}
	fread(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		fread(&strLen, sizeof(int), 1, _pFile);
		fread(strBuff, strLen, 1, _pFile);
		strBuff[strLen + 1] = '\0';
		wstring strKey = strBuff;
		bool bValue = 0;
		fread(&bValue, sizeof(bool), 1, _pFile);
		m_mapBoolConditions.insert(std::make_pair(strKey, bValue));
	}
	fread(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		fread(&strLen, sizeof(int), 1, _pFile);
		fread(strBuff, strLen, 1, _pFile);
		strBuff[strLen + 1] = '\0';
		wstring strKey = strBuff;
		bool bValue = 0;
		fread(&bValue, sizeof(bool), 1, _pFile);
		m_mapTriggerConditions.insert(std::make_pair(strKey, bValue));
	}
	fread(&loopSize, sizeof(int), 1, _pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		fread(&strLen, sizeof(int), 1, _pFile);
		fread(strBuff, strLen, 1, _pFile);
		strBuff[strLen + 1] = '\0';
		wstring strKey = strBuff;
		COMPARISON_TYPE eType;
		fread(&eType, sizeof(COMPARISON_TYPE), 1, _pFile);
		m_mapComparisonConditions.insert(std::make_pair(strKey, eType));
	}
	ZeroMemory(strBuff, MAXLEN);
	fread(&strLen, sizeof(int), 1, _pFile);
	fread(strBuff, strLen, 1, _pFile);
	strBuff[strLen + 1] = '\0';
	m_strConnectNode = strBuff;

	ZeroMemory(strBuff, MAXLEN);
	fread(&strLen, sizeof(int), 1, _pFile);
	fread(strBuff, strLen, 1, _pFile);
	strBuff[strLen + 1] = '\0';
	m_strOwnerNode = strBuff;
	return S_OK;
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
		if (bActive == false)
			return false;
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
		if (bActive == false)
			return false;
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
		if (bActive == false)
			return false;
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
		if (bActive == false)
			return false;
	}

	for (auto itr = m_mapTriggerConditions.begin(); itr != m_mapTriggerConditions.end(); ++itr)
	{
		bFail = false;
		strKey = itr->first;
		bool bValue = _pController->SetTriggerParam(strKey, false);
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

bool CTransition::RegisterCurNode(Ptr<CAnimatorController> _pController)
{
	if (_pController->GetCurNode() == m_pConnectNode)
		return false;
	_pController->SetCurNode(m_pConnectNode);
	return true;
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


