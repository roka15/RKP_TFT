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
void CGameMgr::LoadGameInfo()
{
	m_SharedGameInfo.mapLevelMaxExp.insert(std::make_pair(1, 1));
	m_SharedGameInfo.mapLevelMaxExp.insert(std::make_pair(2, 3));
	m_SharedGameInfo.mapLevelMaxExp.insert(std::make_pair(3, 6));

	m_SharedGameInfo.ExpUpCost = 4;
	m_SharedGameInfo.RefreshCost = 2;

	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::ATTROX, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::Ahri, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::Ashe, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::Azir, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::Azirsoldier, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::BlitzCrank, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::Gwen, t_CharacterInfo{}));
	m_SharedGameInfo.mapChInfo.insert(std::make_pair(CHARACTER_TYPE::ZED, t_CharacterInfo{}));

	m_SharedGameInfo.mapCardInfo.insert(std::make_pair(ITEM_CARD_TYPE::LEVEL1, t_ItemCardInfo{}));
	m_SharedGameInfo.mapCardInfo.insert(std::make_pair(ITEM_CARD_TYPE::LEVEL2, t_ItemCardInfo{}));
	m_SharedGameInfo.mapCardInfo.insert(std::make_pair(ITEM_CARD_TYPE::LEVEL3, t_ItemCardInfo{}));
	m_SharedGameInfo.mapCardInfo.insert(std::make_pair(ITEM_CARD_TYPE::LEVEL4, t_ItemCardInfo{}));
	m_SharedGameInfo.mapCardInfo.insert(std::make_pair(ITEM_CARD_TYPE::LEVEL5, t_ItemCardInfo{}));


	CHARACTER_TYPE eType = CHARACTER_TYPE::ATTROX;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL4;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_aatrox.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 4;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"¾ÆÆ®·Ï½º";


	eType = CHARACTER_TYPE::Ahri;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL4;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_ahri.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 4;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"¾Æ¸®";

	eType = CHARACTER_TYPE::Ashe;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL2;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_ashe.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 2;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"¾Ö½¬";


	eType = CHARACTER_TYPE::Azir;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL3;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_azir.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 3;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"¾ÆÁö¸£";


	eType = CHARACTER_TYPE::Azirsoldier;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL3;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_azirsoldier.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 3;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"°¡·»";


	eType = CHARACTER_TYPE::Azirsoldier;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL3;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_azirsoldier.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 3;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"°¡·»";


	eType = CHARACTER_TYPE::BlitzCrank;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL1;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_blitzcrank.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 1;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"ºí¸®Ã÷Å©·©Å©";


	eType = CHARACTER_TYPE::Gwen;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL5;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_gwen.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 5;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"±×À¢";


	eType = CHARACTER_TYPE::ZED;
	m_SharedGameInfo.mapChInfo[eType].eCardType = ITEM_CARD_TYPE::LEVEL1;
	m_SharedGameInfo.mapChInfo[eType].eChType = eType;
	m_SharedGameInfo.mapChInfo[eType].strBaseImage = L"texture\\UI\\CharacterBanner\\tft9_zed.png";
	m_SharedGameInfo.mapChInfo[eType].iCost = 1;
	m_SharedGameInfo.mapChInfo[eType].strNameText = L"Á¦µå";



	ITEM_CARD_TYPE eItemType = ITEM_CARD_TYPE::LEVEL1;
	m_SharedGameInfo.mapCardInfo[eItemType].strEnterImage = L"texture\\UI\\CharacterBanner\\Level1_On.png";
	m_SharedGameInfo.mapCardInfo[eItemType].strExitImage = L"texture\\UI\\CharacterBanner\\Level1_Off.png";

	eItemType = ITEM_CARD_TYPE::LEVEL2;
	m_SharedGameInfo.mapCardInfo[eItemType].strEnterImage = L"texture\\UI\\CharacterBanner\\Level2_On.png";
	m_SharedGameInfo.mapCardInfo[eItemType].strExitImage = L"texture\\UI\\CharacterBanner\\Level2_Off.png";

	eItemType = ITEM_CARD_TYPE::LEVEL3;
	m_SharedGameInfo.mapCardInfo[eItemType].strEnterImage = L"texture\\UI\\CharacterBanner\\Level3_On.png";
	m_SharedGameInfo.mapCardInfo[eItemType].strExitImage = L"texture\\UI\\CharacterBanner\\Level3_Off.png";

	eItemType = ITEM_CARD_TYPE::LEVEL4;
	m_SharedGameInfo.mapCardInfo[eItemType].strEnterImage = L"texture\\UI\\CharacterBanner\\Level4_On.png";
	m_SharedGameInfo.mapCardInfo[eItemType].strExitImage = L"texture\\UI\\CharacterBanner\\Level4_Off.png";

	eItemType = ITEM_CARD_TYPE::LEVEL5;
	m_SharedGameInfo.mapCardInfo[eItemType].strEnterImage = L"texture\\UI\\CharacterBanner\\Level5_On.png";
	m_SharedGameInfo.mapCardInfo[eItemType].strExitImage = L"texture\\UI\\CharacterBanner\\Level5_Off.png";

}
void CGameMgr::init()
{
	LoadGameInfo();
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

const ITEM_CARD_TYPE CGameMgr::GetItemCardLevel(CHARACTER_TYPE _eType)
{
	return m_SharedGameInfo.mapChInfo[_eType].eCardType; 
}

const t_CharacterInfo& CGameMgr::GetChInfo(CHARACTER_TYPE _eType)
{
	return m_SharedGameInfo.mapChInfo[_eType];
}
const t_ItemCardInfo& CGameMgr::GetCardInfo(ITEM_CARD_TYPE _eType)
{
	return m_SharedGameInfo.mapCardInfo[_eType];
}
bool CGameMgr::BuyItem(int _iGameID, CHARACTER_TYPE _eType, CGameObject* _pPlayer)
{
	return m_vecGames[_iGameID]->BuyItem(_eType, _pPlayer);
}

bool CGameMgr::BuyExp(CGameObject* _pObj)
{
	CPlayerScript* pPlayer = _pObj->GetScript<CPlayerScript>();
	const int& iMoney = pPlayer->GetMoney();
	if (iMoney < m_SharedGameInfo.ExpUpCost)
		return false;
	pPlayer->PayMoney(m_SharedGameInfo.ExpUpCost);
	const int& iLevel = pPlayer->GetLevel();
	auto itr = m_SharedGameInfo.mapLevelMaxExp.find(iLevel);
	if (itr == m_SharedGameInfo.mapLevelMaxExp.end())
		return false;
	int iMaxExp = m_SharedGameInfo.mapLevelMaxExp[iLevel];

	pPlayer->ExpUp();
	const int& iExp = pPlayer->GetExp();
	if (iExp >= iMaxExp)
	{
		pPlayer->ExpZero();
		pPlayer->LevelUp();
	}
	return true;
}

bool CGameMgr::RefreshShop(int _iGameID, CGameObject* _pPlayer)
{
	m_vecGames[_iGameID]->RefreshShopBtn(_pPlayer);
	return false;
}

void CGameMgr::DeathMinion(int _iGameID, CGameObject* _pMinion)
{
	m_vecGames[_iGameID]->DeathMinion(_pMinion);
}

void CGameMgr::CreateCharacterPrefabs()
{
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;

#pragma region HPBar Prefab
	CGameObject* HPBarBackGround = new CGameObject();
	HPBarBackGround->AddComponent(new CTransform());
	HPBarBackGround->AddComponent(new CBillboard());
	HPBarBackGround->AddComponent(new CImage());

	HPBarBackGround->SetName(L"HPBarBG");
	HPBarBackGround->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_ClientHPBackGround.png");
	HPBarBackGround->Transform()->SetRelativeScale(380.f, 50.f, 0.f);
	HPBarBackGround->Transform()->SetRelativePos(Vec3(0.f, 250.f, 0.f));
	CGameObject* HPBar = new CGameObject();
	HPBar->SetName(L"HPBar");
	HPBar->AddComponent(new CTransform());
	HPBar->AddComponent(new CBillboard());
	HPBar->AddComponent(new CImage());
	HPBar->Billboard()->SetGaugeOption(GAUGE_TYPE::RIGHT);
	HPBar->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_ClientHPBar.png");
	HPBar->Image()->SetEnableTexKey(L"texture\\UI\\Hud\\UI_EnemyHPbar.png");
	HPBar->Transform()->SetRelativeScale(270.f, 17.f, 0.f);
	HPBar->Transform()->SetRelativePos(Vec3(-0.065f, 0.f, 0.f));
	HPBarBackGround->AddChild(HPBar);
	Ptr<CPrefab> pHPBarBG = new CPrefab();
	pHPBarBG->RegisterProtoObject(HPBarBackGround);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Billboard_HPBar", pHPBarBG);
#pragma endregion
#pragma region MPBar Prefab
	CGameObject* MPBarBackGround = new CGameObject();
	MPBarBackGround->AddComponent(new CTransform());
	MPBarBackGround->AddComponent(new CBillboard());
	MPBarBackGround->AddComponent(new CImage());

	MPBarBackGround->SetName(L"MPBarBG");
	MPBarBackGround->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_MPBackGround.png");
	MPBarBackGround->Transform()->SetRelativeScale(256.f, 20.f, 0.f);
	MPBarBackGround->Transform()->SetRelativePos(Vec3(-24.f, 250.f, 18.f));
	CGameObject* MPBar = new CGameObject();
	MPBar->SetName(L"MPBar");
	MPBar->AddComponent(new CTransform());
	MPBar->AddComponent(new CBillboard());
	MPBar->AddComponent(new CImage());
	MPBar->Billboard()->SetGaugeOption(GAUGE_TYPE::RIGHT);
	MPBar->Image()->SetNormalTexKey(L"texture\\UI\\Hud\\UI_MPbar.png");
	MPBar->Transform()->SetRelativeScale(245.f, 17.f, 0.f);
	MPBar->Transform()->SetRelativePos(Vec3(0.0f, 0.f, 0.f));
	MPBarBackGround->AddChild(MPBar);
	Ptr<CPrefab> pMPBarBG = new CPrefab();
	pMPBarBG->RegisterProtoObject(MPBarBackGround);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Billboard_MPBar", pMPBarBG);
#pragma endregion

#pragma region Zed Prefab
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Zed.fbx");
	pObj = nullptr;
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Zed");
	pObj->Transform()->SetRelativeScale(Vec3{ 1.5f,1.5f,1.5f });
	pObj->Transform()->SetRelativeRot(Vec3(DEGREE2RADIAN(90.f), 0.f, DEGREE2RADIAN(180.f)));

	CZedMachineScript* ZedFsmScript = (CZedMachineScript*)CScriptMgr::GetScript(SCRIPT_TYPE::ZEDMACHINESCRIPT);
	CBaseCharacterScript* chScript = (CBaseCharacterScript*)CScriptMgr::GetScript(SCRIPT_TYPE::CHMINIONSCRIPT);
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
	pAnimator->RegisterAnimation(L"anim3D\\Zed\\Death_Skeleton.anm");
#pragma endregion

	Ptr<CAnimatorController> pAniController = nullptr;
	wstring strPath = L"controller\\Zed.controller";
	Ptr<CAniClip> pClip;
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
		pAniController->RegisterParam(L"Death", false, false);

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

		pOutNode = pAnyNode;
		pInNode = pAniController->CreateNode(L"Death", L"anim3D\\Zed\\Death_Skeleton.anm");
		t1 = pAniController->CreateTransition(L"AnyState_Death", pInNode, pOutNode, false);
		t1->RegisterCondition(L"Death", true, false, COMPARISON_TYPE::EQUAL);
		t1 = pAniController->CreateTransition(L"Death_Exit", IdleNode, pInNode, false);
		t1->RegisterCondition(L"Death", false, false, COMPARISON_TYPE::EQUAL);

	}
	pObj->Animator3D()->SetController(pAniController);
#pragma endregion

	CGameObject* pZedHPBG = pHPBarBG->Instantiate();
	CGameObject* pZedMPBG = pMPBarBG->Instantiate();
	pObj->AddChild(pZedHPBG);
	pObj->AddChild(pZedMPBG);

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
		t1->SetBlendTime(0.1f);
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
		t1->SetBlendTime(0.f);

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
		t1->RegisterCondition(L"Attack", false, false, COMPARISON_TYPE::EQUAL);

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

	CGameObject* pAttroxHPBG = pHPBarBG->Instantiate();
	CGameObject* pAttroxMPBG = pMPBarBG->Instantiate();
	pObj->AddChild(pAttroxHPBG);
	pObj->AddChild(pAttroxMPBG);

	CPrefab* CH_Attrox = new CPrefab();
	CH_Attrox->RegisterProtoObject(pObj);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Ch_Attrox", CH_Attrox);
#pragma endregion



}

void CGameMgr::RegisterUImap(int _iGameID, wstring _Key, CGameObject* _pObj)
{
	m_vecGames[_iGameID]->RegisterGameUI(_Key, _pObj);
}

bool CGameMgr::IsSamePlayer(CGameObject* _pObj)
{
	return m_ClientOwner == _pObj;
}


