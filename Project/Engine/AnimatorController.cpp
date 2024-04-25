#include "pch.h"
#include "AnimatorController.h"

#include "CPathMgr.h"
#include "CResMgr.h"
#include "CAnimator3D.h"
#include "AniNode.h"
#include "Transition.h"

CAnimatorController::CAnimatorController() :CRes(RES_TYPE::ANICONTROLLER, true)
{
}

CAnimatorController::~CAnimatorController()
{
	for (auto itr = m_mapNode.begin(); itr != m_mapNode.end(); ++itr)
	{
		delete itr->second;
		itr->second = nullptr;
	}
	m_mapNode.clear();


	for (auto itr = m_mapIDTransition.begin(); itr != m_mapIDTransition.end(); ++itr)
	{
		delete itr->second;
		itr->second = nullptr;
	}
	m_mapNode.clear();

	//mapNode에 있는 AniNode* 는 mapIDNode에 있는것과 동일해서 둘 중 하나의 map은 delete 안해줘도 된다.
	m_mapIDNode.clear();
}

int CAnimatorController::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);
	PARAM_TYPE eType;
	int loopSize = 0;
	fread(&eType, sizeof(UINT), 1, pFile);
	fread(&loopSize, sizeof(int), 1, pFile);
	wchar_t strBuff[MAXLEN] = {};
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		wstring strKey;
		int strLen = 0;
		int iValue = 0;
		fread(&strLen, sizeof(int), 1, pFile);
		fread(strBuff, strLen, 1, pFile);
		strBuff[strLen + 1] = '\0';
		strKey = strBuff;
		fread(&iValue, sizeof(int), 1, pFile);
		m_mapIntParams.insert(std::make_pair(strKey, iValue));
	}
	fread(&eType, sizeof(UINT), 1, pFile);
	fread(&loopSize, sizeof(int), 1, pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		wstring strKey;
		int strLen = 0;
		float fValue = 0;
		fread(&strLen, sizeof(int), 1, pFile);
		fread(strBuff, strLen, 1, pFile);
		strBuff[strLen + 1] = '\0';
		strKey = strBuff;
		fread(&fValue, sizeof(int), 1, pFile);
		m_mapFloatParams.insert(std::make_pair(strKey, fValue));
	}
	fread(&eType, sizeof(UINT), 1, pFile);
	fread(&loopSize, sizeof(int), 1, pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		wstring strKey;
		int strLen = 0;
		bool bValue = 0;
		fread(&strLen, sizeof(int), 1, pFile);
		fread(strBuff, strLen, 1, pFile);
		strBuff[strLen + 1] = '\0';
		strKey = strBuff;
		fread(&bValue, sizeof(bool), 1, pFile);
		m_mapBoolParams.insert(std::make_pair(strKey, bValue));
	}
	fread(&eType, sizeof(UINT), 1, pFile);
	fread(&loopSize, sizeof(int), 1, pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		wstring strKey;
		int strLen = 0;
		bool bValue = 0;
		fread(&strLen, sizeof(int), 1, pFile);
		fread(strBuff, strLen, 1, pFile);
		strBuff[strLen + 1] = '\0';
		strKey = strBuff;
		fread(&bValue, sizeof(bool), 1, pFile);
		m_mapTriggerParams.insert(std::make_pair(strKey, bValue));
	}
	fread(&loopSize, sizeof(int), 1, pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ZeroMemory(strBuff, MAXLEN);
		wstring strKey;
		int strLen = 0;
		float bValue = 0;
		fread(&strLen, sizeof(int), 1, pFile);
		fread(strBuff, strLen, 1, pFile);
		strBuff[strLen + 1] = '\0';
		strKey = strBuff;
		CAniNode* pNode = new CAniNode();
		pNode->SetName(strKey);
		pNode->Load(pFile);
		pNode->SetController(this);
		m_mapNode.insert(std::make_pair(strKey, pNode));
	}
	int strLen = 0;
	ZeroMemory(strBuff, MAXLEN);
	fread(&strLen, sizeof(int), 1, pFile);
	fread(strBuff, strLen, 1, pFile);
	m_LoadEditTxtPath = strBuff;

	for (auto itr = m_mapNode.begin(); itr != m_mapNode.end(); ++itr)
	{
		itr->second->LoadAfterProcess();
	}
	Init();

	fclose(pFile);
	return S_OK;
}

