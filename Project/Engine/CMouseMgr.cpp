#include "pch.h"
#include "CMouseMgr.h"
#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCollisionMgr.h"
#include "CGameObject.h"
#include "components.h"
#include "IMouseEvent.h"
#include "CScript.h"


void CMouseMgr::init()
{
	CBoxCollider* collider = (CBoxCollider*)m_pCursor->GetComponent(COMPONENT_TYPE::COLLIDER3D);
	collider->SetOffsetScale(Vec3{ 100.f,100.f,100.f });
	m_pCursor->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-90.f),0.f,0.f });
}

void CMouseMgr::tick()
{
	m_fCurTime += DT;
	vector<CGameObject*> objs;
	PointerEventData curEventInfo = {};
	
	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP ||
		CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
	{
		ClickPlane();
	}
	m_pCursor->tick();
	m_pCursor->finaltick_module();
	objs = CCollisionMgr::GetInst()->CursorCollisionTick();

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP ||
		CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
	{
		MouseDownEvent(objs, curEventInfo);
		MouseDragEvent(objs, curEventInfo);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::RELEASE)
	{
		MouseUpEvent(objs, curEventInfo);
		m_pCursor->Transform()->SetRelativePos(Vec3{ -50000.f,0.f,0.f });
	}
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

void CMouseMgr::MouseDownEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data)
{

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP)
	{
		_data.button = MOUSE_INPUT_TYPE::LEFT;
		if (m_fCurTime - m_BeforEventInfo.clickTime < 1.f)
			_data.clickCount = m_BeforEventInfo.clickCount + 1;
		else
			_data.clickCount = 1;
		_data.clickTime = m_fCurTime;

		if (_pObjs.size() == 0)
		{
			_data.pointerPress = nullptr;
			_data.lastPress = _data.pointerPress;
		}
		else //클릭한 곳에 Object가 있다면.
		{
			_data.pointerPress = _pObjs[0];
			_data.lastPress = _data.pointerPress;

			vector<CScript*> vecScript = _pObjs[0]->GetScripts();
			for (int i = 0; i < vecScript.size(); ++i)
			{
				CScript* script = vecScript[i];
				IMouseDownEvent* pEvent = dynamic_cast<IMouseDownEvent*>(script);
				if (pEvent)
					pEvent->DownEvent(_data);
			}
		}

		//cursor
		vector<CScript*> vecScript = m_pCursor->GetScripts();
		for (int i = 0; i < vecScript.size(); ++i)
		{
			CScript* script = vecScript[i];
			IMouseDownEvent* pEvent = dynamic_cast<IMouseDownEvent*>(script);
			if (pEvent)
				pEvent->DownEvent(_data);
		}

		m_BeforEventInfo = _data;
	}
	return;
}

void CMouseMgr::MouseDragEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data)
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
	{

	}
	return;
}

void CMouseMgr::MouseUpEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data)
{
	_data.lastPress = m_BeforEventInfo.lastPress;

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::RELEASE)
	{
		if (_pObjs.size() == 0)
		{
			_data.pointerPress = nullptr;
		}
		else
		{
			_data.pointerPress = _pObjs[0];

			vector<CScript*> vecScript = _pObjs[0]->GetScripts();
			for (int i = 0; i < vecScript.size(); ++i)
			{
				CScript* script = vecScript[i];
				IMouseUpEvent* pEvent = dynamic_cast<IMouseUpEvent*>(script);
				if (pEvent)
					pEvent->UpEvent(_data);
			}
		}
	}

	//cursor
	vector<CScript*> vecScript = m_pCursor->GetScripts();
	for (int i = 0; i < vecScript.size(); ++i)
	{
		CScript* script = vecScript[i];
		IMouseUpEvent* pEvent = dynamic_cast<IMouseUpEvent*>(script);
		if (pEvent)
			pEvent->UpEvent(_data);
	}
	m_BeforEventInfo = {};
	return;
}

CMouseMgr::CMouseMgr()
{
}

CMouseMgr::~CMouseMgr()
{
	delete m_pCursor;
}
