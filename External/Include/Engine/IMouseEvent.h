#pragma once

struct PointerEventData;
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