int CAnimatorController::Save(const wstring& _strFilePath)
{
	SetRelativePath(_strFilePath);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strFilePath;
	m_LoadEditTxtPath = CPathMgr::GetInst()->GetContentPath();
	m_LoadEditTxtPath = m_LoadEditTxtPath + L"editor\\" + GetKey() + L".nodeEDT";
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);
	//저장 목록 
	//1.params info
	//2.node info

	//1.params info
	PARAM_TYPE eType = PARAM_TYPE::INT;
	int loopSize = m_mapIntParams.size();
	fwrite(&eType, sizeof(UINT), 1, pFile);
	fwrite(&loopSize, sizeof(int), 1, pFile);
	for (auto itr = m_mapIntParams.begin(); itr != m_mapIntParams.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		int iValue = itr->second;

		fwrite(&strLen, sizeof(int), 1, pFile);
		fwrite(strKey.c_str(), strLen, 1, pFile);
		fwrite(&iValue, sizeof(int), 1, pFile);
	}
	eType = PARAM_TYPE::FLOAT;
	loopSize = m_mapFloatParams.size();
	fwrite(&eType, sizeof(UINT), 1, pFile);
	fwrite(&loopSize, sizeof(int), 1, pFile);
	for (auto itr = m_mapFloatParams.begin(); itr != m_mapFloatParams.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		float fValue = itr->second;

		fwrite(&strLen, sizeof(int), 1, pFile);
		fwrite(strKey.c_str(), strLen, 1, pFile);
		fwrite(&fValue, sizeof(float), 1, pFile);
	}
	eType = PARAM_TYPE::BOOL;
	loopSize = m_mapBoolParams.size();
	fwrite(&eType, sizeof(UINT), 1, pFile);
	fwrite(&loopSize, sizeof(int), 1, pFile);
	for (auto itr = m_mapBoolParams.begin(); itr != m_mapBoolParams.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		float bValue = itr->second;

		fwrite(&strLen, sizeof(int), 1, pFile);
		fwrite(strKey.c_str(), strLen, 1, pFile);
		fwrite(&bValue, sizeof(bool), 1, pFile);
	}
	eType = PARAM_TYPE::TRIGGER;
	loopSize = m_mapTriggerParams.size();
	fwrite(&eType, sizeof(UINT), 1, pFile);
	fwrite(&loopSize, sizeof(int), 1, pFile);
	for (auto itr = m_mapTriggerParams.begin(); itr != m_mapTriggerParams.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		float bValue = itr->second;

		fwrite(&strLen, sizeof(int), 1, pFile);
		fwrite(strKey.c_str(), strLen, 1, pFile);
		fwrite(&bValue, sizeof(bool), 1, pFile);
	}

	//2.node info
	loopSize = m_mapNode.size();
	fwrite(&loopSize, sizeof(int), 1, pFile);
	for (auto itr = m_mapNode.begin(); itr != m_mapNode.end(); ++itr)
	{
		wstring strKey = itr->first;
		int strLen = strKey.size() * UNICODELEN;
		CAniNode* pNode = itr->second;
		fwrite(&strLen, sizeof(int), 1, pFile);
		fwrite(strKey.c_str(), strLen, 1, pFile);
		pNode->Save(pFile);
	}

	int strLen = m_LoadEditTxtPath.size() * UNICODELEN;
	fwrite(&strLen, sizeof(int), 1, pFile);
	fwrite(m_LoadEditTxtPath.c_str(), strLen, 1, pFile);

	fclose(pFile);
	return S_OK;
}

