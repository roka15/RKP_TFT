#pragma once
#include <Engine\CScript.h>
class CItem:
	public CScript
{
private:
	int		m_Cost;
	int		m_StartTileNum;
	CGameObject* m_pPlayer;          //item을 관리하는 player 정보. 주의할 점은 캐릭터 Obj는 Tile의 자식이다. Player의 자식이 아님을 주의.
public:
	void SetCost(int _iCost) { m_Cost = _iCost; }
	void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }
	void SetStartTileNum(int _iNum) { m_StartTileNum = _iNum; }
	CGameObject* GetPlayer() { return m_pPlayer; }
	const int& GetCost() { return m_Cost; }
	const int& GetStartTileNum() { return m_StartTileNum; }
public:
	CItem();
	CItem(SCRIPT_TYPE _eType);
	CItem(const CItem& _ref);
	virtual ~CItem();
	CLONE(CItem);
};

