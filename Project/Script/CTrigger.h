#pragma once
#include <Engine\CEntity.h>
class CStateMachineScript;
class CState;
class CTrigger : public CEntity
{

public:
    virtual void notify(CStateMachineScript* _pSMachine, CState* _pState);
    CLONE(CTrigger)
public:
    CTrigger();
    virtual ~CTrigger();
};

