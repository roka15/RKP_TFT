#pragma once
#include "CBaseCharacterScript.h"
class CChMinionScript :
    public CBaseCharacterScript
{
public:
    virtual void start()override;
    virtual void Death()override;

public:
	CChMinionScript();
	CChMinionScript(SCRIPT_TYPE _eType);
	CChMinionScript(const CChMinionScript& _ref);
	virtual ~CChMinionScript();
	CLONE(CChMinionScript)
};

