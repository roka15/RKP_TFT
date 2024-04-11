#include "pch.h"
#include "CGame.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CResMgr.h>

#include "CBaseCharacterScript.h"
#include "CChMinionScript.h"
#include "CPlayerScript.h"
#include "CTileMgr.h"
int CGame::CanEnter()
{
	for (int i = 0; i < m_vecUsers.size(); ++i)
	{
		if (m_vecUsers[i] == nullptr)
			return i;
	}
	return -1;
}

void CGame::init()
{
	if (m_pAIPlayer == nullptr)
	{
		m_pAIPlayer = new CGameObject();
		m_pAIPlayer->SetName(L"AIPlayer");
		m_pAIPlayer->AddComponent(new CTransform());
		m_pAIPlayer->AddComponent(new CPlayerScript());
		CPlayerScript* pPlayerScript = m_pAIPlayer->GetScript<CPlayerScript>();
		pPlayerScript->SetPlayerType(PLAYER_TYPE::AI);
		pPlayerScript->SetGameID(m_GameID);
		SpawnGameObject(m_pAIPlayer, Vec3(0.f, 0.f, 0.f), 0);
	}
	m_vecUsers.resize(m_iUserCnt);
	Ptr<CPrefab> prefab;
	CHARACTER_TYPE eType;
	CreateMinion();

	const int AttroxCnt = 3;
	m_mapShop.insert(std::make_pair(CHARACTER_TYPE::ATTROX, queue<CGameObject*>()));
	eType = CHARACTER_TYPE::ATTROX;
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Ch_Attrox");

	for (int i = 0; i < AttroxCnt; ++i)
	{
		//game 객체를 재활용 하는 경우 이미 멤버변수들은 생성되어 있기 때문에  초기화를 진행한다.
		if (m_mapShop[eType].size() == AttroxCnt)
		{

		}
		else
		{
			CGameObject* obj = prefab->Instantiate();
			m_mapShop[eType].push(obj);
		}
	}

	const int ZedCnt = 1;
	m_mapShop.insert(std::make_pair(CHARACTER_TYPE::ZED, queue<CGameObject*>()));
	eType = CHARACTER_TYPE::ZED;
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Ch_Zed");
	for (int i = 0; i < ZedCnt; ++i)
	{//game 객체를 재활용 하는 경우 이미 멤버변수들은 생성되어 있기 때문에  초기화를 진행한다.
		if (m_mapShop[eType].size() == ZedCnt)
		{

		}
		else
		{
			CGameObject* obj = prefab->Instantiate();
			m_mapShop[eType].push(obj);
		}
	}
	m_bFirstLoading = false;
}

void CGame::tick()
{
	m_fCurTime += DT;

	bool ItemRound = false;
	float CompareTime;
	bool  bBattleTile = false;
	GAME_STATE GameMode = GAME_STATE::END;
	switch (m_eGameState)
	{
	case GAME_STATE::SELECT:
		CompareTime = m_fSelectTime;
		GameMode = GAME_STATE::BATTLE;
		bBattleTile = false;
		break;
	case GAME_STATE::BATTLE:
		CompareTime = m_fBettleTime;
		GameMode = GAME_STATE::LOADING;
		bBattleTile = false;
		break;
	case GAME_STATE::ITEM:
		CompareTime = m_fItemTime;
		GameMode = GAME_STATE::LOADING;
		bBattleTile = false;
		break;
	case GAME_STATE::LOADING:
		CompareTime = m_fLoadingTime;
		GameMode = GAME_STATE::SELECT;
		//첫 로딩때만 여기서 Spawn 한다. 이후에는 아래의 Spawn 함수에서 수행.
		if (m_bFirstLoading == false)
		{
			SpawnMinion(m_iRoundCnt);
			m_bFirstLoading = true;
		}
		bBattleTile = true;
		break;
	}

	if (m_fCurTime - m_fStartTime > CompareTime)
	{
		switch (GameMode)
		{
		case GAME_STATE::SELECT:
			break;
		case GAME_STATE::BATTLE:

			break;
		case GAME_STATE::LOADING:
			++m_iRoundCnt;
			DespawnMinion();
			SpawnMinion(m_iRoundCnt);
			break;
		}

		m_fStartTime = m_fCurTime;

		/*if (m_iRoundCnt % 4 == 0)
		{
			GameMode = GAME_STATE::ITEM;
			++m_iRoundCnt;
		}*/
		m_eGameState = GameMode;

		if (bBattleTile)
		{
			CTileMgr::GetInst()->EnableSelectBattleTile(true);
		}
		else
		{
			CTileMgr::GetInst()->EnableSelectBattleTile(false);
		}
		SendGameState((UINT)m_eGameState);
	}
	UpdateRoundUI(CompareTime);
}

