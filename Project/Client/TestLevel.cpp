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
#include <Script\CAttroxMachineScript.h>
#include <Script\CBaseCharacterScript.h>
#include <Script\CCharacterTrigger.h>
#include <Script\CTileMgr.h>
void CreateTestLevel()
{
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

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

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

	pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(500.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);

	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Directional Light");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);


	SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);

	CTileMgr::GetInst()->SetInfo(Vec2{ 250,220 }, Vec2{ 1,1 }, Vec2{ 10,10 }, Vec3{ 0,0,0 });
	CTileMgr::GetInst()->CreateTile();

	// ============
	// FBX Loading
	// ============	
	CGameObject* pAttroxObj = nullptr;
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\indicator_hexa.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"HexaTile");
		SpawnGameObject(pObj, Vec3(1 * 300.f, 200.f, 500.f), 0);
		
		/*pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Leona.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Leona");
		SpawnGameObject(pObj, Vec3(1 * 300.f, 200.f, 500.f), 0);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Ezreal.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Ezreal");
		SpawnGameObject(pObj, Vec3(2 * 300.f, 200.f, 500.f), 0);

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Ahri2.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Ahri");
		SpawnGameObject(pObj, Vec3(3 * 300.f, 200.f, 500.f), 0);*/

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Attrox.fbx");
		pObj = nullptr;
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Attrox");
		SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
		pAttroxObj = pObj;

		CGameObject* pNewObj = new CGameObject;
		pNewObj->SetName(L"Cube");
		pNewObj->AddComponent(new CTransform);
		pNewObj->AddComponent(new CMeshRender);
		pNewObj->AddComponent(new CCollider2D);
		pNewObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pNewObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
		//pNewObj->MeshRender()->GetMaterial(0)->SetTexParam()

		pNewObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
		SpawnGameObject(pNewObj, Vec3(0.f, 0.f, 0.f), 0);
	}

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

	// ============
	// Ani Controller Init
	// ============	
	Ptr<CAnimatorController> pAniController = nullptr;
	wstring strPath = L"controller\\Attrox.controller";
	Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Dance_Loop.anm");
	pClip->SetLoop(true);
	pAniController = CResMgr::GetInst()->FindRes<CAnimatorController>(strPath);

	//test code 1
	//pAniController = new CAnimatorController();
	//CResMgr::GetInst()->AddRes<CAnimatorController>(strPath, pAniController);


	//pAniController->RegisterParam(L"1to2", 0);
	//pAniController->RegisterParam(L"2to3", 0);
	//pAniController->Init();
	//pAniController->SetName(strPath);
	//CAniNode* pOutNode = pAniController->GetNode(L"Entry");
	//CAniNode* pInNode = pAniController->CreateNode(L"Ani1", L"anim3D\\Dance_Windup.anm");
	//pAniController->CreateTransition(L"Entry_Ani1", pInNode, pOutNode,false);
	//
	//pOutNode = pInNode;
	//pInNode = pAniController->CreateNode(L"Ani2", L"anim3D\\Dance_Loop.anm");
	//CTransition* t1 = pAniController->CreateTransition(L"Ani1_Ani2", pInNode, pOutNode, false);
	//t1->RegisterCondition(L"1to2", 1, COMPARISON_TYPE::EQUAL);

	//pOutNode = pInNode;
	//pInNode = pAniController->CreateNode(L"Ani3", L"anim3D\\Aatrox_Idle1.anm");
	//CTransition* t2 = pAniController->CreateTransition(L"Ani2_Ani3", pInNode, pOutNode, false);
	//t2->RegisterCondition(L"2to3", 2, COMPARISON_TYPE::EQUAL);

	//pAniController->Save(strPath);

	//test code 2
	if (pAniController == nullptr)
	{
		Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Aatrox_Idle1.anm");
		pClip->SetLoop(true);
		pClip->Save(L"anim3D\\Aatrox_Idle1.anm");
		pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Aatrox_ULT_Idle.anm");
		pClip->SetLoop(true);
		pClip->Save(L"anim3D\\Aatrox_ULT_Idle.anm");
		pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Idle1.anm");
		pClip->SetLoop(true);

		//pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Dance_Windup.anm");
		//pClip->SetLoop(true);
		pClip->Save(L"anim3D\\Idle1.anm");

		pAniController = new CAnimatorController();
		CResMgr::GetInst()->AddRes<CAnimatorController>(strPath, pAniController);
		pAniController->RegisterParam(L"Battle", 0);
		pAniController->RegisterParam(L"ULT", 0);
		pAniController->RegisterParam(L"Move", 0);
		pAniController->RegisterParam(L"Attack",false,true);
		pAniController->RegisterParam(L"Dance", false, true);
		pAniController->RegisterParam(L"End", false, true);

		pAniController->Init();
		pAniController->SetName(strPath);
		CAniNode* pAnyNode = pAniController->GetNode(L"AnyState");
		CAniNode* pExitNode = pAniController->GetNode(L"Exit");
		CAniNode* pOutNode = pAniController->GetNode(L"Entry");
		//Idle_in_sheath
		CAniNode* pInNode = pAniController->CreateNode(L"Intro1", L"anim3D\\Aatrox_ReSheath_fullbody.anm"); 
		CTransition* t1 = pAniController->CreateTransition(L"Entry_Intro1", pInNode, pOutNode, true);
		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Intro2", L"anim3D\\Idle_in_sheath.anm");
		t1 = pAniController->CreateTransition(L"Intro1_Intro2", pInNode, pOutNode, true);
		//t1->RegisterCondition(L"Battle", 0, COMPARISON_TYPE::EQUAL);

		CAniNode* NIdleNode = pInNode;
		pOutNode = NIdleNode;
		pInNode = pAniController->CreateNode(L"Normal_Idle", L"anim3D\\Aatrox_Idle1.anm");
		t1= pAniController->CreateTransition(L"Intro2_Normal_Idle", pInNode, pOutNode, true);
		
		//Battle Idle
		CAniNode* BIdleIntroNode = pAniController->CreateNode(L"Battle_Idle_Intro", L"anim3D\\Aatrox_unsheath.anm");
		pOutNode = pInNode;
		t1 = pAniController->CreateTransition(L"Normal_Battle_Intro", BIdleIntroNode, pOutNode, false);
		t1->RegisterCondition(L"Battle", 1, COMPARISON_TYPE::EQUAL);

		pInNode = pAniController->CreateNode(L"Battle_Idle", L"anim3D\\Idle1.anm");
		t1 = pAniController->CreateTransition(L"Normal_Battle_Idle", pInNode, BIdleIntroNode, true);

		t1 = pAniController->CreateTransition(L"Battle_Idle_Exit", pExitNode, pInNode, false);
		t1->RegisterCondition(L"Battle", 0, COMPARISON_TYPE::EQUAL);

		CAniNode* BIdleNode = pInNode;

		pOutNode = BIdleNode;
		pInNode = pAniController->CreateNode(L"Battle_Move", L"anim3D\\Unsheath_run01.anm");
		t1 = pAniController->CreateTransition(L"Battle_Idle_Move", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Move", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Move_Idle", pOutNode, pInNode, false);
		t1->RegisterCondition(L"Move", 0, COMPARISON_TYPE::EQUAL);

		CAniNode* BMoveNode = pInNode;

		CAniNode* UIdleNode;
		pOutNode = BIdleNode;
		pInNode = pAniController->CreateNode(L"ULT_Idle", L"anim3D\\Aatrox_ULT_Idle.anm");
		UIdleNode = pInNode;

		CAniNode* UInMoveNode;
		pInNode = pAniController->CreateNode(L"ULT_InMove", L"anim3D\\Aatrox_ULT_Spell_Dash.anm");

		CAniNode* UInNode;
		pOutNode = UIdleNode;
		UInMoveNode = pInNode;
		pInNode = pAniController->CreateNode(L"ULT_In", L"anim3D\\ULT_Idlein.anm");
		UInNode = pInNode;

		t1 = pAniController->CreateTransition(L"ULT_In_Idle", pOutNode, pInNode, true);
		t1 = pAniController->CreateTransition(L"Battle_Ult_In", pInNode, BIdleNode, false);
		t1->RegisterCondition(L"ULT", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Ult_Idle_In_Move", UInMoveNode, pOutNode, false);
		t1->RegisterCondition(L"Move", 1, COMPARISON_TYPE::EQUAL);
		UInNode = pInNode;

		CAniNode* UOutNode;
		pOutNode = UIdleNode;
		pInNode = pAniController->CreateNode(L"ULT_Out", L"anim3D\\ULT_out.anm");
		t1 = pAniController->CreateTransition(L"ULT_Idle_Out", pInNode, pOutNode, false);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"ULT_Out_Battle_Idle", BIdleNode, pInNode, true);
		UOutNode = pInNode;
	
		CAniNode* UMoveNode;
		pOutNode = UIdleNode;
		pInNode = pAniController->CreateNode(L"ULT_Move", L"anim3D\\Run_Ult.anm");
		
		t1 = pAniController->CreateTransition(L"ULT_In_Move", pInNode, UInMoveNode, true);
		t1 = pAniController->CreateTransition(L"ULT_Move_Idle_In", UInNode, pInNode, false);
		t1->RegisterCondition(L"Move", 0, COMPARISON_TYPE::EQUAL);

		UMoveNode = pInNode;

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Dance_In", L"anim3D\\Dance_Windup.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Dance_In", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Dance", true, true, COMPARISON_TYPE::EQUAL);
		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Dance_Loop", L"anim3D\\Dance_Loop.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Dance_Loop", pInNode, pOutNode, true);
		pOutNode = pInNode;
		t1 = pAniController->CreateTransition(L"AnyState_Dance_Exit", pExitNode, pOutNode, false);
		t1->RegisterCondition(L"End", true,true, COMPARISON_TYPE::EQUAL);


		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack", L"anim3D\\Attack1.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attack", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true,true, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Battle", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Attack_Idle", BIdleNode, pInNode, true);
		//t1->RegisterCondition(L"End", true, COMPARISON_TYPE::EQUAL);
		CAniNode* BAttackNode = pInNode;

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"ULT_Attack", L"anim3D\\Attack1_Ult.anm");
		t1 = pAniController->CreateTransition(L"AnyState_ULT_Attack", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, true, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 1, COMPARISON_TYPE::EQUAL);
		CAniNode* UAttackNode = pInNode;

		t1 = pAniController->CreateTransition(L"ULT_Attack_Idle", UIdleNode, pInNode, true);
		//t1->RegisterCondition(L"End", true, COMPARISON_TYPE::EQUAL);

		pAniController->Save(strPath);
	}
	CAttroxMachineScript* fsmScript = (CAttroxMachineScript*)CScriptMgr::GetScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT);
	CBaseCharacterScript* chScript = (CBaseCharacterScript*)CScriptMgr::GetScript(SCRIPT_TYPE::BASECHARACTERSCRIPT);
	pAttroxObj->Animator3D()->SetController(pAniController);
	pAttroxObj->AddComponent(fsmScript);
	pAttroxObj->AddComponent(chScript);
	/*CCharacterTrigger trigger;
	trigger.SetEvtType(TRIGGER_TYPE::);
	script->notify(&trigger);*/

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}
