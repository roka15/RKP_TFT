#include "pch.h"
#include "CFBXLoader.h"
#include "CMesh.h"
#include "CAniClip.h"
#include "CAnimation3D.h"
#include "CResMgr.h"
#include "CPathMgr.h"


#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using std::experimental::filesystem::path;

CFBXLoader::CFBXLoader()
	: m_pManager(NULL)
	, m_pScene(NULL)
	, m_pImporter(NULL)
	, m_bLoadAni(false)
{
}

CFBXLoader::~CFBXLoader()
{
	m_pScene->Destroy();
	m_pManager->Destroy();

	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		if (nullptr != m_vecBone[i])
			delete m_vecBone[i];
	}

	for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
	{
		if (nullptr != m_vecAnimClip[i])
			delete m_vecAnimClip[i];
	}

	for (int i = 0; i < m_arrAnimName.Size(); ++i)
	{
		if (nullptr != m_arrAnimName[i])
			delete m_arrAnimName[i];
	}
}

void CFBXLoader::init()
{
	m_pManager = FbxManager::Create();
	if (NULL == m_pManager)
		assert(NULL);

	FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIOSettings);

	m_pScene = FbxScene::Create(m_pManager, "");
	if (NULL == m_pScene)
		assert(NULL);
}

void CFBXLoader::LoadFbx(const wstring& _strPath)
{
	m_vecContainer.clear();
	m_bLoadAni = false;

	m_pImporter = FbxImporter::Create(m_pManager, "");

	//wstring str = wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(strName.c_str());
	string strPath(_strPath.begin(), _strPath.end());
	m_Path = strPath;
	if (!m_pImporter->Initialize(strPath.c_str(), -1, m_pManager->GetIOSettings()))
		assert(nullptr);

	m_pImporter->Import(m_pScene);

	/*FbxAxisSystem originAxis = FbxAxisSystem::eMax;
	originAxis = m_pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem DesireAxis = FbxAxisSystem::DirectX;
	DesireAxis.ConvertScene(m_pScene);
	originAxis = m_pScene->GetGlobalSettings().GetAxisSystem();*/

	m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	// Bone 정보 읽기
	LoadSkeleton(m_pScene->GetRootNode());

	// Animation 이름정보 
	m_pScene->FillAnimStackNameArray(m_arrAnimName);

	// Animation Clip 정보
	LoadAnimationClip();

	// 삼각화(Triangulate)
	Triangulate(m_pScene->GetRootNode());

	// 메쉬 데이터 얻기
	LoadMeshDataFromNode(m_pScene->GetRootNode());

	m_pImporter->Destroy();

	// 필요한 텍스쳐 로드
	LoadTexture();

	// 필요한 메쉬 생성
	CreateMesh();
	// 필요한 메테리얼 생성
	CreateMaterial();
}

void CFBXLoader::LoadMeshDataFromNode(FbxNode* _pNode)
{
	// 노드의 메쉬정보 읽기
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr && FbxNodeAttribute::eMesh == pAttr->GetAttributeType())
	{
		FbxAMatrix matGlobal = _pNode->EvaluateGlobalTransform();
		matGlobal.GetR();

		FbxMesh* pMesh = _pNode->GetMesh();
		pMesh->SetName(_pNode->GetName());
		string MeshName = pMesh->GetName();
		//파일 명이 MU가 안붙고 Mesh에만 붙었을 경우 = Mesh마다 Texture mapping 방식이 다름.
		if (m_Path.rfind("MU_") == string::npos)
		{
			if (MeshName.rfind("MU_") != string::npos)
			{
				m_bMultiUVFlag = true;
			}
			else
			{
				m_bMultiUVFlag = false;
			}
		}
		//파일 명에 MU_가 붙었을 경우 = 파일 내의 모든 Mesh가 한 정점에 대한 UV의 개수가 2개 이상일 수 있다.
		else
		{
			m_bMultiUVFlag = true;
		}

		if (NULL != pMesh)
			LoadMesh(pMesh);
	}

	// 해당 노드의 재질정보 읽기
	UINT iMtrlCnt = _pNode->GetMaterialCount();
	if (iMtrlCnt > 0)
	{
		for (UINT i = 0; i < iMtrlCnt; ++i)
		{
			FbxSurfaceMaterial* pMtrlSur = _pNode->GetMaterial(i);
			LoadMaterial(pMtrlSur);
		}
	}

	// 자식 노드 정보 읽기
	int iChildCnt = _pNode->GetChildCount();
	for (int i = 0; i < iChildCnt; ++i)
	{
		LoadMeshDataFromNode(_pNode->GetChild(i));
	}
}