void CGame::RegisterUser(int _iGameID, int _iIdx, CGameObject* _pObj)
{
	CPlayerScript* player = _pObj->GetScript<CPlayerScript>();
	if (player == nullptr)
		assert(nullptr);

	m_vecUsers[_iIdx] = player;
	player->SetGameID(_iGameID);
}

void CGame::RegisterGameUI(wstring _strKey, CGameObject* _pObj)
{
	auto itr = m_mapUIObjs.find(_strKey);
	if (itr != m_mapUIObjs.end())
	{
		return;
	}
	m_mapUIObjs.insert(std::make_pair(_strKey, _pObj));
}

bool CGame::BuyItem(CHARACTER_TYPE _eType, CGameObject* _pPlayer)
{
	bool bEmpty = m_mapShop[_eType].empty();
	if (bEmpty)
		return false;

	CGameObject* pItem = m_mapShop[_eType].front();
	CPlayerScript* pPlayerScript = _pPlayer->GetScript<CPlayerScript>();
	CItem* pItemScript = pItem->GetScript<CItem>();
	//플레이어 돈에서 cost 빼기. 자금이 충분하지 않다면 return false
	int iChange = Buy(pItemScript, pPlayerScript);
	int tileNumber = CTileMgr::GetInst()->FindEmptyWaitTile();
	if (iChange >= 0 && tileNumber >= 0)
	{
		m_mapShop[_eType].pop();
		//player script와 item script는 서로 포인터를 참조용으로만 들고 있는다.
		//객체에 대한 생존 주기 등은 Tile에 속하게 된다.
		//만약 Item을 판매한다면 이 객체는 다시 Game 의 m_mapShop에서 관리.
		pPlayerScript->AddItem(pItem);
		pItemScript->SetPlayer(_pPlayer);
		CTileMgr::GetInst()->RegisterItem(tileNumber, pItem);
		return true;
	}//구매 실패
	else
	{
		return false;
	}
}

void CGame::DeathMinion(CGameObject* _pObj)
{
	int iCnt = m_SpawnMinion.size();
	for (int i = 0; i < iCnt; ++i)
	{
		CGameObject* pMinion = m_SpawnMinion.front();
		m_SpawnMinion.pop();
		if (pMinion == _pObj)
		{
			DespawnMinion(_pObj);
		}
		else
		{
			m_SpawnMinion.push(pMinion);
		}
	}
}

void CGame::SendGameState(UINT _iState)
{
	for (int i = 0; i < m_vecUsers.size(); ++i)
	{
		CPlayerScript* player = m_vecUsers[i];
		if (player == nullptr)
			continue;

		player->SetGameState(_iState);
		player->SetGameStateInfo();
	}

	CPlayerScript* pPlayer = m_pAIPlayer->GetScript<CPlayerScript>();
	pPlayer->SetGameState(_iState);
	pPlayer->SetGameStateInfo();
}

int CGame::Buy(CItem* _pItem, CPlayerScript* _pPlayer)
{
	const int& Money = _pPlayer->GetMoney();
	return _pItem->Buy(Money);
}

void CGame::CreateMinion()
{
	CGameObject* pMinionContainer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MinionContainer");
	//game 객체를 재활용 하는 경우 이미 필요한 객체들은 생성되어 있기 때문에  초기화를 진행한다.
	if (pMinionContainer)
	{
		DespawnMinion();
	}
	else
	{
		pMinionContainer = new CGameObject();
		pMinionContainer->AddComponent(new CTransform());


		pMinionContainer->SetName(L"MinionContainer");
		SpawnGameObject(pMinionContainer, Vec3{ 0.f,-1000.f,0.f }, 0);

		Ptr<CPrefab> prefab;
		CHARACTER_TYPE eType;
		const int MinionCnt = 5;
		prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Ch_Zed");
		for (int i = 0; i < MinionCnt; ++i)
		{
			CGameObject* obj = prefab->Instantiate();
			CChMinionScript* pChScript = obj->GetScript<CChMinionScript>();
			pChScript->SetPlayer(m_pAIPlayer);
			CPlayerScript* pPlayer = m_pAIPlayer->GetScript<CPlayerScript>();
			pPlayer->AddItem(obj);
			m_DespawnMinion.push(obj);
			pMinionContainer->AddChild(obj);
		}

		const int iMinionRoundCnt = 3;
		const int arrMinionCnt[iMinionRoundCnt] = { 2,3,4 };
		vector<vector<int>> vecTileNums;
		vecTileNums.resize(iMinionRoundCnt);
		vecTileNums[0].push_back(60);
		vecTileNums[0].push_back(62);
		vecTileNums[1].push_back(60);
		vecTileNums[1].push_back(62);
		vecTileNums[1].push_back(47);
		vecTileNums[2].push_back(60);
		vecTileNums[2].push_back(62);
		vecTileNums[2].push_back(53);
		vecTileNums[2].push_back(54);

		for (int i = 0; i < iMinionRoundCnt; ++i)
		{
			t_RoundInfo tRound = {};
			tRound.iMinionCnt = arrMinionCnt[i];
			for (int j = 0; j < tRound.iMinionCnt; ++j)
				tRound.vecMinionTileNum.push_back(vecTileNums[i][j]);

			//1~3 라운드에 minion 배치 정보 저장.
			m_RoundInfo.insert(std::make_pair(i, tRound));
		}
	}
}

