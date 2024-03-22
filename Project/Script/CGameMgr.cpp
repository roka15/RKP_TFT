#include "pch.h"
#include "CGameMgr.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\Transition.h>
#include "CAttroxMachineScript.h"
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

bool CGameMgr::BuyItem(int _iGameID, CHARACTER_TYPE _eType, CGameObject* _pPlayer)
{
	return m_vecGames[_iGameID]->BuyItem(_eType,_pPlayer);
}

void CGameMgr::CreateCharacterPrefabs()
{
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;

#pragma region Attrox
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Attrox.fbx");
	pObj = nullptr;
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Attrox");
	pObj->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
	pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(90), 0.f, DEGREE2RADIAN(180.f)));
	pObj->AddComponent(new CBoxCollider);

	Ptr<CAnimatorController> pAniController = nullptr;
	wstring strPath = L"controller\\Attrox.controller";
	Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(L"anim3D\\Dance_Loop.anm");
	pClip->SetLoop(true);
	pAniController = CResMgr::GetInst()->FindRes<CAnimatorController>(strPath);
#pragma region Attrox_Controller
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
		pAniController->RegisterParam(L"Attack", false, true);
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
		t1 = pAniController->CreateTransition(L"Intro2_Normal_Idle", pInNode, pOutNode, true);

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
		t1->RegisterCondition(L"End", true, true, COMPARISON_TYPE::EQUAL);


		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Battle_Attack", L"anim3D\\Attack1.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Battle_Attack", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Attack", true, true, COMPARISON_TYPE::EQUAL);
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
#pragma endregion
	CAttroxMachineScript* fsmScript = (CAttroxMachineScript*)CScriptMgr::GetScript(SCRIPT_TYPE::ATTROXMACHINESCRIPT);
	CBaseCharacterScript* chScript = (CBaseCharacterScript*)CScriptMgr::GetScript(SCRIPT_TYPE::BASECHARACTERSCRIPT);
	pObj->Animator3D()->SetController(pAniController);
	pObj->AddComponent(fsmScript);
	pObj->AddComponent(chScript);

	CPrefab* CH_Attrox = new CPrefab();
	CH_Attrox->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Ch_Attrox", CH_Attrox);
#pragma endregion
}

