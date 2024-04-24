#pragma once
#include <Engine\CSingleton.h>
#include <Engine\CGameObject.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"




// ========
// Delegate
// ========
class UI;
class AniControllerEditUI;
typedef void (UI::* UI_DELEGATE)(void);
typedef void (UI::* UI_DELEGATE_1)(DWORD_PTR);
typedef void (UI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

struct ComboFunc
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
class ImGuiMgr :
    public CSingleton<ImGuiMgr>
{
    SINGLE(ImGuiMgr);
private:
    HWND                     m_hMainHwnd;
    map<string, UI*>         m_mapUI;

    HANDLE                   m_hObserver;
    ImGuiContext*            m_pContext;
public:
    void init(HWND _hWnd);
    void progress();
    void release();
public:
    UI* FindUI(const string& _UIName);
    HWND GetMainHwnd() { return m_hMainHwnd; }
    ImTextureID LoadTexture(const char* _path);
    int GetTextureHeight(ImTextureID _texture);
    int GetTextureWidth(ImTextureID _texture);
    ImGuiWindowFlags GetWindowFlags()const;
private:
    void CreateUI();
    void ObserveContent();

    void begin();
    void tick();
    void finaltick();
    void render();

    ImGuiContext* GetGuiContext() { return m_pContext; }

private:
    friend class AniControllerEditUI;
};