bool CGame::SpawnMinion(int _iRound)
{
	auto itr = m_RoundInfo.find(_iRound);
	if (itr == m_RoundInfo.end())
		return false;

	const int& iCnt = itr->second.iMinionCnt;
	const vector<int>& iTileNums = itr->second.vecMinionTileNum;

	for (int i = 0; i < iCnt; ++i)
	{
		CGameObject* pMinion = m_DespawnMinion.front();
		m_DespawnMinion.pop();
		m_SpawnMinion.push(pMinion);

		int iTileNum = iTileNums[i];
		CTileMgr::GetInst()->RegisterItem(iTileNum, pMinion);
	}
	return true;
}

void CGame::DespawnMinion()
{
	int iCnt = m_SpawnMinion.size();
	for (int i = 0; i < iCnt; ++i)
	{
		CGameObject* pMinion = m_SpawnMinion.front();
		m_SpawnMinion.pop();
		DespawnMinion(pMinion);
	}
}

void CGame::DespawnMinion(CGameObject* _pObj)
{
	m_DespawnMinion.push(_pObj);
	CBaseCharacterScript* pChScript = _pObj->GetScript<CBaseCharacterScript>();
	pChScript->start();
	CGameObject* pMinionContainer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MinionContainer");
	pMinionContainer->AddChild(_pObj);
}

void CGame::UpdateRoundUI(float _fCompare)
{
	CGameObject* PTimeObj = m_mapUIObjs[L"TIMER_TEXT"];
	CText* pTimeText = (CText*)PTimeObj->GetComponent(COMPONENT_TYPE::TEXT);
	CGameObject* pRoundObj = m_mapUIObjs[L"ROUND_TEXT"];
	CText* pRoundText = (CText*)pRoundObj->GetComponent(COMPONENT_TYPE::TEXT);
	float fTime = _fCompare - (m_fCurTime - m_fStartTime);
	int iTime = (int)fTime + 1;
	int firstNum = (m_iRoundCnt / m_iRoundMax) + 1;
	int secondNum = (m_iRoundCnt % m_iRoundMax) + 1;
	pTimeText->SetText(std::to_wstring(iTime));
	pRoundText->SetText(std::to_wstring(firstNum) + L" - " + std::to_wstring(secondNum));

	CGameObject* pTimeGaugeObj = m_mapUIObjs[L"TIMER_IMAGE"];
	pTimeGaugeObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fTime);
	pTimeGaugeObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &_fCompare);

	int beforRoundNum = secondNum == 1 ? 3 : secondNum - 1;
	CGameObject* pRoundCursor = m_mapUIObjs[L"CUR_ROUND_IMAGE"];
	wstring RoundName = L"ROUND_IMAGE";
	wstring CurRoundName = RoundName + to_wstring(secondNum);
	wstring BeforRoundName = RoundName + to_wstring(beforRoundNum);
	CGameObject* pCurRound = m_mapUIObjs[CurRoundName];
	CGameObject* pBeforRound = m_mapUIObjs[BeforRoundName];
	Vec3 v3RoundPos = pCurRound->Transform()->GetRelativePos();
	Vec3 v3CursorPos = pRoundCursor->Transform()->GetRelativePos();
	v3CursorPos.x = v3RoundPos.x;
	pRoundCursor->Transform()->SetRelativePos(v3CursorPos);
	
	pCurRound->Image()->SetActiveImage(true);
	pBeforRound->Image()->SetActiveImage(false);
}


CGame::CGame()
	:m_iUserCnt(8),
	m_eGameState(GAME_STATE::LOADING),
	m_iRoundCnt(0),
	m_fStartTime(0.f),
	m_fCurTime(0.f),
	m_fSelectTime(10.f),
	m_fBettleTime(10.f),
	m_fItemTime(30.f),
	m_fLoadingTime(10.f),
	m_bFirstLoading(false),
	m_iRoundMax(3)
{
}

CGame::~CGame()
{
	for (auto itr = m_mapShop.begin(); itr != m_mapShop.end(); ++itr)
	{
		queue<CGameObject*>  queue = itr->second;
		while (queue.empty() == false)
		{
			CGameObject* obj = queue.front();
			queue.pop();
			delete obj;
			obj = nullptr;
		}
	}
}
