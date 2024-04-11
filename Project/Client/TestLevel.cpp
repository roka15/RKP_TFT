#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include "CLevelSaveLoad.h"

#include <Engine\CTileMap.h>
#include <Engine/CSetColorShader.h>
#include <Engine\AnimatorController.h>
#include <Engine\Transition.h>

#include <Script\CScriptMgr.h>
#include <Script\CAStarMgr.h>
#include <Script\CGameMgr.h>
#include <Script\CGame.h>
#include <Script\CAttroxMachineScript.h>
#include <Script\CBaseCharacterScript.h>
#include <Script\CCharacterTrigger.h>
#include <Script\CTileMgr.h>
#include <Script\CTileScript.h>
#include <Script\CPlayerScript.h>


void CreateTestLevel()
{
	CGameObject* pPlayer = new CGameObject();
	pPlayer->SetName(L"Player01");
	pPlayer->AddComponent(new CTransform());
	//pPlayer->AddComponent(new CMeshRender());
	pPlayer->AddComponent(new CPlayerScript());
	pPlayer->GetScript<CPlayerScript>()->SetPlayerType(PLAYER_TYPE::CLIENT);

	SpawnGameObject(pPlayer, Vec3{ 0.f,0.f,0.f }, 0);
	CGameMgr::GetInst()->EnterGame(0, pPlayer);
	CGameMgr::GetInst()->SetClientOwner(pPlayer);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.
	pMainCam->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(60.f),0.f,0.f });
	pMainCam->Camera()->SetFar(100000.f);
	

	SpawnGameObject(pMainCam, Vec3{ 800.f,1450.f,-900.f }, 0);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");

	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);		// Sub 카메라로 지정
	pUICam->Camera()->SetLayerMask(31, true);	// 31번 레이어만 체크

	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


	// SkyBox 추가
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\riots_sru_skybox_cubemap.dds"));

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);

	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(103), DEGREE2RADIAN(-176), 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(100.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	SpawnGameObject(pLightObj, Vec3(0, 10000.f, -2000.f), 0);

	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Directional Light");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);


	SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);

	// ============
	// FBX Loading
	// ============	


	CGameObject* pAttroxObj = nullptr;
	CGameObject* pTestEnemy = nullptr;
	CGameObject* pTestEnemy2 = nullptr;
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pMap = new CGameObject();
	pMap->AddComponent(new CTransform());
	pMap->Transform()->SetRelativePos(Vec3(1.0f, 1.0f, -33.000));
	pMap->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-2), 0.f, 0.f));
	
