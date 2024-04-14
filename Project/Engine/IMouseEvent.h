#pragma once

struct PointerEventData;
class IMouseExitEvent
{
public:
	virtual void ExitEvent(PointerEventData) = 0;
};
class IMouseEnterEvent
{
public:
	virtual void EnterEvent(PointerEventData) = 0;
};
class IMouseDownEvent
{
public:
	virtual void DownEvent(PointerEventData) = 0;
};

class IMouseDragEvent
{
public:
	virtual void DragEvent(PointerEventData) = 0;
};

class IMouseUpEvent
{
public:
	virtual void UpEvent(PointerEventData) = 0;
};