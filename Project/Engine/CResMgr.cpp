#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CGameObject.h"
#include "components.h"

CResMgr::CResMgr()
	: m_Changed(false)
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{
	InitSound();
	
	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();	
	CreateDefaultTexture();

	InitObjectPrefab();
}



void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::InitObjectPrefab()
{
	CGameObject* pObj = nullptr;
	CPrefab* pPrefab = nullptr;
#pragma region Text
	pObj = new CGameObject();
	pObj->SetName(L"Text");
	pObj->AddComponent(new CTransform());
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CText());
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pPrefab = new CPrefab();
	pPrefab->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"UI_Text", pPrefab);
#pragma endregion

#pragma region Image
	pObj = new CGameObject();
	pObj->SetName(L"Image");
	pObj->AddComponent(new CTransform());
	pObj->AddComponent(new CMeshRender());
	pObj->AddComponent(new CImage());
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pPrefab = new CPrefab();
	pPrefab->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"UI_Image", pPrefab);
#pragma endregion

#pragma region Button
	pObj = new CGameObject();
	pObj->SetName(L"Button");
	pObj->AddComponent(new CTransform());
	pObj->AddComponent(new CMeshRender());
	pObj->AddComponent(new CCollider2D());
	pObj->AddComponent(new CImage());
	pObj->AddComponent(new CButton());
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pPrefab = new CPrefab();
	pPrefab->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"UI_Button", pPrefab);
#pragma endregion
}


void CResMgr::tick()
{
	m_Changed = false;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex =  FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CMeshData> CResMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetKey(strName);
	pMeshData->SetRelativePath(strName);

	m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.Get()));

	// meshdata 를 실제파일로 저장
	pMeshData->Save(strName);

	return pMeshData;
}


void CResMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	assert(!(iter == m_arrRes[(UINT)_type].end()));

	m_arrRes[(UINT)_type].erase(iter);	

	m_Changed = true;
}



void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName, UINT _iSlotNum, UINT _iSemanticIdx)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};

	if (0 == _iSlotNum)
	{
		LayoutDesc.AlignedByteOffset = m_iLayoutOffset_0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.InstanceDataStepRate = 0;
	}
	else if (1 == _iSlotNum)
	{
		LayoutDesc.AlignedByteOffset = m_iLayoutOffset_1;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc.InstanceDataStepRate = 1;
	}

	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = _iSlotNum;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = _iSemanticIdx;

	m_vecLayoutInfo.push_back(LayoutDesc);


	// Offset 증가
	if (0 == _iSlotNum)
		m_iLayoutOffset_0 += GetSizeofFormat(_eFormat);
	else if (1 == _iSlotNum)
		m_iLayoutOffset_1 += GetSizeofFormat(_eFormat);
}