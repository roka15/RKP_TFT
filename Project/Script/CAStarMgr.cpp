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
	//p1 대기석 - p1 전투석 - p2 전투석 - p2 대기석 순으로 저장되기 때문에
	//p1 대기석 갯수를 빼줘야 함.
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
	//p1 대기석 - p1 전투석 - p2 전투석 - p2 대기석 순으로 저장되기 때문에
	//p1 대기석 갯수를 빼줘야 함.
	//AStar 함수들은 대기석을 제외한 전투석의 육각 타일들에 대해서만 논리적으로 계산한다.
	int Offset = CTileMgr::GetInst()->GetWaitCount().x * -1;
	Vec2 count = CTileMgr::GetInst()->GetBattleCount();
	int width = count.x;
	int height = count.y * 2;// 생성할때 p1 영역 , p2 영역 나눠서 절반으로 생성하기 때문에 *2 해줘야 함.
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
	startNode.Parent = Vec2{ -1,-1 }; // start 라는 의미
	queue.push(startNode);
	while (queue.empty()==false)
	{
		t_AStarNode curNode = queue.top();
		queue.pop();

		closeList[curNode.Cur.y][curNode.Cur.x] = true;
		int iCurNodeIndex = curNode.Cur.y * width + curNode.Cur.x;
		//육각형이기 때문.
		// ↖ ↗
		// ←  → 
		// ↙ ↘
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
			//배열 범위를 벗어나는 지 체크
			if (x < 0 || x >= width)
				continue;
			if (y < 0 || y >= height)
				continue;
			//이미 close 항목인지 체크
			if (closeList[y][x] == true)
				continue;
			//이동이 예약된 목록인지
			auto itr = m_mapReserv.find(iCurNodeIndex);
			if (itr != m_mapReserv.end() && m_mapReserv[iCurNodeIndex])
				continue;
			//벽인지 체크
			bool bEmpty = CTileMgr::GetInst()->EmptyBattleTile(x + (y * width));
			if (bEmpty == false)
				int a = 0;
			//종료 지점은 타겟이 존재하기 때문에 오브젝트가 있다.그래서 EmptyTile 검사하면 false가 나옴.
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

			//open에 등록이 안되어 있거나, 이동 비용이 기존에 기록된 값보다 작은 경우
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
