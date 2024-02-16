#include "pch.h"
#include "AnimatorController.h"

#include "CPathMgr.h"
#include "CResMgr.h"
#include "AniNode.h"
CAnimatorController::CAnimatorController() :CRes(RES_TYPE::ANICONTROLLER, true)
{
	wstring strPath = L"controller\\" + GetName() + L".controller";
	Save(strPath);
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

void CAnimatorController::finaltick()
{
	if (m_bStart && m_pCurNode)
		m_pCurNode->finaltick();
}

void CAnimatorController::UpdateData(CStructuredBuffer*& _finalMat)
{
	if (m_pCurNode)
		m_pCurNode->UpdateData(_finalMat);
}

void CAnimatorController::CreateNode(wstring _strName, wstring _strClipName)
{
}

void CAnimatorController::DestroyNode(wstring _strName)
{
}

void CAnimatorController::CreateTransition(wstring _strName, CAniNode* _pConnectNode)
{
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
