#include "pch.h"
#include "CGameMgr.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\Transition.h>
#include <Engine\CAnimation3D.h>
#include "CAttroxMachineScript.h"
#include "CZedMachineScript.h"
#include "CPlayerScript.h"
#include "CGame.h"
#include "CBaseCharacterScript.h"
CGameMgr::CGameMgr()

{

}
CGameMgr::~CGameMgr()
{
	for (int i = 0; i < m_vecGames.size(); ++i)
	{
		if (m_vecGames[i] == nullptr)
			continue;
		delete m_vecGames[i];
		m_vecGames[i] = nullptr;
	}
}
void CGameMgr::init()
{
	CreateCharacterPrefabs();
	for (int i = 0; i < m_vecGames.size(); ++i)
	{
		m_vecGames[i]->init();
	}
}
void CGameMgr::tick()
{
	for (int i = 0; i < m_vecGames.size(); ++i)
	{
		m_vecGames[i]->tick();
	}
}

void CGameMgr::CreateGame()
{
	CGame* pGame = new CGame();

	int index = m_vecGames.size();
	pGame->SetGameID(index);
	m_vecGames.push_back(pGame);
}

bool CGameMgr::EnterGame(int _iID, CGameObject* _pObj)
{
	int size = m_vecGames.size();
	if (_iID >= size)
		return false;

	CGame* game = m_vecGames[_iID];
	int iEnterIndex = game->CanEnter();
	if (iEnterIndex != -1)
	{
		UINT gameID = game->GetGameID();
		game->RegisterUser(gameID, iEnterIndex, _pObj);
	}
}

int CGameMgr::GetState(CGameObject* _pObj)
{
	CPlayerScript* player = _pObj->GetScript<CPlayerScript>();
	if (player == nullptr)
		return -1;
	UINT iGameID = player->GetGameID();
	CGame* pGame = m_vecGames[iGameID];
	
	return pGame->GetGameState();;
}

int CGameMgr::GetState()
{
	CPlayerScript* player = m_ClientOwner->GetScript<CPlayerScript>();
	if (player == nullptr)
		return -1;
	UINT iGameID = player->GetGameID();
	CGame* pGame = m_vecGames[iGameID];

	return pGame->GetGameState();;
}

bool CGameMgr::BuyItem(int _iGameID, CHARACTER_TYPE _eType, CGameObject* _pPlayer)
{
	return m_vecGames[_iGameID]->BuyItem(_eType,_pPlayer);
}

void CGameMgr::CreateCharacterPrefabs()
{
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;

#pragma region Zed Prefab
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Zed.fbx");
	pObj = nullptr;
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Zed");
	pObj->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
	pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(180.f)));

	CZedMachineScript* ZedFsmScript = (CZedMachineScript*)CScriptMgr::GetScript(SCRIPT_TYPE::ZEDMACHINESCRIPT);
	CBaseCharacterScript* chScript = (CBaseCharacterScript*)CScriptMgr::GetScript(SCRIPT_TYPE::BASECHARACTERSCRIPT);
	pObj->AddComponent(ZedFsmScript);
	pObj->AddComponent(chScript);
#pragma region Animator - Animation Register
	CAnimator3D* pAnimator = pObj->Animator3D(); 
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Spawn_Skeleton.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Zed_idle1.anm_Skeleton.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Zed_idle_LeadIn1.anm_Skeleton.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Zed_run.anm_Skeleton.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Zed_attack1.anm_Skeleton.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Zed_attack2.anm_Skeleton.anm");

#pragma endregion

	Ptr<CAnimatorController> pAniController = nullptr;
	wstring strPath = L"controller\\Zed.controller";
	Ptr<CAniClip> pClip ;
	pAniController = CResMgr::GetInst()->FindRes<CAnimatorController>(strPath);
