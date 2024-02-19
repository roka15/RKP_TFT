#include "pch.h"
#include "AnimatorController.h"

#include "CPathMgr.h"
#include "CResMgr.h"
#include "AniNode.h"
#include "Transition.h"

CAnimatorController::CAnimatorController() :CRes(RES_TYPE::ANICONTROLLER, true)
{
	m_pEntryNode = CreateNode(L"Entry", L"");

	m_pExitNode = CreateNode(L"Exit", L"");

	m_pAnyStateNode = CreateNode(L"AnyState", L"");

	m_mapNode.insert(std::make_pair(m_pEntryNode->GetName(), m_pEntryNode));
	m_mapNode.insert(std::make_pair(m_pExitNode->GetName(), m_pExitNode));
	m_mapNode.insert(std::make_pair(m_pAnyStateNode->GetName(), m_pAnyStateNode));

	m_pCurNode = m_pEntryNode;
}

CAnimatorController::~CAnimatorController()
{
}

int CAnimatorController::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);
	UINT test = 0;
	fread(&test, sizeof(UINT), 1, pFile);
	fclose(pFile);
	return S_OK;
}

int CAnimatorController::Save(const wstring& _strFilePath)
{
	SetRelativePath(_strFilePath);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strFilePath;
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);
	int test = -1;
	fwrite(&test, sizeof(UINT), 1, pFile);
	fclose(pFile);
	return S_OK;
}

void CAnimatorController::Init()
{
	m_pEntryNode->SetController(this);
	m_pExitNode->SetController(this);
	m_pAnyStateNode->SetController(this);
}

void CAnimatorController::finaltick()
{
	if (m_pCurNode)
		m_pCurNode->finaltick();
}

void CAnimatorController::UpdateData(CStructuredBuffer*& _finalMat)
{
	if (m_pCurNode)
		m_pCurNode->UpdateData(_finalMat);
}

CAniNode* CAnimatorController::CreateNode(wstring _strName, wstring _strClipName)
{
	auto itr = m_mapNode.find(_strName);
	if (itr != m_mapNode.end())
	{
		assert(nullptr);
		return nullptr;
	}

	CAniNode* pNode = new CAniNode(_strClipName);
	pNode->SetName(_strName);
	pNode->SetController(this);
	m_mapNode.insert(std::make_pair(pNode->GetName(), pNode));
	return pNode;
}

void CAnimatorController::DestroyNode(wstring _strName)
{
	CAniNode* pNode = GetNode(_strName);
	pNode->Destory();
}

CTransition* CAnimatorController::CreateTransition(wstring _strName, CAniNode* _pInNode, CAniNode* _pOutNode)
{
	CTransition* pTransition = new CTransition();
	_pOutNode->AddOutTransition(pTransition);
	_pInNode->AddInTransition(pTransition);
	pTransition->SetConnectNode(_pInNode);
	pTransition->SetOwner(_pOutNode);
	return pTransition;
}

void CAnimatorController::DestroyTransition(wstring _strName)
{

}

void CAnimatorController::RegisterParam(wstring _strName, int _iValue)
{
}

void CAnimatorController::RegisterParam(wstring _strName, float _fValue)
{
}

void CAnimatorController::RegisterParam(wstring _strName, bool _bValue, bool _bTrigger)
{
}

void CAnimatorController::DeleteParam(PARAM_TYPE _eType, wstring _strName)
{
}

UINT CAnimatorController::GetBoneCount()
{
	return m_pCurNode->GetBoneCount();
}