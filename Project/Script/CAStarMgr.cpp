#include "pch.h"
#include "CAStarMgr.h"
#include <Engine\CPathMgr.h>
#include "CGameMgr.h"
#include "CTileMgr.h"
#include "CTileScript.h"

CAStarMgr::CAStarMgr() :m_iDirCnt(6)
{

}
CAStarMgr::~CAStarMgr()
{
	delete m_arrDirY;
	delete m_arrOddDirX;
	delete m_arrEvenDirX;
}
int CAStarMgr::SearchTarget(UINT _startNode, int& _out_distance,CGameObject* _pPlayer)
{
	//p1 ��⼮ - p1 ������ - p2 ������ - p2 ��⼮ ������ ����Ǳ� ������
	//p1 ��⼮ ������ ����� ��.
	int Offset = CTileMgr::GetInst()->GetWaitCount().x * -1;
	vector<int> vecEnumyNums = CTileMgr::GetInst()->SearchEnemyTile(_pPlayer);
	int size = vecEnumyNums.size();
	int min = 100000;
	int iTargetNum = -1;
	for (int i = 0; i < size; ++i)
	{
		Vec2 StartIdx = ConvertNumberToOffset(_startNode + Offset);
 		Vec2 EndIdx = ConvertNumberToOffset(vecEnumyNums[i] + Offset);
		Vec3 cube1 = ConvertOffsetToCube(StartIdx.x, StartIdx.y);
		Vec3 cube2 = ConvertOffsetToCube(EndIdx.x, EndIdx.y);
		int diff = CalcuDistance(cube1, cube2);
		if (min > diff)
		{
			min = diff;
			iTargetNum = vecEnumyNums[i];
		}
	}
	_out_distance = min;
	return iTargetNum;
}
vector<int> CAStarMgr::GetNextNodeAStar(UINT _startNode, UINT _endNode)
{
	//p1 ��⼮ - p1 ������ - p2 ������ - p2 ��⼮ ������ ����Ǳ� ������
	//p1 ��⼮ ������ ����� ��.
	//AStar �Լ����� ��⼮�� ������ �������� ���� Ÿ�ϵ鿡 ���ؼ��� �������� ����Ѵ�.
	int Offset = CTileMgr::GetInst()->GetWaitCount().x * -1;
	Vec2 count = CTileMgr::GetInst()->GetBattleCount();
	int width = count.x;
	int height = count.y * 2;// �����Ҷ� p1 ���� , p2 ���� ������ �������� �����ϱ� ������ *2 ����� ��.
	vector<int> Result = {};
	int iStartNum = _startNode + Offset;
	int iEndNum = _endNode + Offset;
	if (iStartNum == iEndNum)
		return Result;
	tAStarResult result = {};
	vector<vector<t_AStarNode>> openList = {};
	vector<vector<bool>> closeList = { {false,} };
	openList.resize(height);
	closeList.resize(height);
	for (int i = 0; i < height; ++i)
	{
		openList[i].resize(width);
		closeList[i].resize(width);
	}

	Vec2 StartIdx = ConvertNumberToOffset(iStartNum);
	Vec2 EndIdx = ConvertNumberToOffset(iEndNum);
	int* dirY = m_arrDirY;
	int* evenDirX = m_arrEvenDirX;
	int* oddDirX = m_arrOddDirX;
	const int cost = 1;
	const int dirCnt = 6;
	priority_queue<t_AStarNode, vector<t_AStarNode>, AStarNodeLess> queue;
	t_AStarNode startNode = {};
	startNode.Cur = StartIdx;
	startNode.Parent = Vec2{ -1,-1 }; // start ��� �ǹ�
	queue.push(startNode);
	while (queue.empty()==false)
	{
		t_AStarNode curNode = queue.top();
		queue.pop();

		closeList[curNode.Cur.y][curNode.Cur.x] = true;
		int iCurNodeIndex = curNode.Cur.y * width + curNode.Cur.x;
		//�������̱� ����.
		// �� ��
		// ��  �� 
		// �� ��
		if (curNode.Cur.x == EndIdx.x && curNode.Cur.y == EndIdx.y)
		{
			t_AStarNode Node = curNode;
			vector<Vec2> history;
			while (1)
			{
				int x = Node.Parent.x;
				int y = Node.Parent.y;
				Result.push_back(y * width + x);
				if (x == StartIdx.x && y == StartIdx.y)
				{
					return Result;
				}
				Node = openList[y][x];
			}

		}

		for (int i = 0; i < m_iDirCnt; ++i)
		{
			int x = curNode.Cur.x;
			if ((int)curNode.Cur.y % 2 == 0)
			{
				x += evenDirX[i];
			}
			else
			{
				x += oddDirX[i];
			}

			int y = curNode.Cur.y + dirY[i];
			//�迭 ������ ����� �� üũ
			if (x < 0 || x >= width)
				continue;
			if (y < 0 || y >= height)
				continue;
			//�̹� close �׸����� üũ
			if (closeList[y][x] == true)
				continue;
			//�̵��� ����� �������
			auto itr = m_mapReserv.find(iCurNodeIndex);
			if (itr != m_mapReserv.end() && m_mapReserv[iCurNodeIndex])
				continue;
			//������ üũ
			bool bEmpty = CTileMgr::GetInst()->EmptyBattleTile(x + (y * width));
			if (bEmpty == false)
				int a = 0;
			//���� ������ Ÿ���� �����ϱ� ������ ������Ʈ�� �ִ�.�׷��� EmptyTile �˻��ϸ� false�� ����.
			if ((x != EndIdx.x || y != EndIdx.y) && bEmpty == false)
				continue;

			Vec3 cube1 = ConvertOffsetToCube(x, y);
			Vec3 cube2 = ConvertOffsetToCube(EndIdx.x, EndIdx.y);
			int diff = CalcuDistance(cube1, cube2);

			t_AStarNode nextNode = {};
			nextNode.Cur = Vec2{ x,y };
			nextNode.Parent = curNode.Cur;
			nextNode.G = curNode.G + cost;
			nextNode.H = diff;
			nextNode.F = nextNode.G + nextNode.H;

			//open�� ����� �ȵǾ� �ְų�, �̵� ����� ������ ��ϵ� ������ ���� ���
			if (openList[y][x].F == 0 || openList[y][x].G > nextNode.G)
			{
				openList[y][x] = nextNode;
				queue.push(nextNode);
			}
		}
	}
	return Result;
}
void CAStarMgr::Reserve(int _iNode)
{
	auto itr = m_mapReserv.find(_iNode);
	if (itr == m_mapReserv.end())
	{
		m_mapReserv.insert(std::make_pair(_iNode,false));
	}
	m_mapReserv[_iNode] = true;
}
void CAStarMgr::CancelReserve(int _iNode)
{
	m_mapReserv[_iNode] = false;
}
void CAStarMgr::SaveFile(FILE* _pFile)
{
}
void CAStarMgr::LoadFile(FILE* _pFile)
{
}
Vec3 CAStarMgr::ConvertOffsetToCube(UINT _col, UINT _row)
{
	/*if (_row % 2 != 0)
		_col += 1;*/
	
	int q = _col - (_row + (_row & 1)) / 2;
	int r = _row;
	return Vec3(q, r, -q - r);
}
int CAStarMgr::CalcuDistance(Vec3 _start, Vec3 _end)
{
	int x = abs(_start.x - _end.x);
	int y = abs(_start.y - _end.y);
	int z = abs(_start.z - _end.z);

	int result = max(x, y);
	result = max(result, z);
	return result;
}
Vec2 CAStarMgr::ConvertNumberToOffset(int _iTileNum)
{
	Vec2 count = CTileMgr::GetInst()->GetBattleCount();
	int width = count.x;
	int height = count.y;

	int x = _iTileNum % width;
	int y = _iTileNum / width;
	return Vec2(x, y);
}
void CAStarMgr::init()
{
	m_arrDirY = new int[m_iDirCnt] {1, 1, 0, -1, -1, 0};
	m_arrOddDirX = new int[m_iDirCnt] {-1, 0, 1, 0, -1, -1}; 
	m_arrEvenDirX = new int[m_iDirCnt] { 0, 1, 1, 1, 0, -1 };
}
void CAStarMgr::tick()
{
}