void CAnimatorController::Init()
{
	m_pEntryNode = CreateNode(L"Entry", L"", -1);
	if (m_pEntryNode == nullptr)
	{
		m_pEntryNode = new CAniNode();
	}
	m_pExitNode = CreateNode(L"Exit", L"", -1);
	if (m_pExitNode == nullptr)
	{
		m_pExitNode = new CAniNode();
	}
	m_pAnyStateNode = CreateNode(L"AnyState", L"", -1);
	if (m_pAnyStateNode == nullptr)
	{
		m_pAnyStateNode = new CAniNode();
	}

	m_mapNode.insert(std::make_pair(m_pEntryNode->GetName(), m_pEntryNode));
	m_mapNode.insert(std::make_pair(m_pExitNode->GetName(), m_pExitNode));
	m_mapNode.insert(std::make_pair(m_pAnyStateNode->GetName(), m_pAnyStateNode));


	m_pEntryNode->SetController(this);
	m_pEntryNode->SetEditorNodeID(-1);
	m_pEntryNode->SetName(L"Entry");
	m_pExitNode->SetController(this);
	m_pExitNode->SetEditorNodeID(-1);
	m_pExitNode->SetName(L"Exit");
	m_pAnyStateNode->SetController(this);
	m_pAnyStateNode->SetEditorNodeID(-1);
	m_pAnyStateNode->SetName(L"AnyState");
}
CAniNode* CAnimatorController::CreateNode(wstring _strName, wstring _strClipName, int _iId)
{
	auto itr = m_mapNode.find(_strName);
	if (itr != m_mapNode.end())
	{
		return itr->second;
	}

	CAniNode* pNode = new CAniNode(_strClipName);
	pNode->SetName(_strName);
	pNode->SetController(this);
	pNode->SetEditorNodeID(_iId);
	m_mapNode.insert(std::make_pair(pNode->GetName(), pNode));
	m_mapIDNode.insert(std::make_pair(_iId, pNode));
	return pNode;
}

void CAnimatorController::SetNodeInfo(wstring _strName, wstring _strClipName)
{
	CAniNode* pNode = GetNode(_strName);
	pNode->SetName(_strName);
	pNode->SetAniKey(_strClipName);
}

void CAnimatorController::DestroyNode(wstring _strName)
{
	CAniNode* pNode = GetNode(_strName);
	pNode->Destory();

	m_mapIDNode.erase(pNode->GetEditorNodeID());
	m_mapNode.erase(_strName);

	delete pNode;
}

void CAnimatorController::DestroyNode(const int& _iID)
{
	CAniNode* pNode = GetNode(_iID);
	pNode->Destory();

	m_mapIDNode.erase(_iID);
	m_mapNode.erase(pNode->GetName());

	delete pNode;
}

void CAnimatorController::CreateTransition(const int& _startID, const int& _endID, const int& _linkID)
{
	auto itr = m_mapIDNode.find(_startID);
	auto itr2 = m_mapIDNode.find(_endID);
	auto link = m_mapIDTransition.find(_linkID);
	if (itr == m_mapIDNode.end() || itr2 == m_mapIDNode.end() || link != m_mapIDTransition.end())
		return;
	CAniNode* StartNode = itr->second;
	CAniNode* EndNode = itr2->second;

	CTransition* pTransition = new CTransition();
	pTransition->SetConnectNode(EndNode);
	pTransition->SetOwner(StartNode);

	StartNode->AddOutTransition(pTransition);
	EndNode->AddInTransition(pTransition);

	m_mapIDTransition.insert(std::make_pair(_linkID, pTransition));
	return;
}

CTransition* CAnimatorController::CreateTransition(wstring _strName, CAniNode* _pInNode, CAniNode* _pOutNode, bool _bExitTime)
{
	CTransition* pTransition = new CTransition();
	pTransition->SetName(_strName);
	pTransition->SetExitTime(_bExitTime);
	_pOutNode->AddOutTransition(pTransition);
	_pInNode->AddInTransition(pTransition);
	pTransition->SetConnectNode(_pInNode);
	pTransition->SetOwner(_pOutNode);
	return pTransition;
}

void CAnimatorController::DestroyTransition(wstring _strName)
{

}

void CAnimatorController::DestroyTransition(const int& _iID)
{
	auto itr = m_mapIDTransition.find(_iID);
	if (itr == m_mapIDTransition.end())
		return;
	CTransition* pTransition = itr->second;
	CAniNode* StartNode = pTransition->GetOwner();
	CAniNode* EndNode = pTransition->GetConnectNode();

	StartNode->RemoveOutTransition(pTransition);
	EndNode->RemoveInTransition(pTransition);

	delete itr->second;
	m_mapIDTransition.erase(itr);
}