void CFBXLoader::LoadMesh(FbxMesh* _pFbxMesh)
{
	string strName = _pFbxMesh->GetName();
	if (strName.compare("Icosphere") == 0)
		return;
	m_vecContainer.push_back(tContainer{});
	tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

	Container.strName = wstring(strName.begin(), strName.end());

	int iVtxCnt = 0;
	int iControlPointCnt = _pFbxMesh->GetControlPointsCount();
	Container.iControlPointCnt = iControlPointCnt;
	// 폴리곤 개수
	int iPolyCnt = _pFbxMesh->GetPolygonCount();
	int iPolySize = _pFbxMesh->GetPolygonSize(0);

	FbxVector4* pFbxPos = _pFbxMesh->GetControlPoints();

	if (m_bMultiUVFlag)
	{
		iVtxCnt = iPolyCnt * iPolySize;
	}
	else
	{
		iVtxCnt = iControlPointCnt;
	}

	Container.Resize(iVtxCnt);

	std::vector<Vec3> tempVecPos;
	tempVecPos.resize(iControlPointCnt);
	for (int i = 0; i < iControlPointCnt; ++i)
	{
		tempVecPos[i].x = (float)pFbxPos[i].mData[0];
		tempVecPos[i].y = (float)pFbxPos[i].mData[2];
		tempVecPos[i].z = (float)pFbxPos[i].mData[1];
	}
	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int iMtrlCnt = _pFbxMesh->GetNode()->GetMaterialCount();
	Container.vecIdx.resize(iMtrlCnt);

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* pMtrl = _pFbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수

	if (3 != iPolySize)
		assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우

	UINT arrIdx[3] = {};
	UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번

	for (int i = 0; i < iPolyCnt; ++i)
	{
		for (int j = 0; j < iPolySize; ++j)
		{
			// i 번째 폴리곤에, j 번째 정점
			int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
			arrIdx[j] = iIdx;

			GetTangent(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetBinormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetNormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetUV(_pFbxMesh, &Container, iIdx, _pFbxMesh->GetTextureUVIndex(i, j), iVtxOrder);
			if (m_bMultiUVFlag)
			{
				Container.vecPos[iVtxOrder] = tempVecPos[iIdx];
			}
			else
			{
				Container.vecPos[iIdx] = tempVecPos[iIdx];
			}

			++iVtxOrder;
		}
		/*	if (iMtrlCnt == 0)
				continue;*/
		UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
		if (m_bMultiUVFlag)
		{
			int iIdxNum = iVtxOrder - 1;
			Container.vecIdx[iSubsetIdx].push_back(iIdxNum - 2);
			Container.vecIdx[iSubsetIdx].push_back(iIdxNum);
			Container.vecIdx[iSubsetIdx].push_back(iIdxNum - 1);
		}
		else
		{
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
		}
	}

	/*if (iMtrlCnt == 0)
		return;*/


	LoadAnimationData(_pFbxMesh, &Container);
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* _pMtrlSur)
{
	tFbxMaterial tMtrlInfo{};

	string str = _pMtrlSur->GetName();
	tMtrlInfo.strMtrlName = wstring(str.begin(), str.end());

	// Diff
	tMtrlInfo.tMtrl.vDiff = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sDiffuse
		, FbxSurfaceMaterial::sDiffuseFactor);

	// Amb
	tMtrlInfo.tMtrl.vAmb = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sAmbient
		, FbxSurfaceMaterial::sAmbientFactor);

	// Spec
	tMtrlInfo.tMtrl.vSpec = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sSpecular
		, FbxSurfaceMaterial::sSpecularFactor);

	// Emisv
	tMtrlInfo.tMtrl.vEmv = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sEmissive
		, FbxSurfaceMaterial::sEmissiveFactor);

	// Texture Name
	tMtrlInfo.strDiff = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sDiffuse);
	tMtrlInfo.strNormal = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sNormalMap);
	tMtrlInfo.strSpec = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sSpecular);
	tMtrlInfo.strEmis = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sEmissive);


	m_vecContainer.back().vecMtrl.push_back(tMtrlInfo);
}