#pragma region Zed_Controller
	if (pAniController == nullptr)
	{
		pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Zed\\Zed_idle1.anm_Skeleton.anm");
		pClip->SetLoop(true);
		pClip->Save(pClip->GetRelativePath());

		pAniController = new CAnimatorController();
		CResMgr::GetInst()->AddRes<CAnimatorController>(strPath, pAniController);
		pAniController->RegisterParam(L"Battle", 0);
		pAniController->RegisterParam(L"ULT", 0);
		pAniController->RegisterParam(L"Move", 0);
		pAniController->RegisterParam(L"Attack", false, false);
		pAniController->RegisterParam(L"Dance", false, true);
		pAniController->RegisterParam(L"End", false, true);
		pAniController->RegisterParam(L"Attack_Number", 0);

		pAniController->Init();
		pAniController->SetName(strPath);
		CAniNode* pAnyNode = pAniController->GetNode(L"AnyState");
		CAniNode* pExitNode = pAniController->GetNode(L"Exit");
		CAniNode* pOutNode = pAniController->GetNode(L"Entry");
		//Idle_in_sheath
		CAniNode* pInNode = pAniController->CreateNode(L"Intro1", L"anim3D\\Zed\\Spawn_Skeleton.anm");
		CTransition* t1 = pAniController->CreateTransition(L"Entry_Intro1", pInNode, pOutNode, true);
		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Intro2", L"anim3D\\Zed\\Zed_idle_LeadIn1.anm_Skeleton.anm");
		t1 = pAniController->CreateTransition(L"Intro1_Intro2", pInNode, pOutNode, true);
		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Normal_Idle", L"anim3D\\Zed\\Zed_idle1.anm_Skeleton.anm");
		t1 = pAniController->CreateTransition(L"Intro2_Normal_Idle", pInNode, pOutNode, true);
		CAniNode* IdleNode = pInNode;

		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Battle_Move", L"anim3D\\Zed\\Zed_run.anm_Skeleton.anm");
		t1 = pAniController->CreateTransition(L"Battle_Idle_Move", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Move", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Move_Idle", pOutNode, pInNode, false);
		t1->RegisterCondition(L"Move", 0, COMPARISON_TYPE::EQUAL);

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack1", L"anim3D\\Zed\\Zed_attack1.anm_Skeleton.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attack", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 0, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Attack_Idle", IdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack2", L"anim3D\\Zed\\Zed_attack2.anm_Skeleton.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attack2", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 1, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Attack_Idle", IdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);
	}
	pObj->Animator3D()->SetController(pAniController);
#pragma endregion

	CPrefab* CH_Zed = new CPrefab();
	CH_Zed->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Ch_Zed", CH_Zed);
#pragma endregion	
#pragma region Attrox
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Attrox.fbx");
	pObj = nullptr;
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Attrox");
	pObj->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
	pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(180.f)));
	pObj->AddComponent(new CBoxCollider);
	CAttroxMachineScript* AttroxFsmScript = (CAttroxMachineScript*)CScriptMgr::GetScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT);
	AttroxFsmScript = (CAttroxMachineScript*)CScriptMgr::GetScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT);
	chScript = (CBaseCharacterScript*)CScriptMgr::GetScript(SCRIPT_TYPE::BASECHARACTERSCRIPT);
	pObj->AddComponent(AttroxFsmScript);
	pObj->AddComponent(chScript);

#pragma region Animator - Animation Register
	pAnimator = pObj->Animator3D();
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Aatrox_ReSheath_fullbody.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Idle_in_sheath.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Aatrox_Idle1.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Aatrox_unsheath.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Idle1.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Unsheath_run01.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Aatrox_ULT_Idle.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Aatrox_ULT_Spell_Dash.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\ULT_Idlein.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\ULT_out.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Run_Ult.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Dance_Windup.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Dance_Loop.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Attack1.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Attack2.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Attack3.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Attack1_Ult.anm");
	pAnimator->RegisterAnimation(L"anim3D\\Attrox\\Attack2_Ult.anm");
#pragma endregion
	pAniController = nullptr;
	strPath = L"controller\\Attrox.controller";
	pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Attrox\\Dance_Loop.anm");
	pClip->SetLoop(true);
	pAniController = CResMgr::GetInst()->FindRes<CAnimatorController>(strPath);

