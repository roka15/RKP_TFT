#include "pch.h"
#include "CAStarMgr.h"
#include <Engine\CPathMgr.h>
#include "CTileMgr.h"

CAStarMgr::CAStarMgr()
{

}
CAStarMgr::~CAStarMgr()
{

}
vector<int> CAStarMgr::GetNextNodeAStar(UINT _startNode, UINT _endNode, UINT _width, UINT _height)
{
	vector<int> Result = {};
	if (_startNode == _endNode)
		return Result;
	tAStarResult result = {};
	vector<vector<t_AStarNode>> openList = {};
	vector<vector<bool>> closeList = { {false,} };
	openList.resize(_height);
	closeList.resize(_height);
	for (int i = 0; i < _height; ++i)
	{
		openList[i].resize(_width);
		closeList[i].resize(_width);
	}

	Vec2 StartIdx = Vec2{ (int)_startNode % _width , _startNode / _width };
	Vec2 EndIdx = Vec2{ (int)_endNode % _width , _endNode / _width };
	int dirY[] = { -1, -1, 0, 1, 1, 0 };
	int evenDirX[] = { 0,1,1,1,0,-1 };
	int oddDirX[] = { -1,0,1,0,-1,-1 };
	const int cost = 1;
	const int dirCnt = 6;
	priority_queue<t_AStarNode, vector<t_AStarNode>, AStarNodeLess> queue;
	t_AStarNode startNode = {};
	startNode.Cur = StartIdx;
	startNode.Parent = Vec2{ -1,-1 }; // start 라는 의미
	queue.push(startNode);
	while (1)
	{
		t_AStarNode curNode = queue.top();
		queue.pop();

		closeList[curNode.Cur.y][curNode.Cur.x] = true;
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
				Result.push_back(y * _width + x);
				if (x == StartIdx.x && y == StartIdx.y)
				{
					return Result;
				}
				Node = openList[y][x];
			}

		}

		for (int i = 0; i < dirCnt; ++i)
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
			if (x < 0 || x >= _width)
				continue;
			if (y < 0 || y >= _height)
				continue;
			//이미 close 항목인지 체크
			if (closeList[y][x] == true)
				continue;
			//벽인지 체크
			bool bEmpty = CTileMgr::GetInst()->EmptyBattleTile(x * y);
			if (bEmpty == false)
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
void CAStarMgr::SaveFile(FILE* _pFile)
{
}
void CAStarMgr::LoadFile(FILE* _pFile)
{
}
Vec3 CAStarMgr::ConvertOffsetToCube(UINT _col, UINT _row)
{
	if (_row % 2 != 0)
		_col += 1;
	int q = _col - (_row + (_row & 1)) / 2;
	int r = _row;
	return Vec3(q, r,-q-r);
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
void CAStarMgr::init()
{
}
void CAStarMgr::tick()
{
}
