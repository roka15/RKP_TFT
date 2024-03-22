#pragma once
#include <Engine\CScript.h>
class CItem:
	public CScript
{
private:
	int		m_Cost;
	CGameObject* m_pPlayer;          //item�� �����ϴ� player ����. ������ ���� ĳ���� Obj�� Tile�� �ڽ��̴�. Player�� �ڽ��� �ƴ��� ����.
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

