#include "pch.h"
#include "CGame.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CResMgr.h>

#include "CBaseCharacterScript.h"

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
		CGameObject* obj = prefab->Instantiate();
		m_mapShop[eType].push(obj);
	}

	const int ZedCnt = 1;
	m_mapShop.insert(std::make_pair(CHARACTER_TYPE::ZED, queue<CGameObject*>()));
	eType = CHARACTER_TYPE::ZED;
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Ch_Zed");
	for (int i = 0; i < ZedCnt; ++i)
	{
		CGameObject* obj = prefab->Instantiate();
		m_mapShop[eType].push(obj);
	}
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
		bBattleTile = true;
		break;
	}

	if (m_fCurTime - m_fStartTime > CompareTime)
	{
		if (GameMode == GAME_STATE::BATTLE)
			++m_iRoundCnt;
		m_fStartTime = m_fCurTime;

		if (m_iRoundCnt % 4 == 0)
		{
			GameMode = GAME_STATE::ITEM;
			++m_iRoundCnt;
		}
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
}

void CGame::RegisterUser(int _iGameID, int _iIdx, CGameObject* _pObj)
{
	CPlayerScript* player = _pObj->GetScript<CPlayerScript>();
	if (player == nullptr)
		assert(nullptr);

	m_vecUsers[_iIdx] = player;
	player->SetGameID(_iGameID);
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
}

int CGame::Buy(CItem* _pItem, CPlayerScript* _pPlayer)
{
	const int& Money = _pPlayer->GetMoney();
	return _pItem->Buy(Money);
}

void CGame::CreateMinion()
{
	Ptr<CPrefab> prefab;
	CHARACTER_TYPE eType;
	const int MinionCnt = 5;
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Ch_Zed");
	for (int i = 0; i < MinionCnt; ++i)
	{
		CGameObject* obj = prefab->Instantiate();
		m_vecMinion.push_back(obj);
	}
}

CGame::CGame()
	:m_iUserCnt(8),
	m_eGameState(GAME_STATE::SELECT),
	m_iRoundCnt(0),
	m_fStartTime(0.f),
	m_fCurTime(0.f),
	m_fSelectTime(20.f),
	m_fBettleTime(1000000.f),
	m_fItemTime(30.f),
	m_fLoadingTime(10.f)
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
