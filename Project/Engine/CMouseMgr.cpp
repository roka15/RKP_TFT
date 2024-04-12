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
	CBoxCollider* collider = (CBoxCollider*)GetCursor(CURSOR_TYPE::CURSOR_3D)->GetComponent(COMPONENT_TYPE::COLLIDER3D);
	collider->SetOffsetScale(Vec3{ 100.f,100.f,100.f });
	GetCursor(CURSOR_TYPE::CURSOR_3D)->Transform()->SetRelativeRot(Vec3{ DEGREE2RADIAN(-90.f),0.f,0.f });
}

void CMouseMgr::tick()
{
	m_fCurTime += DT;
	vector<CGameObject*> objs;
	PointerEventData curEventInfo = {};

	for (int i = 0; i < (UINT)CURSOR_TYPE::END; ++i)
	{
		CGameObject* pCursor = m_CursorList[i];
		if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP ||
			CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
		{
			CalcuMousePos((CURSOR_TYPE)i);
		}
		pCursor->tick();
		pCursor->finaltick_module();
		objs = CCollisionMgr::GetInst()->CursorCollisionTick(i);

		if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP ||
			CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
		{
			MouseDownEvent(objs, curEventInfo, (CURSOR_TYPE)i);
			MouseDragEvent(objs, curEventInfo, (CURSOR_TYPE)i);
		}
		else if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::RELEASE)
		{
			MouseUpEvent(objs, curEventInfo, (CURSOR_TYPE)i);
			pCursor->Transform()->SetRelativePos(Vec3{ -50000.f,0.f,0.f });
		}
	}
}

void CMouseMgr::render()
{
	for (int i = 0; i < (UINT)CURSOR_TYPE::END; ++i)
	{
		CGameObject* pCursor = m_CursorList[i];
		pCursor->render();
	}
}

void CMouseMgr::CalcuMousePos(CURSOR_TYPE _eType)
{
	switch (_eType)
	{
	case CURSOR_TYPE::CURSOR_2D:
	{
		CalcuMousePos2D(); // UI Cursor의 위치 설정
		break;
	}
	case CURSOR_TYPE::CURSOR_3D:
		CalcuMousePos3D(); // Game Cursor의 위치 설정
		break;
	}
}

void CMouseMgr::CalcuMousePos2D()
{
	CCamera* pUICam = CRenderMgr::GetInst()->GetCam(1);
	POINT cursorPos = {};
	GetCursorPos(&cursorPos);
	Vec3 Pos = { (float)cursorPos.x,(float)cursorPos.y,0.0f };
	Viewport view;

	view.width = GlobalData.Resolution.x;
	view.height = GlobalData.Resolution.y;
	view.x = 0;
	view.y = 0;
	view.minDepth = 0.0f;
	view.maxDepth = 1.0f;
	Pos = view.Unproject(Pos, pUICam->GetProjMat(), pUICam->GetViewMat(), IDENTITY_MAT);
	GetCursor(CURSOR_TYPE::CURSOR_2D)->Transform()->SetRelativePos(Pos);
}

void CMouseMgr::CalcuMousePos3D()
{
	ClickPlane();
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
	GetCursor(CURSOR_TYPE::CURSOR_3D)->Transform()->SetRelativePos(spawnPos);
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

void CMouseMgr::CursorDownEvent(PointerEventData& _data, CURSOR_TYPE _eType)
{
	//cursor
	vector<CScript*> vecScript = GetCursor(_eType)->GetScripts();
	for (int i = 0; i < vecScript.size(); ++i)
	{
		CScript* script = vecScript[i];
		IMouseDownEvent* pEvent = dynamic_cast<IMouseDownEvent*>(script);
		if (pEvent)
			pEvent->DownEvent(_data);
	}
}

void CMouseMgr::CursorUpEvent(PointerEventData& _data, CURSOR_TYPE _eType)
{
	//cursor
	vector<CScript*> vecScript = GetCursor(_eType)->GetScripts();
	for (int i = 0; i < vecScript.size(); ++i)
	{
		CScript* script = vecScript[i];
		IMouseUpEvent* pEvent = dynamic_cast<IMouseUpEvent*>(script);
		if (pEvent)
			pEvent->UpEvent(_data);
	}
}

void CMouseMgr::MouseDownEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data, CURSOR_TYPE _eType)
{

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP)
	{
		_data.button = MOUSE_INPUT_TYPE::LEFT;
		if (m_fCurTime - m_BeforEventInfo[(UINT)_eType].clickTime < 1.f)
			_data.clickCount = m_BeforEventInfo[(UINT)_eType].clickCount + 1;
		else
			_data.clickCount = 1;
		_data.clickTime = m_fCurTime;

		if (_pObjs.size() == 0)
		{
			_data.pointerPress = nullptr;
			_data.lastPress = _data.pointerPress;
			CursorDownEvent(_data, _eType);
		}
		else //클릭한 곳에 Object가 있다면.
		{
			_data.pointerPress = _pObjs[0];
			_data.lastPress = _data.pointerPress;

			CursorDownEvent(_data, _eType);

			vector<CScript*> vecScript = _pObjs[0]->GetScripts();
			for (int i = 0; i < vecScript.size(); ++i)
			{
				CScript* script = vecScript[i];
				IMouseDownEvent* pEvent = dynamic_cast<IMouseDownEvent*>(script);
				if (pEvent)
					pEvent->DownEvent(_data);
			}
			for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
			{
				CComponent* pComponent = _pObjs[0]->GetComponent((COMPONENT_TYPE)i);
				IMouseDownEvent* pEvent = dynamic_cast<IMouseDownEvent*>(pComponent);
				if (pEvent)
					pEvent->DownEvent(_data);
			}
		}



		m_BeforEventInfo[(UINT)_eType] = _data;
	}
	return;
}

void CMouseMgr::MouseDragEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data, CURSOR_TYPE _eType)
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::PRESSED)
	{

	}
	return;
}

void CMouseMgr::MouseUpEvent(vector<CGameObject*>& _pObjs, PointerEventData& _data, CURSOR_TYPE _eType)
{
	_data.lastPress = m_BeforEventInfo[(UINT)_eType].lastPress;

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::RELEASE)
	{
		if (_pObjs.size() == 0)
		{
			_data.pointerPress = nullptr;
			CursorUpEvent(_data, _eType);
		}
		else
		{
			_data.pointerPress = _pObjs[0];
			CursorUpEvent(_data, _eType);
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

	m_BeforEventInfo[(UINT)_eType] = {};

	return;
}

CMouseMgr::CMouseMgr()
{
}

CMouseMgr::~CMouseMgr()
{
	for (int i = 0; i < (UINT)CURSOR_TYPE::END; ++i)
	{
		delete m_CursorList[i];
	}
}
