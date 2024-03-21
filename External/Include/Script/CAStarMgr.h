#pragma once
#include <Engine\CSingleton.h>
#include <Engine\CClientMgr.h>
struct tAStarResult
{
	UINT	iCost;
	vector<UINT> vecRoute;
};
struct t_AStarNode
{
	Vec2 Cur;
	Vec2 Parent;
	UINT G;
	UINT H;
	UINT F;
};
struct AStarNodeLess
{
	bool operator()(const t_AStarNode& _node1, const t_AStarNode& _node2)const
	{
		return _node1.F > _node2.F;
	}
};
class CAStarMgr :
	public CSingleton<CAStarMgr>, public IClientManager
{
	SINGLE(CAStarMgr)
private:
	const int m_iDirCnt;
	int* m_arrDirY;
	int* m_arrOddDirX;
	int* m_arrEvenDirX;
private:
	void SaveFile(FILE*);
	void LoadFile(FILE*);
private:
	Vec3 ConvertOffsetToCube(UINT _col, UINT _row);
	int CalcuDistance(Vec3 _start,Vec3 _end);
	Vec2 ConvertNumberToOffset(int _iTileNum);
public:
	virtual void init()override;
	virtual void tick()override;
public:
	int SearchTarget(UINT _startNode);
	vector<int> GetNextNodeAStar(UINT _startNode, UINT _endNode);
};