void CFBXLoader::GetTangent(FbxMesh* _pMesh
	, tContainer* _pContainer
	, int _iIdx		 /*해당 정점의 인덱스*/
	, int _iVtxOrder /*폴리곤 단위로 접근하는 순서*/)
{
	int iTangentCnt = _pMesh->GetElementTangentCount();
	if (1 < iTangentCnt)
		assert(NULL); // 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.

	if (1 == iTangentCnt)
	{
		// 탄젠트 data 의 시작 주소
		FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();
		UINT iTangentIdx = 0;
		fbxsdk::FbxLayerElement::EMappingMode mode = pTangent->GetMappingMode();
		FbxVector4 vTangent = {};
		switch (mode)
		{
		case FbxGeometryElement::eByPolygonVertex:
			if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				iTangentIdx = _iVtxOrder;
			else
				iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
			break;
		case FbxGeometryElement::eByControlPoint:
			if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				iTangentIdx = _iIdx;
			else
				iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
			break;
		}
		vTangent = pTangent->GetDirectArray().GetAt(iTangentIdx);

		if (m_bMultiUVFlag)
		{
			_pContainer->vecTangent[_iVtxOrder].x = (float)vTangent.mData[0];
			_pContainer->vecTangent[_iVtxOrder].y = (float)vTangent.mData[2];
			_pContainer->vecTangent[_iVtxOrder].z = (float)vTangent.mData[1];
		}
		else
		{
			_pContainer->vecTangent[_iIdx].x = (float)vTangent.mData[0];
			_pContainer->vecTangent[_iIdx].y = (float)vTangent.mData[2];
			_pContainer->vecTangent[_iIdx].z = (float)vTangent.mData[1];
		}
	}
	else
	{
		if (m_bMultiUVFlag)
		{
			_pContainer->vecTangent[_iVtxOrder].x = 0.f;
			_pContainer->vecTangent[_iVtxOrder].y = 0.f;
			_pContainer->vecTangent[_iVtxOrder].z = 0.f;
		}
		else
		{
			_pContainer->vecTangent[_iIdx].x = 0.f;
			_pContainer->vecTangent[_iIdx].y = 0.f;
			_pContainer->vecTangent[_iIdx].z = 0.f;
		}
	}
}

void CFBXLoader::GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
	int iBinormalCnt = _pMesh->GetElementBinormalCount();
	if (1 < iBinormalCnt)
		assert(NULL);// 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	if (1 == iBinormalCnt)
	{
		// 종법선 data 의 시작 주소
		FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
		UINT iBinormalIdx = 0;

		fbxsdk::FbxLayerElement::EMappingMode mode = pBinormal->GetMappingMode();
		FbxVector4 vBinormal = {};
		switch (mode)
		{
		case FbxGeometryElement::eByPolygonVertex:
			if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				iBinormalIdx = _iVtxOrder;
			else
				iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
			break;
		case FbxGeometryElement::eByControlPoint:
			if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				iBinormalIdx = _iIdx;
			else
				iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
			vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);
			break;
		}
		vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);
		if (m_bMultiUVFlag)
		{
			_pContainer->vecBinormal[_iVtxOrder].x = (float)vBinormal.mData[0];
			_pContainer->vecBinormal[_iVtxOrder].y = (float)vBinormal.mData[2];
			_pContainer->vecBinormal[_iVtxOrder].z = (float)vBinormal.mData[1];
		}
		else
		{
			_pContainer->vecBinormal[_iIdx].x = (float)vBinormal.mData[0];
			_pContainer->vecBinormal[_iIdx].y = (float)vBinormal.mData[2];
			_pContainer->vecBinormal[_iIdx].z = (float)vBinormal.mData[1];
		}
	}
	else
	{
		if (m_bMultiUVFlag)
		{
			_pContainer->vecBinormal[_iVtxOrder].x = 0.f;
			_pContainer->vecBinormal[_iVtxOrder].y = 0.f;
			_pContainer->vecBinormal[_iVtxOrder].z = 0.f;
		}
		else
		{
			_pContainer->vecBinormal[_iIdx].x = 0.f;
			_pContainer->vecBinormal[_iIdx].y = 0.f;
			_pContainer->vecBinormal[_iIdx].z = 0.f;
		}
	}

}

