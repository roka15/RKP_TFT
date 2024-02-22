#pragma once
#include "CStateScript.h"
#define DECLARE_NAME(n) virtual wstring GetStateName(){return (n);}
class IChState
{
    virtual wstring GetStateName()=0;
};

class CCharacterStateScript :
    public CStateScript
{
public:
    virtual void OnEntry(CStateMachineScript* _pSMachine, CStateScript* _pState)override;
    virtual void OnExit(CStateMachineScript* _pSMachine, CStateScript* _pState)override;
    CLONE(CCharacterStateScript)
public:
    CCharacterStateScript();
    CCharacterStateScript(SCRIPT_TYPE _eType);
    virtual ~CCharacterStateScript();
};

