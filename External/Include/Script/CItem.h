#pragma once
#include <Engine\CScript.h>
class CItem:
	public CScript
{
private:
	int		m_Cost;
	CGameObject* m_pPlayer;          //item을 관리하는 player 정보. 주의할 점은 캐릭터 Obj는 Tile의 자식이다. Player의 자식이 아님을 주의.
protected:
	void SetCost(int _iCost) { m_Cost = _iCost; }
	CGameObject* GetPlayer() { return m_pPlayer; }
public:
	virtual int Buy(const int& _iMoney);
	void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }
public:
	CItem();
	CItem(SCRIPT_TYPE _eType);
	virtual ~CItem();
	CLONE(CItem);
};