void CFBXLoader::GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
	int iNormalCnt = _pMesh->GetElementNormalCount();
	if (1 != iNormalCnt)
		assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();
	UINT iNormalIdx = 0;

	fbxsdk::FbxLayerElement::EMappingMode mode = pNormal->GetMappingMode();
	FbxVector4 vNormal = {};
	switch (mode)
	{
	case FbxGeometryElement::eByPolygonVertex:
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iVtxOrder;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);

		vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);
		break;
	case FbxGeometryElement::eByControlPoint:
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iIdx;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
		vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);
		break;
	}
	vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);
	if (m_bMultiUVFlag)
	{
		_pContainer->vecNormal[_iVtxOrder].x = (float)vNormal.mData[0];
		_pContainer->vecNormal[_iVtxOrder].y = (float)vNormal.mData[2];
		_pContainer->vecNormal[_iVtxOrder].z = (float)vNormal.mData[1];
	}
	else
	{
		_pContainer->vecNormal[_iIdx].x = (float)vNormal.mData[0];
		_pContainer->vecNormal[_iIdx].y = (float)vNormal.mData[2];
		_pContainer->vecNormal[_iIdx].z = (float)vNormal.mData[1];
	}
}

void CFBXLoader::GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iUVIndex, int _iVtxOrder)
{
	if (_iUVIndex > 300)
		int a = 0;
	FbxGeometryElementUV* pUV = _pMesh->GetElementUV();

	UINT iUVIdx = 0;

	/*
		if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect)
			iUVIdx = _iIdx;
		else
			iUVIdx = pUV->GetIndexArray().GetAt(_iIdx);
	*/

	iUVIdx = _iUVIndex;
	int size = pUV->GetDirectArray().GetCount();
	FbxVector2 vUV = pUV->GetDirectArray().GetAt(iUVIdx);

	if (m_bMultiUVFlag)
	{
		_pContainer->vecUV[_iVtxOrder].x = (float)vUV.mData[0];
		_pContainer->vecUV[_iVtxOrder].y = 1.f - (float)vUV.mData[1];
	}
	else
	{
		_pContainer->vecUV[_iIdx].x = (float)vUV.mData[0];
		_pContainer->vecUV[_iIdx].y = 1.f - (float)vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
	}
}

Vec4 CFBXLoader::GetMtrlData(FbxSurfaceMaterial* _pSurface
	, const char* _pMtrlName
	, const char* _pMtrlFactorName)
{
	FbxDouble3  vMtrl;
	FbxDouble	dFactor = 0.;

	FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
	FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

	if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
	{
		vMtrl = tMtrlProperty.Get<FbxDouble3>();
		dFactor = tMtrlFactorProperty.Get<FbxDouble>();
	}

	Vec4 vRetVal = Vec4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
	return vRetVal;
}

wstring CFBXLoader::GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty)
{
	string strName;

	FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
	if (TextureProperty.IsValid())
	{
		UINT iCnt = TextureProperty.GetSrcObjectCount();

		if (1 <= iCnt)
		{
			FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
			if (NULL != pFbxTex)
				strName = pFbxTex->GetFileName();
		}
	}

	return wstring(strName.begin(), strName.end());
}

