#pragma once
#include "ComponentUI.h"
class CGameObject;

class ButtonUI :
    public ComponentUI
{
private:
    int     m_iCurObjIdx;
    int     m_iCurFuncIdx;
    wstring m_wstrCurObject;

    int     m_iObjParamIdx;
    int     m_iIntParam;
    float   m_fFloatParam;
    string  m_strStringParam;
private:
    void LinkFuncPtrVoid(CGameObject* _pLinkObj,wstring _strFunc);
    void LinkFuncPtrInt(CGameObject* _pLinkObj, wstring _strFunc);
    void LinkFuncPtrFloat(CGameObject* _pLinkObj, wstring _strFunc);
    void LinkFuncPtrString(CGameObject* _pLinkObj, wstring _strFunc);
    void LinkFuncPtrObject(CGameObject* _pLinkObj, wstring _strFunc);

    vector<CGameObject*> GetObjectList();
public:
    virtual int render_update() override;
public:
    ButtonUI();
    virtual ~ButtonUI();
};

