#pragma once
#include "CEntity.h"
class CState;
class CStateMachineScript;
class CTrigger :
    public CEntity
{
public:
    virtual void notify(CStateMachineScript* _pSMachine, CState* _pState);
};