void CFBXLoader::LoadTexture()
{
	path path_content = CPathMgr::GetInst()->GetContentPath();

	path path_fbx_texture = path_content.wstring() + L"texture\\FBXTexture\\";
	if (false == exists(path_fbx_texture))
	{
		create_directory(path_fbx_texture);
	}

	path path_origin;
	path path_filename;
	path path_dest;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
		{
			vector<path> vecPath;
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strDiff.c_str());
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strNormal.c_str());
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strSpec.c_str());
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strEmis.c_str());

			for (size_t k = 0; k < vecPath.size(); ++k)
			{
				if (vecPath[k].filename().empty())
					continue;

				path_origin = vecPath[k];
				path_filename = vecPath[k].filename();
				path_dest = path_fbx_texture.wstring() + path_filename.wstring();

				if (false == exists(path_dest))
				{
					copy(path_origin, path_dest);
				}

				path_dest = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), path_dest);
				CResMgr::GetInst()->Load<CTexture>(path_dest, path_dest);

				switch (k)
				{
				case 0: m_vecContainer[i].vecMtrl[j].strDiff = path_dest; break;
				case 1: m_vecContainer[i].vecMtrl[j].strNormal = path_dest; break;
				case 2: m_vecContainer[i].vecMtrl[j].strSpec = path_dest; break;
				case 3: m_vecContainer[i].vecMtrl[j].strEmis = path_dest; break;
				}
			}
		}
		path_origin = path_origin.parent_path();
		remove_all(path_origin);
	}
}

void CFBXLoader::CreateMesh()
{
	wstring strPath;
	Ptr<CMesh> pMesh = nullptr;
	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		std::wstring strMeshName = m_vecContainer[i].strName;
		strPath = L"mesh\\";
		strPath += strMeshName + L".mesh";
		m_vecContainer[i].strName = strPath;

		pMesh = CResMgr::GetInst()->FindRes<CMesh>(strPath);
		if (pMesh != nullptr)
		{
			continue;
		}

		pMesh = CMesh::CreateFromContainer(*this, i);
		pMesh->SetName(strMeshName);
		pMesh->SetKey(strPath);
		pMesh->SetRelativePath(strPath);

		CResMgr::GetInst()->AddRes(pMesh->GetKey(), pMesh);
		pMesh->Save(strPath);
	}


}

void CFBXLoader::CreateMaterial()
{
	wstring strMtrlName;
	wstring strPath;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
		{
			// Material 이름짓기
			strMtrlName = m_vecContainer[i].vecMtrl[j].strMtrlName;
			if (strMtrlName.empty())
				strMtrlName = path(m_vecContainer[i].vecMtrl[j].strDiff).stem();

			strPath = L"material\\";
			strPath += strMtrlName + L".mtrl";

			// 재질 이름
			m_vecContainer[i].vecMtrl[j].strMtrlName = strPath;

			// 이미 로딩된 재질이면 로딩된 것을 사용
			Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(strPath);
			if (nullptr != pMaterial)
				continue;

			pMaterial = new CMaterial;

			// 상대경로가 곧 키
			pMaterial->SetKey(strPath);
			pMaterial->SetRelativePath(strPath);

			pMaterial->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));

			wstring strTexKey = m_vecContainer[i].vecMtrl[j].strDiff;
			Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_0, pTex);

			strTexKey = m_vecContainer[i].vecMtrl[j].strNormal;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_1, pTex);

			strTexKey = m_vecContainer[i].vecMtrl[j].strSpec;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_2, pTex);

			strTexKey = m_vecContainer[i].vecMtrl[j].strEmis;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_3, pTex);


			pMaterial->SetMaterialCoefficient(
				m_vecContainer[i].vecMtrl[j].tMtrl.vDiff
				, m_vecContainer[i].vecMtrl[j].tMtrl.vSpec
				, m_vecContainer[i].vecMtrl[j].tMtrl.vAmb
				, m_vecContainer[i].vecMtrl[j].tMtrl.vEmv);

			CResMgr::GetInst()->AddRes<CMaterial>(pMaterial->GetKey(), pMaterial.Get());
			pMaterial->Save(strPath);
		}
	}
}



