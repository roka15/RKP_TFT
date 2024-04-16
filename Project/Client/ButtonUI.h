#pragma once
#include "ComponentUI.h"
class CGameObject;
struct ButtonUIFunc
{
    static bool FuncGetter(void* data, int n, const char** out_str)
    {
        std::string* stringArray = static_cast<std::string*>(data);

        *out_str = stringArray[n].c_str();
        return true;
    }
    static bool ObjGetter(void* data, int n, const char** out_str)
    {
        wstring wstr = ((CGameObject**)data)[n]->GetName();
        static string str;
        str.clear();
        str.assign(wstr.begin(), wstr.end());
        *out_str = str.c_str();
        return true;
    }
};
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