#pragma region Map Object 
	{
		pMap->SetName(L"MapObjects");
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_terrain.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Ground");
		pObj->Transform()->SetRelativePos(Vec3(800, -200.f, 900.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), 0.f, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));
		pMap->AddChild(pObj);
        #pragma region Rock
		CGameObject* Rock_List = new CGameObject();
		Rock_List->SetName(L"RockParent");
		Rock_List->AddComponent(new CTransform());
		pMap->AddChild(Rock_List);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_terrain_rocks.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Ground_Rock");
		pObj->Transform()->SetRelativePos(Vec3(800, -200.f, 755.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), 0.f, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.0f, 1.0f, 1.0f));
		Rock_List->AddChild(pObj);
		
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock1");
		pObj->Transform()->SetRelativePos(Vec3(-8.f, -200.f, 2649.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-32), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.2f, 1.2f, 1.2f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock2");
		pObj->Transform()->SetRelativePos(Vec3(-1045.f, -200.f, 2.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-79), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.6f, 0.6f, 1.f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock3.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock3");
		pObj->Transform()->SetRelativePos(Vec3(2752.f, -200.f, -111.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(106), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.9f, 0.8f, 1.0f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock4");
		pObj->Transform()->SetRelativePos(Vec3(-605.f, -200.f, -1076.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(342), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.9f, 0.8f, 1.0f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock5");
		pObj->Transform()->SetRelativePos(Vec3(-1137.f, -200.f, -822.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(91), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.4f, 0.3f, 1.0f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock3.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock6");
		pObj->Transform()->SetRelativePos(Vec3(-1017.f, -190.f, -945.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(121), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.155f, 0.2f, 0.2));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock7");
		pObj->Transform()->SetRelativePos(Vec3(-809.f, -190.f, -457.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(106), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.4f, 0.3f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock8");
		pObj->Transform()->SetRelativePos(Vec3(-843.f, -200.f, 1456.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(20), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.4f, 0.4f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock3.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock9");
		pObj->Transform()->SetRelativePos(Vec3(-678.f, -190.f, 142.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-59), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.3f, 0.3f, 0.5f));
		Rock_List->AddChild(pObj);

	    pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock10");
		pObj->Transform()->SetRelativePos(Vec3(-704.f, -190.f,1357.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(53), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock11");
		pObj->Transform()->SetRelativePos(Vec3(-605.f, -204.f, 1590.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(169), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.1f, 0.1f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock3.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock12");
		pObj->Transform()->SetRelativePos(Vec3(-911.f, -200.f, -2638.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(40), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.8f, 0.7f, 0.7f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock13");
		pObj->Transform()->SetRelativePos(Vec3(-740.f, -190.f, 2170.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-69), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.3f, 0.3f, 0.5f));
		Rock_List->AddChild(pObj);


		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock14");
		pObj->Transform()->SetRelativePos(Vec3(2702.f, -203.f, 1990.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(126), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.8f, 0.65f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock15");
		pObj->Transform()->SetRelativePos(Vec3(2696.f, -205.f, 1322.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(79), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.65f, 0.7f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock3.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock16");
		pObj->Transform()->SetRelativePos(Vec3(2436.f, -198.f, 527.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-33), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock17");
		pObj->Transform()->SetRelativePos(Vec3(2457.f, -200.f, 363.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(180), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.15f, 0.15f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock18");
		pObj->Transform()->SetRelativePos(Vec3(2749.f, -195.f, 563.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(229), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.25f, 0.25f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock19");
		pObj->Transform()->SetRelativePos(Vec3(2327.f, -196.f, -876.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(123), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.3f, 0.25f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock3.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock20");
		pObj->Transform()->SetRelativePos(Vec3(2068.f, -202.f, -919.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(145), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.125f, 0.125f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock21");
		pObj->Transform()->SetRelativePos(Vec3(1666.f, -208.f, -846.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-63), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.09f, 0.08f, 0.5f));
		Rock_List->AddChild(pObj);


		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock22");
		pObj->Transform()->SetRelativePos(Vec3(1419.f, -199.f, -684.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-63), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock23");
		pObj->Transform()->SetRelativePos(Vec3(284.f, -202.f, -715.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(200), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.125f, 0.1f, 0.5f));
		Rock_List->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Rock2.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Rock24");
		pObj->Transform()->SetRelativePos(Vec3(33.f, -208.f, -897.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(127), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.4f, 0.4f, 0.5f));
		Rock_List->AddChild(pObj);

        #pragma endregion

        #pragma region Tree

		CGameObject* TreeList = new CGameObject();
		TreeList->SetName(L"TreeParent");
		TreeList->AddComponent(new CTransform());
		pMap->AddChild(TreeList);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Tree.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Tree1");
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), 0.f, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.0f, 1.0f, 1.0f));
		const vector<CGameObject*> treeChilds = pObj->GetChild();
		treeChilds[1]->Transform()->SetRelativePos(Vec3{128,-70,540});
		treeChilds[1]->Transform()->SetRelativeScale(Vec3{ 3,3,3 });
		treeChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(24),0,0 });
		
		pObj->Transform()->SetRelativePos(Vec3(2595, -167, -836));
		pObj->Transform()->SetRelativeScale(Vec3{ 1,1,1 });
		pObj->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-90),DEGREE2RADIAN(-88),0 });
		TreeList->AddChild(pObj);

		CGameObject* Tree = pObj->Clone();
		Tree->SetName(L"Tree2");
		Tree->Transform()->SetRelativePos(Vec3{ 2619,-184,854 });
		Tree->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.2f });
		Tree->Transform()->SetRelativeRot(Vec3{DEGREE2RADIAN(-90),DEGREE2RADIAN(39),0 });
		TreeList->AddChild(Tree);

		Tree = pObj->Clone();
		Tree->SetName(L"Tree3");
		Tree->Transform()->SetRelativePos(Vec3{ -937,-184,252 });
		Tree->Transform()->SetRelativeScale(Vec3{ 0.7,0.7,0.7 });
		Tree->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-90),DEGREE2RADIAN(98),0 });
		TreeList->AddChild(Tree);

		Tree = pObj->Clone();
		Tree->SetName(L"Tree4");
		Tree->Transform()->SetRelativePos(Vec3{ -856,-184,1832 });
		Tree->Transform()->SetRelativeScale(Vec3{ 0.7,0.7,0.7 });
		Tree->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-90),DEGREE2RADIAN(293),0 });
		TreeList->AddChild(Tree);

		Tree = pObj->Clone();
		Tree->SetName(L"Tree5");
		Tree->Transform()->SetRelativePos(Vec3{ -1068,-184,2379 });
		Tree->Transform()->SetRelativeScale(Vec3{ 0.9,0.9,0.9 });
		Tree->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-90),DEGREE2RADIAN(154 /*514*/),0 });
		TreeList->AddChild(Tree);

        #pragma endregion

		#pragma region Grass
		CGameObject* GrassList = new CGameObject();
		GrassList->SetName(L"GrassParent");
		GrassList->AddComponent(new CTransform());
		pMap->AddChild(GrassList);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Grass.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Grass1");
		pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), 0.f, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.0f, 1.0f, 1.0f));
		
		CGameObject* CopyGrass = pObj->Clone();

		vector<CGameObject*>pChilds = pObj->GetChild();
		pChilds[0]->Transform()->SetRelativePos(Vec3{ -607.f,-185.f,-112.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(35.f),0.f });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ -711.000f,-538.f,-123.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 0.7f,0.7f,0.7f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(13.f),0.f });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ -1085.0f,182.f,-200.f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(-19.f),0.f });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ 1148.f,837.f,-200.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(-19.f),0.f });
		
		pChilds[4]->Transform()->SetRelativePos(Vec3{ 662.f,776.f,-200.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(-6.f),DEGREE2RADIAN(-91.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ -468.f,-1482.f,-200.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(11.f),DEGREE2RADIAN(2.f),DEGREE2RADIAN(-109.f) });

		GrassList->AddChild(pObj);
		pChilds.clear();

		pObj = CopyGrass;
		Ptr<CMaterial> GrassType1Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\tree leaf.mtrl");
		pObj->SetName(L"Grass2");
		pChilds = pObj->GetChild();
		for (int i = 0; i < pChilds.size(); ++i)
		{
			pChilds[i]->MeshRender()->SetMaterial(GrassType1Mtrl, 0);
		}
		
		pChilds[0]->Transform()->SetRelativePos(Vec3{ -1216.f,638.f,-200.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(6.f),DEGREE2RADIAN(30.f),DEGREE2RADIAN(9.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ -665.000f,-876.f,-241.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 4.0f,4.0f,4.0f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(13.f),0.f });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ -628.0f,-488.f,-200.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 2.5f,2.5f,2.5f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-6.f),DEGREE2RADIAN(11.f),DEGREE2RADIAN(-4.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ -707.f,308.f,-172.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{5.f, 5.f ,5.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ 0.f,DEGREE2RADIAN(-8.f),0.f });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ -902.f,-1684.f,-200.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 1.5f, 1.5f ,1.5f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(22.f),DEGREE2RADIAN(-71.f),DEGREE2RADIAN(0.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ -759.f,-1838.f,-200.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 1.5f, 1.5f ,1.5f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-3.f),DEGREE2RADIAN(1.f),DEGREE2RADIAN(28.f) });

		GrassList->AddChild(pObj);
		pChilds.clear();

		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass3");
		GrassList->AddChild(pObj);

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ -739.f,-1697.f,-200.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-8.f),DEGREE2RADIAN(22.f),DEGREE2RADIAN(0.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ -327.000f,-1483.f,-204.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 2.5f,2.5f,2.5f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-18.f),DEGREE2RADIAN(34.f),DEGREE2RADIAN(30.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ -946.0f,-629.f,-200.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(3.f),DEGREE2RADIAN(18.f),DEGREE2RADIAN(4.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ -915.f,-443.f,-179.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 2.f, 2.f ,2.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-13.f),DEGREE2RADIAN(28.f),DEGREE2RADIAN(-2.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ -771.f,-1358.f,-200.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(2.f),DEGREE2RADIAN(-8.f),DEGREE2RADIAN(-80.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ -252.f,-2176.f,-200.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-3.f),DEGREE2RADIAN(1.f),DEGREE2RADIAN(28.f) });

		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass4");
		GrassList->AddChild(pObj);
		pChilds.clear();

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ -42.f,684.f,-125.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(8.f),DEGREE2RADIAN(7.f),DEGREE2RADIAN(6.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ -216.0f,862.f,-229.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 2.5f,2.5f,2.5f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(22.f),DEGREE2RADIAN(16.f),DEGREE2RADIAN(18.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ -321.0f,907.f,-200.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(18.f),DEGREE2RADIAN(39.f),DEGREE2RADIAN(4.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ -902.f,974.f,-236.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(9.f),DEGREE2RADIAN(30.f),DEGREE2RADIAN(-2.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ -979.f,895.f,-199.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(7.f),DEGREE2RADIAN(24.f),DEGREE2RADIAN(-71.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ -1058.f,885.f,-214.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(32.f),DEGREE2RADIAN(24.f),DEGREE2RADIAN(26.f) });

		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass5");
		GrassList->AddChild(pObj);
		pChilds.clear();

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ 415.f,868.f,-125.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(8.f),DEGREE2RADIAN(7.f),DEGREE2RADIAN(62.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ 660.0f,688.f,-229.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 2.5f,2.5f,2.5f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(13.f),DEGREE2RADIAN(-23.f),DEGREE2RADIAN(18.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ 331.0f,733.f,-216.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(18.f),DEGREE2RADIAN(14.f),DEGREE2RADIAN(44.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ 812.f,860.f,-236.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(15.f),DEGREE2RADIAN(2.f),DEGREE2RADIAN(-2.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ 1131.f,653.f,-123.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 2.f, 2.f ,2.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(2.f),DEGREE2RADIAN(-15.f),DEGREE2RADIAN(-40.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ 1144.f,649.f,-149.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 2.f, 2.f ,2.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(7.f),DEGREE2RADIAN(-9.f),DEGREE2RADIAN(48.f) });

		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass6");
		GrassList->AddChild(pObj);
		pChilds.clear();

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ 1458.f,667.f,-165.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 4.f,4.f,4.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(6.f),DEGREE2RADIAN(15.f),DEGREE2RADIAN(9.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ 1927.0f,667.f,-155.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 4.f,4.f,4.f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-14.f),DEGREE2RADIAN(-18.f),DEGREE2RADIAN(47.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ 2302.0f,800.f,-200.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-6.f),DEGREE2RADIAN(-25.f),DEGREE2RADIAN(-4.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ 2436.f,980.f,-259.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(0.f),DEGREE2RADIAN(-8.f),DEGREE2RADIAN(0.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ 2261.f,-145.f,-136.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(22.f),DEGREE2RADIAN(-14.f),DEGREE2RADIAN(-36.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ 2404.f,453.f,-190.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 4.f, 4.f ,4.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(12.f),DEGREE2RADIAN(-3.f),DEGREE2RADIAN(20.f) });

		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass7");
		GrassList->AddChild(pObj);
		pChilds.clear();

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ 2694.f,365.f,-180.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 4.f,4.f,4.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(6.f),DEGREE2RADIAN(15.f),DEGREE2RADIAN(-17.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ 2646.0f,-68.f,-241.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 3.f,3.f,3.f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-15.f),DEGREE2RADIAN(-9.f),DEGREE2RADIAN(47.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ 2685.0f,-366.f,-211.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(13.f),DEGREE2RADIAN(-40.f),DEGREE2RADIAN(17.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ 2537.f,-711.f,-227.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 2.f, 2.f ,2.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-22.f),DEGREE2RADIAN(-13.f),DEGREE2RADIAN(65.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ 2743.f,-650.f,-168.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-14.f),DEGREE2RADIAN(-94.f),DEGREE2RADIAN(151.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ 2263.f,-1219.f,-149.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(12.f),DEGREE2RADIAN(-3.f),DEGREE2RADIAN(87.f) });


		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass8");
		GrassList->AddChild(pObj);
		pChilds.clear();

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ 2320.f,-1002.f,-202.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-14.f),DEGREE2RADIAN(0.f),DEGREE2RADIAN(-65.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ 2683.0f,-2211.f,-241.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 3.f,3.f,3.f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-15.f),DEGREE2RADIAN(-9.f),DEGREE2RADIAN(47.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ 2943.0f,-2598.f,-211.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 2.f,2.f,2.f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(9.f),DEGREE2RADIAN(-7.f),DEGREE2RADIAN(-73.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ 1882.f,-2799.f,-227.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 4.f, 4.f ,4.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(7.f),DEGREE2RADIAN(28.f),DEGREE2RADIAN(65.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ 795.f,-2161.f,-168.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(0.f),DEGREE2RADIAN(-31.f),DEGREE2RADIAN(-78.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ 1839.f,-2113.f,-149.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(12.f),DEGREE2RADIAN(-3.f),DEGREE2RADIAN(87.f) });

		pObj = CopyGrass->Clone();
		pObj->SetName(L"Grass9");
		GrassList->AddChild(pObj);
		pChilds.clear();

		pChilds = pObj->GetChild();

		pChilds[0]->Transform()->SetRelativePos(Vec3{ 244.f,-2754.f,-202.f });
		pChilds[0]->Transform()->SetRelativeScale(Vec3{ 3.f,3.f,3.f });
		pChilds[0]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-24.f),DEGREE2RADIAN(-6.f),DEGREE2RADIAN(-65.f) });

		pChilds[1]->Transform()->SetRelativePos(Vec3{ 1728.0f,-2697.f,-241.f });
		pChilds[1]->Transform()->SetRelativeScale(Vec3{ 3.f,3.f,3.f });
		pChilds[1]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-15.f),DEGREE2RADIAN(-9.f),DEGREE2RADIAN(47.f) });

		pChilds[2]->Transform()->SetRelativePos(Vec3{ 1852.0f,-2580.f,-211.f });
		pChilds[2]->Transform()->SetRelativeScale(Vec3{ 3.f,3.f,3.f });
		pChilds[2]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(9.f),DEGREE2RADIAN(-35.f),DEGREE2RADIAN(-73.f) });

		pChilds[3]->Transform()->SetRelativePos(Vec3{ 934.f,-2633.f,-244.f });
		pChilds[3]->Transform()->SetRelativeScale(Vec3{ 5.f, 5.f ,5.f });
		pChilds[3]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-16.f),DEGREE2RADIAN(-4.f),DEGREE2RADIAN(65.f) });

		pChilds[4]->Transform()->SetRelativePos(Vec3{ 142.f,-2428.f,-168.f });
		pChilds[4]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[4]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(0.f),DEGREE2RADIAN(-39.f),DEGREE2RADIAN(-45.f) });

		pChilds[5]->Transform()->SetRelativePos(Vec3{ 116.f,-2413.f,-149.f });
		pChilds[5]->Transform()->SetRelativeScale(Vec3{ 3.f, 3.f ,3.f });
		pChilds[5]->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(28.f),DEGREE2RADIAN(-87.f),DEGREE2RADIAN(-36.f) });

		#pragma endregion
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Structures.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Structures1");
		pObj->Transform()->SetRelativePos(Vec3(-500.f, -100.f, -450.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), 0.f, 0.f));
		CGameObject* TileSet = CTileMgr::GetInst()->CreateTile(TILE_OWNER_TYPE::PLAYER);
		TileSet->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(90), 0.f, 0.f));
		pObj->AddChild(TileSet);
		pMap->AddChild(pObj);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MU_Structures.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Structures2");
		pObj->Transform()->SetRelativePos(Vec3(2225.f, -100.f, 1910.f));
		pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(-90), DEGREE2RADIAN(-180), 0.f));
		TileSet = CTileMgr::GetInst()->CreateTile(TILE_OWNER_TYPE::ENEMY);
		TileSet->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(90), 0.f, 0.f));
		pObj->AddChild(TileSet);
		pMap->AddChild(pObj);
	}
	SpawnGameObject(pMap, Vec3{ 0.f,0.f,0.f }, 0);