void CFBXLoader::LoadSkeleton(FbxNode* _pNode)
{
	int iChildCount = _pNode->GetChildCount();

	LoadSkeleton_Re(_pNode, 0, 0, -1);
}

void CFBXLoader::LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		tBone* pBone = new tBone;

		string strBoneName = _pNode->GetName();

		pBone->strBoneName = wstring(strBoneName.begin(), strBoneName.end());
		pBone->iDepth = _iDepth++;
		pBone->iParentIndx = _iParentIdx;

		m_vecBone.push_back(pBone);
	}

	int iChildCount = _pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		LoadSkeleton_Re(_pNode->GetChild(i), _iDepth, (int)m_vecBone.size(), _iIdx);
	}
}

void CFBXLoader::LoadAnimationClip()
{
	int iStartidx = 0;
	int iAnimCount = m_arrAnimName.GetCount();

	for (int i = iStartidx; i < iAnimCount; ++i)
	{
		FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());


		//FbxAnimEvaluator* pevaluator = m_pScene->GetAnimationEvaluator();
		//m_pScene->SetCurrentAnimationStack();
		if (!pAnimStack)
			continue;

		tAnimClip* pAnimClip = new tAnimClip;
		int StartIndex = m_Path.rfind("\\") + 1;
		int EndIndex = m_Path.rfind(".");
		int Length = EndIndex - StartIndex;
		string strOwnerName = m_Path.substr(StartIndex,Length);
		string strClipName = pAnimStack->GetName();
		size_t iFindIdx = strClipName.rfind("|") + 1;
		pAnimClip->strName = L"anim3D\\" +wstring(strOwnerName.begin(), strOwnerName.end())+L"\\" + wstring(strClipName.begin() + iFindIdx, strClipName.end()) + L".anm";
		pAnimClip->originName = wstring(strClipName.begin(), strClipName.end());
		FbxTakeInfo * pTakeInfo = m_pScene->GetTakeInfo(pAnimStack->GetName());
		pAnimClip->tStartTime = pTakeInfo->mLocalTimeSpan.GetStart();
		pAnimClip->tEndTime = pTakeInfo->mLocalTimeSpan.GetStop();

		pAnimClip->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount(pAnimClip->eMode) - pAnimClip->tStartTime.GetFrameCount(pAnimClip->eMode);

		m_vecAnimClip.push_back(pAnimClip);
	}
}

void CFBXLoader::Triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr &&
		(pAttr->GetAttributeType() == FbxNodeAttribute::eMesh
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter converter(m_pManager);
		converter.Triangulate(pAttr, true);
	}

	int iChildCount = _pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(_pNode->GetChild(i));
	}
}

void CFBXLoader::LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer)
{
	// Animation Data 로드할 필요가 없음
	int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iSkinCount <= 0 || m_vecAnimClip.empty())
		return;

	_pContainer->bAnimation = true;
	int iContainSize = m_vecContainer.size();
	// Skin 개수만큼 반복을하며 읽는다.	
	for (int i = 0; i < iSkinCount; ++i)
	{
		FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (pSkin)
		{
			FbxSkin::EType eType = pSkin->GetSkinningType();
			if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
			{
				// Cluster 를 얻어온다
				// Cluster == Joint == 관절
				int iClusterCount = pSkin->GetClusterCount();
				for (int j = 0; j < iClusterCount; ++j)
				{
					FbxCluster* pCluster = pSkin->GetCluster(j);

					if (!pCluster->GetLink())
						continue;

					// 현재 본 인덱스를 얻어온다.
					int iBoneIdx = FindBoneIndex(pCluster->GetLink()->GetName());
					if (-1 == iBoneIdx)
						assert(NULL);
					if (iBoneIdx >= iClusterCount)
						break;

					FbxAMatrix matNodeTransform = GetTransform(_pMesh->GetNode());

					// Weights And Indices 정보를 읽는다.
					LoadWeightsAndIndices(pCluster, iBoneIdx, _pContainer);

					if (m_bLoadAni == false)
					{
						// Bone 의 OffSet 행렬 구한다.
						LoadOffsetMatrix(_pMesh, pCluster, matNodeTransform, iBoneIdx);

						// Bone KeyFrame 별 행렬을 구한다.
						LoadKeyframeTransform(_pMesh->GetNode(), pCluster, matNodeTransform, iBoneIdx);
					}
				}
			}
		}
	}
	CheckWeightAndIndices(_pMesh, _pContainer);
	//aniClip Struct Buffer
	if (m_bLoadAni == false)
	{
		AniClipPostProcess();
		m_bLoadAni = true;
	}
}


