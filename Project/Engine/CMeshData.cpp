#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CResMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

#include "CFBXLoader.h"


CMeshData::CMeshData(bool _bEngine)
	: CRes(RES_TYPE::MESHDATA, _bEngine)
{
}

CMeshData::~CMeshData()
{
}


CGameObject* CMeshData::Instantiate()
{
	UINT MeshCnt = m_vecDataNode.size();
	CGameObject* parrentObj = new CGameObject();
	parrentObj->AddComponent(new CTransform);
	//CAnimator3D* pAnimator = new CAnimator3D;
	//parrentObj->AddComponent(pAnimator);

	for (int i = 0; i < MeshCnt; ++i)
	{
		Ptr<CMesh> pMesh = m_vecDataNode[i]->m_pMesh;
		vector<Ptr<CMaterial>> vecMaterial = m_vecDataNode[i]->m_vecMtrl;

		CGameObject* pNewObj = new CGameObject;
		parrentObj->AddChild(pNewObj);

		pNewObj->SetName(pMesh->GetName());
		pNewObj->AddComponent(new CTransform);
		pNewObj->AddComponent(new CMeshRender);
		pNewObj->MeshRender()->SetMesh(pMesh);

		for (UINT i = 0; i < vecMaterial.size(); ++i)
		{
			pNewObj->MeshRender()->SetMaterial(vecMaterial[i], i);
		}
		// Animation 파트 추가
		if (pMesh->GetAniClipList().size() == 0)
			continue;

		CAnimator3D* pAnimator = new CAnimator3D;
		pNewObj->AddComponent(pAnimator);
		pAnimator->RegisterAniClip(pMesh->GetAniClipList());
	}
	return parrentObj;
}


CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	vector<CMeshDataNode*> vecMdatNode = {};
	UINT ContainSize = loader.GetContainerCount();
	for (int i = 0; i < ContainSize; ++i)
	{
		std::wstring MeshName = loader.GetContainer(i).strName;
		Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(MeshName);
		assert(pMesh.Get());

		vector<Ptr<CMaterial>> vecMtrl;
		// 메테리얼 가져오기
		for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
		{
			// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(i).vecMtrl[j].strMtrlName);
			assert(pMtrl.Get());

			vecMtrl.push_back(pMtrl);
		}

		CMeshDataNode* mdatNode = new CMeshDataNode();
		mdatNode->m_pMesh = pMesh;
		mdatNode->m_vecMtrl = vecMtrl;
		vecMdatNode.push_back(mdatNode);
	}


	CMeshData* pMeshData = new CMeshData(true);
	pMeshData->m_vecDataNode = vecMdatNode;

	return pMeshData;
}




int CMeshData::Save(const wstring& _strRelativePath)
{
	SetRelativePath(_strRelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);
	UINT iMeshCnt = m_vecDataNode.size();

	fwrite(&iMeshCnt, sizeof(UINT), 1, pFile);
	for (int i = 0; i < iMeshCnt; ++i)
	{	// Mesh Key 저장	
		// Mesh Data 저장
		fwrite(&i, sizeof(UINT), 1, pFile);
		SaveResRef(m_vecDataNode[i]->m_pMesh.Get(), pFile);

		// material 정보 저장
		UINT iMtrlCount = (UINT)m_vecDataNode[i]->m_vecMtrl.size();
		fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

		UINT j = 0;
		for (; j < iMtrlCount; ++j)
		{
			if (nullptr == m_vecDataNode[i]->m_vecMtrl[j])
				continue;

			// Material 인덱스, Key, Path 저장
			fwrite(&j, sizeof(UINT), 1, pFile);
			SaveResRef(m_vecDataNode[i]->m_vecMtrl[j].Get(), pFile);
		}
	
	}

	int iEnd = -1; // 마감 값
	fwrite(&iEnd, sizeof(UINT), 1, pFile);
	fclose(pFile);

	return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);

	
	// Mesh Load
	UINT iMeshCount = 0;
	fread(&iMeshCount, sizeof(UINT), 1, pFile);
	m_vecDataNode.resize(iMeshCount);
	for (int i = 0; i < iMeshCount; ++i)
	{
		m_vecDataNode[i] = new CMeshDataNode();
		Ptr<CMesh> pMesh = nullptr;
		UINT iMeshidx = -1;
		fread(&iMeshidx, 4, 1, pFile);
		LoadResRef<CMesh>(pMesh, pFile);
		m_vecDataNode[i]->m_pMesh = pMesh;
		assert(pMesh.Get());
	    

		// material 정보 읽기
		UINT iMtrlCount = 0;
		fread(&iMtrlCount, sizeof(UINT), 1, pFile);
		vector<Ptr<CMaterial>>& vecMatrial = m_vecDataNode[i]->m_vecMtrl;
		vecMatrial.resize(iMtrlCount);

		for (UINT j = 0; j < iMtrlCount; ++j)
		{
			UINT idx = -1;
			fread(&idx, 4, 1, pFile);
			if (idx == -1)
				break;

			wstring strKey, strPath;

			Ptr<CMaterial> pMtrl;
			LoadResRef<CMaterial>(pMtrl, pFile);

			vecMatrial[j] = pMtrl;
		}
	}
	fclose(pFile);
	return S_OK;
}