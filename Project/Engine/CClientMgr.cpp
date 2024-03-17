#include "pch.h"
#include "CClientMgr.h"

CClientMgr::CClientMgr()
{

}
CClientMgr::~CClientMgr()
{

}
void CClientMgr::init()
{
	for (int i = 0; i < m_vecClientManager.size(); ++i)
	{
		m_vecClientManager[i]->init();
	}
}

void CClientMgr::tick()
{
	for (int i = 0; i < m_vecClientManager.size(); ++i)
	{
		m_vecClientManager[i]->tick();
	}
}