void CFBXLoader::CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer)
{
	vector<vector<tWeightsAndIndices>>::iterator iter = _pContainer->vecWI.begin();

	int iVtxIdx = 0;
	for (iter; iter != _pContainer->vecWI.end(); ++iter, ++iVtxIdx)
	{
		if ((*iter).size() > 1)
		{
			// 가중치 값 순으로 내림차순 정렬
			sort((*iter).begin(), (*iter).end()
				, [](const tWeightsAndIndices& left, const tWeightsAndIndices& right)
				{
					return left.dWeight > right.dWeight;
				}
			);

			double dWeight = 0.f;
			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				dWeight += (*iter)[i].dWeight;
			}

			// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
			double revision = 0.f;
			if (dWeight > 1.0)
			{
				revision = 1.0 - dWeight;
				(*iter)[0].dWeight += revision;
			}

			if ((*iter).size() >= 4)
			{
				(*iter).erase((*iter).begin() + 4, (*iter).end());
			}
		}

		// 정점 정보로 변환, 
		float fWeights[4] = {};
		float fIndices[4] = {};

		for (UINT i = 0; i < (*iter).size(); ++i)
		{
			fWeights[i] = (float)(*iter)[i].dWeight;
			fIndices[i] = (float)(*iter)[i].iBoneIdx;
		}

		memcpy(&_pContainer->vecWeights[iVtxIdx], fWeights, sizeof(Vec4));
		memcpy(&_pContainer->vecIndices[iVtxIdx], fIndices, sizeof(Vec4));
	}
}

void CFBXLoader::LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster
	, const FbxAMatrix& _matNodeTransform, int _iBoneIdx)
{
	if (m_vecAnimClip.empty())
		return;

	FbxVector4   v1 = { 1, 0, 0, 0 };
	FbxVector4   v2 = { 0, 0, 1, 0 };
	FbxVector4   v3 = { 0, 1, 0, 0 };
	FbxVector4   v4 = { 0, 0, 0, 1 };
	FbxAMatrix   matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	m_vecBone[_iBoneIdx]->matBone = _matNodeTransform;

	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
	{
		// AnimClip 이름으로 AnimStack 을 가져온다.
		string strAnimName = string(m_vecAnimClip[i]->originName.begin(), m_vecAnimClip[i]->originName.end());
		FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>(strAnimName.c_str());

		// 애니메이션 행렬 정보를 받아올 AnimStack 을 설정한다.      
		m_pScene->SetCurrentAnimationStack(pAnimStack);

		// AnimClip 의 행렬을 연산해서 각 뼈의 KeyFrame 행렬에 추가한다.
		FbxLongLong llStartFrame = m_vecAnimClip[i]->tStartTime.GetFrameCount(eTimeMode);
		FbxLongLong llEndFrame = m_vecAnimClip[i]->tEndTime.GetFrameCount(eTimeMode);

		vector<tKeyFrame> vecKeyFrames = {};
		for (FbxLongLong j = llStartFrame; j < llEndFrame; ++j)
		{
			tKeyFrame tFrame = {};
			FbxTime   tTime = 0;

			tTime.SetFrame(j, eTimeMode);

			FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _matNodeTransform;
			FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);
			matCurTrans = matReflect * matCurTrans * matReflect;

			tFrame.dTime = tTime.GetSecondDouble();
			tFrame.matTransform = matCurTrans;
			vecKeyFrames.push_back(tFrame);
		}

		CAniClip::CreateBoneFrameData(*this, m_vecAnimClip[i], vecKeyFrames, _iBoneIdx);
	}
}