#pragma region Attrox_Controller
	if (pAniController == nullptr)
	{
		Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Attrox\\Aatrox_Idle1.anm");
		pClip->SetLoop(true);
		pClip->Save(L"anim3D\\Attrox\\Aatrox_Idle1.anm");
		pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Attrox\\Aatrox_ULT_Idle.anm");
		pClip->SetLoop(true);
		pClip->Save(L"anim3D\\Attrox\\Aatrox_ULT_Idle.anm");
		pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Attrox\\Idle1.anm");
		pClip->SetLoop(true);
		
		//pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Dance_Windup.anm");
		//pClip->SetLoop(true);
		pClip->Save(L"anim3D\\Attrox\\Idle1.anm");

		pAniController = new CAnimatorController();
		CResMgr::GetInst()->AddRes<CAnimatorController>(strPath, pAniController);
		pAniController->RegisterParam(L"Battle", 0);
		pAniController->RegisterParam(L"ULT", 0);
		pAniController->RegisterParam(L"Move", 0);
		pAniController->RegisterParam(L"Attack", false, false);
		pAniController->RegisterParam(L"Dance", false, true);
		pAniController->RegisterParam(L"End", false, true);
		pAniController->RegisterParam(L"Attack_Number", 0);

		pAniController->Init();
		pAniController->SetName(strPath);
		CAniNode* pAnyNode = pAniController->GetNode(L"AnyState");
		CAniNode* pExitNode = pAniController->GetNode(L"Exit");
		CAniNode* pOutNode = pAniController->GetNode(L"Entry");
		//Idle_in_sheath
		CAniNode* pInNode = pAniController->CreateNode(L"Intro1", L"anim3D\\Attrox\\Aatrox_ReSheath_fullbody.anm");
		CTransition* t1 = pAniController->CreateTransition(L"Entry_Intro1", pInNode, pOutNode, true);
		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Intro2", L"anim3D\\Attrox\\Idle_in_sheath.anm");
		t1 = pAniController->CreateTransition(L"Intro1_Intro2", pInNode, pOutNode, true);
		//t1->RegisterCondition(L"Battle", 0, COMPARISON_TYPE::EQUAL);

		CAniNode* NIdleNode = pInNode;
		pOutNode = NIdleNode;
		pInNode = pAniController->CreateNode(L"Normal_Idle", L"anim3D\\Attrox\\Aatrox_Idle1.anm");
		t1 = pAniController->CreateTransition(L"Intro2_Normal_Idle", pInNode, pOutNode, true);

		//Battle Idle
		CAniNode* BIdleIntroNode = pAniController->CreateNode(L"Battle_Idle_Intro", L"anim3D\\Attrox\\Aatrox_unsheath.anm");
		pOutNode = pInNode;
		t1 = pAniController->CreateTransition(L"Normal_Battle_Intro", BIdleIntroNode, pOutNode, false);
		t1->RegisterCondition(L"Battle", 1, COMPARISON_TYPE::EQUAL);

		pInNode = pAniController->CreateNode(L"Battle_Idle", L"anim3D\\Attrox\\Idle1.anm");
		t1 = pAniController->CreateTransition(L"Normal_Battle_Idle", pInNode, BIdleIntroNode, true);

		t1 = pAniController->CreateTransition(L"Battle_Idle_Exit", pExitNode, pInNode, false);
		t1->RegisterCondition(L"Battle", 0, COMPARISON_TYPE::EQUAL);

		CAniNode* BIdleNode = pInNode;

		pOutNode = BIdleNode;
		pInNode = pAniController->CreateNode(L"Battle_Move", L"anim3D\\Attrox\\Unsheath_run01.anm");
		t1 = pAniController->CreateTransition(L"Battle_Idle_Move", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Move", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Move_Idle", pOutNode, pInNode, false);
		t1->RegisterCondition(L"Move", 0, COMPARISON_TYPE::EQUAL);

		CAniNode* BMoveNode = pInNode;

		CAniNode* UIdleNode;
		pOutNode = BIdleNode;
		pInNode = pAniController->CreateNode(L"ULT_Idle", L"anim3D\\Attrox\\Aatrox_ULT_Idle.anm");
		UIdleNode = pInNode;

		CAniNode* UInMoveNode;
		pInNode = pAniController->CreateNode(L"ULT_InMove", L"anim3D\\Attrox\\Aatrox_ULT_Spell_Dash.anm");

		CAniNode* UInNode;
		pOutNode = UIdleNode;
		UInMoveNode = pInNode;
		pInNode = pAniController->CreateNode(L"ULT_In", L"anim3D\\Attrox\\ULT_Idlein.anm");
		UInNode = pInNode;

		t1 = pAniController->CreateTransition(L"ULT_In_Idle", pOutNode, pInNode, true);
		t1 = pAniController->CreateTransition(L"Battle_Ult_In", pInNode, BIdleNode, false);
		t1->RegisterCondition(L"ULT", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Ult_Idle_In_Move", UInMoveNode, pOutNode, false);
		t1->RegisterCondition(L"Move", 1, COMPARISON_TYPE::EQUAL);
		UInNode = pInNode;

		CAniNode* UOutNode;
		pOutNode = UIdleNode;
		pInNode = pAniController->CreateNode(L"ULT_Out", L"anim3D\\Attrox\\ULT_out.anm");
		t1 = pAniController->CreateTransition(L"ULT_Idle_Out", pInNode, pOutNode, false);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"ULT_Out_Battle_Idle", BIdleNode, pInNode, true);
		UOutNode = pInNode;

		CAniNode* UMoveNode;
		pOutNode = UIdleNode;
		pInNode = pAniController->CreateNode(L"ULT_Move", L"anim3D\\Attrox\\Run_Ult.anm");

		t1 = pAniController->CreateTransition(L"ULT_In_Move", pInNode, UInMoveNode, true);
		t1 = pAniController->CreateTransition(L"ULT_Move_Idle_In", UInNode, pInNode, false);
		t1->RegisterCondition(L"Move", 0, COMPARISON_TYPE::EQUAL);

		UMoveNode = pInNode;

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Dance_In", L"anim3D\\Attrox\\Dance_Windup.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Dance_In", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Dance", true, true, COMPARISON_TYPE::EQUAL);
		pOutNode = pInNode;
		pInNode = pAniController->CreateNode(L"Dance_Loop", L"anim3D\\Attrox\\Dance_Loop.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Dance_Loop", pInNode, pOutNode, true);
		pOutNode = pInNode;
		t1 = pAniController->CreateTransition(L"AnyState_Dance_Exit", pExitNode, pOutNode, false);
		t1->RegisterCondition(L"End", true, true, COMPARISON_TYPE::EQUAL);


		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack1", L"anim3D\\Attrox\\Attack1.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attack", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 0, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Attack_Idle", BIdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false,false, COMPARISON_TYPE::EQUAL);
		
		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack2", L"anim3D\\Attrox\\Attack2.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attack2", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 1, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Attack_Idle", BIdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack3", L"anim3D\\Attrox\\Attack3.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attac3", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 2, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 0, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Battle_Attack_Idle", BIdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);


		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"ULT_Attack1", L"anim3D\\Attrox\\Attack1_Ult.anm");
		t1 = pAniController->CreateTransition(L"AnyState_ULT_Attack1", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 0, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"ULT_Attack_Idle", UIdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"ULT_Attack2", L"anim3D\\Attrox\\Attack2_Ult.anm");
		t1 = pAniController->CreateTransition(L"AnyState_ULT_Attack2", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, false, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"Attack_Number", 1, COMPARISON_TYPE::EQUAL);
		t1->RegisterCondition(L"ULT", 1, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"ULT_Attack_Idle", UIdleNode, pInNode, false);
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);


	

		
		//t1->RegisterCondition(L"End", true, COMPARISON_TYPE::EQUAL);

		pAniController->Save(strPath);
	}
#pragma endregion

	pObj->Animator3D()->SetController(pAniController);
	CPrefab* CH_Attrox = new CPrefab();
	CH_Attrox->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Ch_Attrox", CH_Attrox);
#pragma endregion
}

bool CGameMgr::IsSamePlayer(CGameObject* _pObj)
{
	return m_ClientOwner == _pObj;
}