#pragma endregion


	//pTestEnemy = new CGameObject();
	//pTestEnemy->SetName(L"TestEnemy1");
	//pTestEnemy->AddComponent(new CTransform());
	//pTestEnemy->AddComponent(new CMeshRender());
	//pTestEnemy->Transform()->SetRelativeScale(Vec3{ 100.f,100.f,100.f });
	//pTestEnemy->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pTestEnemy->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	//pTestEnemy->AddComponent(new CBaseCharacterScript());
	//pTestEnemy2 = new CGameObject();
	//pTestEnemy2->SetName(L"TestEnemy2");
	//pTestEnemy2->AddComponent(new CTransform());
	//pTestEnemy2->AddComponent(new CMeshRender());
	//pTestEnemy2->Transform()->SetRelativeScale(Vec3{ 100.f,100.f,100.f });
	//pTestEnemy2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pTestEnemy2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	//pTestEnemy2->AddComponent(new CBaseCharacterScript());
	//{
	//	CTileMgr::GetInst()->RegisterItem(51, pTestEnemy);
	//	CTileMgr::GetInst()->RegisterItem(56, pTestEnemy2);
	//}
	

	//CGameObject* pNewObj = new CGameObject;
	//pNewObj->SetName(L"Cube1");
	//pNewObj->AddComponent(new CTransform);
	//pNewObj->AddComponent(new CMeshRender);
	//pNewObj->AddComponent(new CBoxCollider);
	//CCollider3D* cubeCollider = (CCollider3D*)pNewObj->GetComponent(COMPONENT_TYPE::COLLIDER3D);
	//cubeCollider->SetTrigger(true);
	//pNewObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pNewObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	////pNewObj->MeshRender()->GetMaterial(0)->SetTexParam()

	//pNewObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	//SpawnGameObject(pNewObj, Vec3(0.f, 0.f, 0.f), 2);


 //   pNewObj = new CGameObject;
	//pNewObj->SetName(L"Cube2");
	//pNewObj->AddComponent(new CTransform);
	//pNewObj->AddComponent(new CMeshRender);
	//pNewObj->AddComponent(new CBoxCollider);
	//pNewObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pNewObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	////pNewObj->MeshRender()->GetMaterial(0)->SetTexParam()

	//pNewObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	//SpawnGameObject(pNewObj, Vec3(0.f, 0.f, 0.f), 3);

	//{
	//	// 인스턴싱 테스트
	//	Ptr<CMeshData> pMeshData = nullptr;
	//	CGameObject* pObj = nullptr;
	//		
	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\AhriSpell1.fbx");
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		pObj = pMeshData->Instantiate();
	//		pObj->SetName(L"House");
	//		SpawnGameObject(pObj, Vec3((i + 1) * 300.f, 200.f, 500.f), 0);
	//	}

	//	/*pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");		
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		pObj = pMeshData->Instantiate();
	//		pObj->SetName(L"Monster");
	//		SpawnGameObject(pObj, Vec3((i + 1) * 50.f, 200.f, 500.f), 0);
	//	}*/
	//}


	//// LandScape Object
	//CGameObject* pLandScape = new CGameObject;
	//pLandScape->SetName(L"LandScape");

	//pLandScape->AddComponent(new CTransform);
	//pLandScape->AddComponent(new CLandScape);

	//pLandScape->Transform()->SetRelativeScale(Vec3(500.f, 3000.f, 500.f));

	//pLandScape->LandScape()->SetFace(64, 64);
	//pLandScape->LandScape()->SetFrustumCheck(false); 
	////pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));

	//SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 0);
	CPlayerScript* pPlayerScript = pPlayer->GetScript<CPlayerScript>();
	CGameMgr::GetInst()->BuyItem(pPlayerScript->GetGameID(), CHARACTER_TYPE::ATTROX, pPlayer);
	CGameMgr::GetInst()->BuyItem(pPlayerScript->GetGameID(), CHARACTER_TYPE::ATTROX, pPlayer);
	/*CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::);
	script->notify(&trigger);*/

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");


	//UI Test
	Ptr<CPrefab> pPrefab = nullptr;

	CGameObject* pRoundUI = new CGameObject();
	pRoundUI->AddComponent(new CTransform());
	pRoundUI->SetName(L"RoundUI");
	SpawnGameObject(pRoundUI, Vec3(0.f, 0.f, 0.f), 31);

	CGameObject* pImageObj = new CGameObject();
	pImageObj->SetName(L"Image1");
	pImageObj->AddComponent(new CTransform());
	pImageObj->AddComponent(new CMeshRender());
	pImageObj->AddComponent(new CImage());
	pImageObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pImageObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"),0);
	pImageObj->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_RoundBackGround.png");
	pImageObj->Image()->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	pImageObj->Transform()->SetRelativePos(3.f, 340.f, 0.f);
	pImageObj->Transform()->SetRelativeScale(550.f, 100.f, 0.f);
	pRoundUI->AddChild(pImageObj);

	CGameObject* pTextObj = new CGameObject();
	pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UI_Text");
	pTextObj = pPrefab->Instantiate();
	pTextObj->SetName(L"Text");
	pTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pTextObj->Text()->SetText(L"1 - 1");
	pTextObj->Text()->SetSize(20.f);
	pTextObj->Text()->SetColor(Vec4(143.f, 115.f, 58.f, 255.f));
	pTextObj->Transform()->SetRelativePos(465.f, 29.f, 0.f);
	pRoundUI->AddChild(pTextObj);

	CGameObject* pTextObj2 = new CGameObject();
	pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UI_Text");
	pTextObj2 = pPrefab->Instantiate();
	pTextObj2->SetName(L"Text");
	pTextObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTextObj2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pTextObj2->Text()->SetText(L"30");
	pTextObj2->Text()->SetSize(20.f);
	pTextObj2->Text()->SetColor(Vec4(143.f, 115.f, 58.f, 255.f));
	pTextObj2->Transform()->SetRelativePos(815.f, 29.f, 0.f);
	pRoundUI->AddChild(pTextObj2);

	CGameObject* pImageObj2 = new CGameObject();
	pImageObj2->SetName(L"Image2");
	pImageObj2->AddComponent(new CTransform());
	pImageObj2->AddComponent(new CMeshRender());
	pImageObj2->AddComponent(new CImage());
	pImageObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	wstring MtrlName = L"material\\UI_RoundTimeGauge.mtrl";
	Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(MtrlName);
	if (pMaterial == nullptr)
	{
		pMaterial = new CMaterial();
		pMaterial->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DGaugeUIShader"));
		CResMgr::GetInst()->AddRes<CMaterial>(MtrlName, pMaterial);
		pMaterial->Save(MtrlName);
	}
	pImageObj2->MeshRender()->SetMaterial(pMaterial, 0);
	pImageObj2->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_RoundTimeGauge.png");
	pImageObj2->Image()->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	pImageObj2->Transform()->SetRelativePos(-1.f, 310.f, 0.f);
	pImageObj2->Transform()->SetRelativeScale(405.f, 21.f, 0.f);
	pRoundUI->AddChild(pImageObj2);

	CGameObject* pImageObj6 = new CGameObject();
	pImageObj6->SetName(L"Image");
	pImageObj6->AddComponent(new CTransform());
	pImageObj6->AddComponent(new CMeshRender());
	pImageObj6->AddComponent(new CImage());
	pImageObj6->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pImageObj6->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pImageObj6->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_CurRoundBackGround.png");
	pImageObj6->Image()->SetColor(Vec4(0.25f, 0.25f, 0.25f, 1.f));
	pImageObj6->Transform()->SetRelativePos(-71.f, 341.f, 0.f);
	pImageObj6->Transform()->SetRelativeScale(50.f, 50.f, 0.f);
	pRoundUI->AddChild(pImageObj6);

	CGameObject* pImageObj3 = new CGameObject();
	pImageObj3->SetName(L"Image3");
	pImageObj3->AddComponent(new CTransform());
	pImageObj3->AddComponent(new CMeshRender());
	pImageObj3->AddComponent(new CImage());
	pImageObj3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pImageObj3->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pImageObj3->Image()->SetNormalTexKey(L"texture\\UI\\Icon\\announce_icon_combat_white.png");
	pImageObj3->Image()->SetEnableTexKey(L"texture\\UI\\Icon\\announce_icon_combat.png");
	pImageObj3->Image()->SetColor(Vec4(0.25f, 0.25f, 0.25f, 1.f));
	pImageObj3->Transform()->SetRelativePos(-71.f, 345.f, 0.f);
	pImageObj3->Transform()->SetRelativeScale(25.f, 25.f, 0.f);
	pRoundUI->AddChild(pImageObj3);

	CGameObject* pImageObj4 = new CGameObject();
	pImageObj4->SetName(L"Image4");
	pImageObj4->AddComponent(new CTransform());
	pImageObj4->AddComponent(new CMeshRender());
	pImageObj4->AddComponent(new CImage());
	pImageObj4->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pImageObj4->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pImageObj4->Image()->SetNormalTexKey(L"texture\\UI\\Icon\\announce_icon_combat_white.png");
	pImageObj4->Image()->SetEnableTexKey(L"texture\\UI\\Icon\\announce_icon_combat.png");
	pImageObj4->Image()->SetColor(Vec4(0.25f, 0.25f, 0.25f, 1.f));
	pImageObj4->Transform()->SetRelativePos(3.f, 345.f, 0.f);
	pImageObj4->Transform()->SetRelativeScale(25.f, 25.f, 0.f);
	pRoundUI->AddChild(pImageObj4);

	CGameObject* pImageObj5 = new CGameObject();
	pImageObj5->SetName(L"Image5");
	pImageObj5->AddComponent(new CTransform());
	pImageObj5->AddComponent(new CMeshRender());
	pImageObj5->AddComponent(new CImage());
	pImageObj5->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pImageObj5->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	pImageObj5->Image()->SetNormalTexKey(L"texture\\UI\\Icon\\announce_icon_combat_white.png");
	pImageObj5->Image()->SetEnableTexKey(L"texture\\UI\\Icon\\announce_icon_combat.png");
	pImageObj5->Image()->SetColor(Vec4(0.25f, 0.25f, 0.25f, 1.f));
	pImageObj5->Transform()->SetRelativePos(76.f, 345.f, 0.f);
	pImageObj5->Transform()->SetRelativeScale(25.f, 25.f, 0.f);
	pRoundUI->AddChild(pImageObj5);


	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"ROUND_TEXT", pTextObj);
	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"TIMER_TEXT", pTextObj2);
	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"TIMER_IMAGE", pImageObj2);
	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"ROUND_IMAGE1", pImageObj3);
	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"ROUND_IMAGE2", pImageObj4);
	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"ROUND_IMAGE3", pImageObj5);
	CGameMgr::GetInst()->RegisterUImap(pPlayerScript->GetGameID(), L"CUR_ROUND_IMAGE", pImageObj6);
	
}