void CFBXLoader::AniClipPostProcess()
{
	int iClipSize = m_vecAnimClip.size();
	for (int i = 0; i < iClipSize; ++i)
	{
		wstring strClipPath = m_vecAnimClip[i]->strName;
		Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(strClipPath);
		pClip->CreateStructBuffer();
		pClip->SetName(m_vecAnimClip[i]->strName);
		pClip->SetKey(strClipPath);
		pClip->SetRelativePath(strClipPath);

		pClip->Save(strClipPath);
	}
}
void CFBXLoader::LoadOffsetMatrix(FbxMesh* _pMesh, FbxCluster* _pCluster
	, const FbxAMatrix& _matNodeTransform
	, int _iBoneIdx)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;
	bool flag = false;

	_pCluster->GetTransformMatrix(matClusterTrans);
	_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;

	//matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _matNodeTransform;
	//matOffset = matReflect * matOffset * matReflect;

	FbxAMatrix globalMat = matClusterTrans * _matNodeTransform;
	matClusterLinkTrans = globalMat.Inverse() * matClusterLinkTrans;
	matOffset = matClusterLinkTrans.Inverse();
	matOffset = matReflect * matOffset * matReflect;
	m_vecBone[_iBoneIdx]->matOffset = matOffset;
}


void CFBXLoader::LoadWeightsAndIndices(FbxCluster* _pCluster
	, int _iBoneIdx
	, tContainer* _pContainer)
{
	int iIndicesCount = _pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iIndicesCount; ++i)
	{
		tWeightsAndIndices tWI = {};

		// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
		tWI.iBoneIdx = _iBoneIdx;
		tWI.dWeight = _pCluster->GetControlPointWeights()[i];

		int iVtxIdx = _pCluster->GetControlPointIndices()[i];

		_pContainer->vecWI[iVtxIdx].push_back(tWI);
	}
}



int CFBXLoader::FindBoneIndex(string _strBoneName)
{
	wstring strBoneName = wstring(_strBoneName.begin(), _strBoneName.end());

	for (UINT i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i]->strBoneName == strBoneName)
			return i;
	}

	return -1;
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

FbxCluster* CFBXLoader::GetParrentCluster(FbxMesh* _pMesh, FbxCluster* _pCluster)
{
	if (!_pCluster)
		return nullptr;

	// Get the linked node of the current cluster
	FbxNode* pLinkedNode = _pCluster->GetLink();
	if (!pLinkedNode)
		return nullptr; // If the cluster is not linked to any node, return nullptr

	// Traverse upwards in the bone hierarchy
	FbxNode* pParentNode = pLinkedNode->GetParent();

	int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iSkinCount <= 0 || m_vecAnimClip.empty())
		return nullptr;

	// Skin 개수만큼 반복을하며 읽는다.	
	for (int i = 0; i < iSkinCount; ++i)
	{
		FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (pSkin)
		{
			FbxSkin::EType eType = pSkin->GetSkinningType();
			if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
			{
				// Cluster 를 얻어온다
				// Cluster == Joint == 관절
				int iClusterCount = pSkin->GetClusterCount();
				for (int j = 0; j < iClusterCount; ++j)
				{
					FbxCluster* pCluster = pSkin->GetCluster(j);

					if (!pCluster->GetLink())
						continue;

					if (strcmp(pCluster->GetName(), pParentNode->GetName()) != 0)
						continue;
					else return pCluster;
				}
			}
		}
	}
	return nullptr;
}

FbxCluster* CFBXLoader::FindParrentCluster(FbxNode* _pNode)
{
	if (!_pNode)
		return nullptr;

	// Check if the node has any cluster associated with it
	for (int i = 0; i < _pNode->GetChildCount(); ++i) {
		FbxNode* pChildNode = _pNode->GetChild(i);
		FbxCluster* pCluster = FindParrentCluster(pChildNode);
		if (pCluster)
			return pCluster; // Return the first cluster found in the node hierarchy
	}
	// If no cluster is found, return nullptr
	return nullptr;
}
