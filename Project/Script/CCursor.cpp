#include "pch.h"
#include "CCursor.h"
#include <Engine\CMouseMgr.h>

void CCursor::RegisterDragItem(CGameObject* _obj)
{
	GetOwner()->AddChild(_obj);
}

void CCursor::tick()
{
}

void CCursor::BeginOverlap(CCollider* _Other)
{
}

void CCursor::OnOverlap(CCollider* _Other)
{
}

void CCursor::EndOverlap(CCollider* _Other)
{
}

void CCursor::DownEvent(PointerEventData _data)
{
}

void CCursor::UpEvent(PointerEventData _data)
{
	const vector<CGameObject*>& childs = GetOwner()->GetChild();
	if (_data.pointerPress == nullptr)
	{
		if (childs.size() == 0)
			return;
		CGameObject* dragObj = childs[0];
		_data.lastPress->AddChild(dragObj);
	}
	else
	{
		if (childs.size() == 0)
			return;
		CGameObject* dragObj = childs[0];
		_data.pointerPress->AddChild(dragObj);
	}
}

CCursor::CCursor() :CScript((UINT)SCRIPT_TYPE::CURSOR)
{
}

CCursor::~CCursor()
{
}
