#pragma once
#include <Engine\CScript.h>

enum class CHARACTER_TYPE;
enum class ITEM_CARD_TYPE;
class CCard
	:public CScript , public IMouseEnterEvent,public IMouseExitEvent
{
public:
	void SetInfo(CHARACTER_TYPE _eChType,ITEM_CARD_TYPE _eCardType);
public:
	virtual void start()override {};
	virtual void BeginOverlap(CCollider* _Other)override {}
	virtual void OnOverlap(CCollider* _Other)override {}
	virtual void EndOverlap(CCollider* _Other)override {}
	virtual void EnterEvent(PointerEventData) override;
	virtual void ExitEvent(PointerEventData)override;
public:
	CLONE(CCard);
	CCard();
	virtual ~CCard();
};