void CAnimatorController::RegisterParam(wstring _strName, int _iValue)
{
	m_mapIntParams.insert(std::make_pair(_strName, _iValue));
}

void CAnimatorController::RegisterParam(wstring _strName, float _fValue)
{
	m_mapFloatParams.insert(std::make_pair(_strName, _fValue));
}

void CAnimatorController::RegisterParam(wstring _strName, bool _bValue, bool _bTrigger)
{
	if (_bTrigger)
	{
		m_mapTriggerParams.insert(std::make_pair(_strName, _bValue));
	}
	else
	{
		m_mapBoolParams.insert(std::make_pair(_strName, _bValue));
	}
}

CAniNode* CAnimatorController::GetNode(const wstring _strName)
{
	auto itr = m_mapNode.find(_strName);
	if (itr == m_mapNode.end())
		return nullptr;

	return m_mapNode[_strName];
}

CAniNode* CAnimatorController::GetNode(const int& _iID)
{
	auto itr = m_mapIDNode.find(_iID);
	if (itr == m_mapIDNode.end())
		return nullptr;

	return m_mapIDNode[_iID];
}

CTransition* CAnimatorController::GetTransition(const int& _iID)
{
	auto itr = m_mapIDTransition.find(_iID);
	if (itr == m_mapIDTransition.end())
		return nullptr;

	return m_mapIDTransition[_iID];
}

CTransition* CAnimatorController::GetTransition(const int& _startNodeId, const int& _endNodeId)
{
	CAniNode* pStart = m_mapIDNode[_startNodeId];
	CAniNode* pEnd = m_mapIDNode[_endNodeId];
	
	return pStart->GetTransition(pEnd);
}

void CAnimatorController::ChangeNode(const wstring _strName, int _iID)
{
	CAniNode* pReturnNode = nullptr;
	for (auto itr = m_mapNode.begin(); itr != m_mapNode.end(); ++itr)
	{
		wstring wstrName = itr->first;
		CAniNode* pNode = itr->second;
		if (pNode == nullptr)
			continue;
		const int& iID = pNode->GetEditorNodeID();
		if (iID == _iID)
		{
			if (_strName != wstrName)
			{
				m_mapNode.erase(itr);
				m_mapNode.insert(std::make_pair(_strName, pNode));
				return;
			}
		}
	}
}

