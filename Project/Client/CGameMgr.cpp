#include "pch.h"
#include "CGameMgr.h"
#include <Engine\CTimeMgr.h>
#include <Script\CPlayerScript.h>
#include "CGame.h"
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


