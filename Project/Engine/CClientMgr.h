#pragma once
#include "CSingleton.h"
class IClientManager
{
public:
    virtual void init() = 0;
    virtual void tick() = 0;
};
class CClientMgr :
    public CSingleton<CClientMgr>
{
    SINGLE(CClientMgr)
private:
    vector<IClientManager*> m_vecClientManager;
public:
    void init();
    void tick();
    void RegisterManager(IClientManager* _pManager) { m_vecClientManager.push_back(_pManager); }
};