bool CAnimatorController::IsChangeNodeName(const wstring _strName, int _iID)
{
	for (auto itr = m_mapNode.begin(); itr != m_mapNode.end();)
	{
		wstring wstrName = itr->first;
		CAniNode* pNode = itr->second;
		if (pNode == nullptr)
			return false;
		const int& iID = pNode->GetEditorNodeID();
		if (iID == _iID)
		{
			if (_strName != wstrName)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}



ANI_NODE_RETURN CAnimatorController::NextNode(bool _bFinish, bool _bLoop, wstring _strCurName, CAnimator3D* _pAnimator)
{
	CAniNode* CurNode = nullptr;
	auto NodeItr = m_mapNode.find(_strCurName);
	//_strCurName 이 any name 인 경우.
	if (NodeItr == m_mapNode.end())
	{
		for (auto itr = m_mapNode.begin(); itr != m_mapNode.end(); ++itr)
		{
			wstring key = itr->second->GetAnimationKey();
			if (key.compare(_strCurName) == 0)
			{
				CurNode = itr->second;
				break;
			}
		}
	}//entry,exit node 인 경우
	else
	{
		CurNode = NodeItr->second;
	}

	if (CurNode == nullptr)
		return ANI_NODE_RETURN::NOTHING;

	//AnyState가 Change 면 CurNode 보다 우선순위가 높음.
	ANI_NODE_RETURN eType1 = CurNode->NextNode(_bFinish, _bLoop, _pAnimator);
	ANI_NODE_RETURN eType2 = m_pAnyStateNode->NextNode(_bFinish, _bLoop, _pAnimator);

	if (eType2 == ANI_NODE_RETURN::CHANGE)
		return eType2;
	else
		return eType1;
}

void CAnimatorController::CopyParams(CAnimator3D* _pAnimator)
{
	_pAnimator->m_AniParams.mapIntParams.clear();
	for (auto itr = m_mapIntParams.begin(); itr != m_mapIntParams.end(); ++itr)
	{
		wstring key = itr->first;
		int value = itr->second;

		_pAnimator->m_AniParams.mapIntParams.insert(std::make_pair(key, value));
	}

	for (auto itr = m_mapFloatParams.begin(); itr != m_mapFloatParams.end(); ++itr)
	{
		wstring key = itr->first;
		float value = itr->second;

		_pAnimator->m_AniParams.mapFloatParams.insert(std::make_pair(key, value));
	}

	for (auto itr = m_mapBoolParams.begin(); itr != m_mapBoolParams.end(); ++itr)
	{
		wstring key = itr->first;
		bool value = itr->second;

		_pAnimator->m_AniParams.mapBoolParams.insert(std::make_pair(key, value));
	}

	for (auto itr = m_mapTriggerParams.begin(); itr != m_mapTriggerParams.end(); ++itr)
	{
		wstring key = itr->first;
		bool value = itr->second;

		_pAnimator->m_AniParams.mapTriggerParams.insert(std::make_pair(key, value));
	}
}



vector<wstring> CAnimatorController::GetParamNames()
{
	vector<wstring> Result;
	for (auto itr = m_mapIntParams.begin(); itr != m_mapIntParams.end(); ++itr)
	{
		wstring Key = itr->first;
		Result.push_back(Key);
	}
	for (auto itr = m_mapFloatParams.begin(); itr != m_mapFloatParams.end(); ++itr)
	{
		wstring Key = itr->first;
		Result.push_back(Key);
	}
	for (auto itr = m_mapBoolParams.begin(); itr != m_mapBoolParams.end(); ++itr)
	{
		wstring Key = itr->first;
		Result.push_back(Key);
	}
	for (auto itr = m_mapTriggerParams.begin(); itr != m_mapTriggerParams.end(); ++itr)
	{
		wstring Key = itr->first;
		Result.push_back(Key);
	}

	return Result;
}

PARAM_TYPE CAnimatorController::GetParamType(wstring _strName)
{
	for (auto itr = m_mapIntParams.begin(); itr != m_mapIntParams.end(); ++itr)
	{
		if (itr->first == _strName)
		{
			return PARAM_TYPE::INT;
		}
	}

	for (auto itr = m_mapFloatParams.begin(); itr != m_mapFloatParams.end(); ++itr)
	{
		if (itr->first == _strName)
		{
			return PARAM_TYPE::FLOAT;
		}
	}

	for (auto itr = m_mapBoolParams.begin(); itr != m_mapBoolParams.end(); ++itr)
	{
		if (itr->first == _strName)
		{
			return PARAM_TYPE::BOOL;
		}
	}

	for (auto itr = m_mapTriggerParams.begin(); itr != m_mapTriggerParams.end(); ++itr)
	{
		if (itr->first == _strName)
		{
			return PARAM_TYPE::TRIGGER;
		}
	}
}

void CAnimatorController::RegisterIDNode(const int& _iID, CAniNode* _pNode)
{
	auto itr = m_mapIDNode.find(_iID);
	if (itr != m_mapIDNode.end())
		return;

	m_mapIDNode.insert(std::make_pair(_iID, _pNode));

	_pNode->SetEditorNodeID(_iID);
}

void CAnimatorController::RegisterIDTransition(const int& _iId, CTransition* _pTransition)
{
	auto itr = m_mapIDTransition.find(_iId);
	if (itr != m_mapIDTransition.end())
		return;

	m_mapIDTransition.insert(std::make_pair(_iId, _pTransition));
}

vector<CAniNode*> CAnimatorController::GetAllNode()
{
	vector<CAniNode*> Result;
	for (auto itr = m_mapNode.begin(); itr != m_mapNode.end(); ++itr)
	{
		Result.push_back(itr->second);
	}

	return Result;
}
