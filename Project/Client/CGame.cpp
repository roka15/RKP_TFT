#include "pch.h"
#include "CGame.h"
#include <Engine\CTimeMgr.h>
#include <Script\CPlayerScript.h>
#include <Script\CTileMgr.h>
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
		GameMode = GAME_STATE::SELECT;
		bBattleTile = true;
		break;
	case GAME_STATE::ITEM:
		CompareTime = m_fItemTime;
		GameMode = GAME_STATE::SELECT;
		bBattleTile = true;
		break;
	}

	if (m_fCurTime - m_fStartTime > CompareTime)
	{
		++m_iRoundCnt;
		m_fStartTime = m_fCurTime;

		if (m_iRoundCnt % 4 == 0)
			GameMode = GAME_STATE::ITEM;

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

void CGame::SendGameState(UINT _iState)
{
	for (int i = 0; i < m_vecUsers.size(); ++i)
	{
		CPlayerScript* player = m_vecUsers[i];
		if (player == nullptr)
			continue;

		player->SetGameState(_iState);
	}
}

CGame::CGame()
	:m_iUserCnt(8),
	m_eGameState(GAME_STATE::SELECT),
	m_iRoundCnt(0),
	m_fStartTime(0.f),
	m_fCurTime(0.f),
	m_fSelectTime(20.f),
	m_fBettleTime(30.f),
	m_fItemTime(60.f)
{
}

CGame::~CGame()
{
}
