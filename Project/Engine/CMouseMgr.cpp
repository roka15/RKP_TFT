#include "pch.h"
#include "CMouseMgr.h"
#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "components.h"


void CMouseMgr::init()
{
	m_pCursor->Transform()->SetRelativeScale(Vec3{ 100.f,100.f,100.f });
}

void CMouseMgr::tick()
{
	
	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP ||
		CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
		ClickPlane();
	else
	{
		m_pCursor->Transform()->SetRelativePos(Vec3{ -50000.f,0.f,0.f });
	}

	m_pCursor->tick();
	m_pCursor->finaltick_module();
}

void CMouseMgr::render()
{
	m_pCursor->render();
}

void CMouseMgr::ClickPlane()
{
	tRay RayInfo = CRenderMgr::GetInst()->GetMainCam()->GetRay();
	Vec3 Start = RayInfo.vStart;
	Vec3 Dir = RayInfo.vDir;

	//xz 축에 평행한 평면.
	Vec3 NormalVec = { 0.f,-1.f,0.f };
	Vec3 PlaneVec = { 0.f, -200.f,0.f };

	// t = vplen.dot(n) - vstart.dot(n)/vdir.dot(n) 
	float t = PlaneVec.Dot(NormalVec) - Start.Dot(NormalVec) / Dir.Dot(NormalVec);
	Vec3 spawnPos;
	spawnPos.x = Start.x + Dir.x * t;
	spawnPos.y = Start.y + Dir.y * t;
	spawnPos.z = Start.z + Dir.z * t;

	m_pCursor->Transform()->SetRelativePos(spawnPos);
	return;
}

Vec3 CMouseMgr::GetMousePos()
{
	tRay RayInfo = CRenderMgr::GetInst()->GetMainCam()->GetRay();
	Vec3 Start = RayInfo.vStart;
	Vec3 Dir = RayInfo.vDir;

	Vec3 Ray;
	Ray = Start + (Dir * 10000.f);
	return Ray;
}

CMouseMgr::CMouseMgr()
{
}

CMouseMgr::~CMouseMgr()
{
	delete m_pCursor;
}
