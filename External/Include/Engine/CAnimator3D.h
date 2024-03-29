#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnimation3D;
class CAnimatorController;

struct AniEventFunc
{
    map<wstring, std::function<void()>>                  m_mapVoid;
    map<wstring, std::function<void(float)>>             m_mapFloat;
    map<wstring, std::function<void(int)>>               m_mapInt;
    map<wstring, std::function<void(string)>>            m_mapString;
    map<wstring, std::function<void(CGameObject*)>>      m_mapObj;
};
class CAnimator3D :
    public CComponent
{
private:
    int                                         m_iCurIdx;
    bool                                        m_bBlending;
    CStructuredBuffer*                          m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
    Ptr<CAnimatorController>                    m_pController;
    AniEventFunc                                 m_AniEvent;

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetController(Ptr<CAnimatorController> _pController);
    void SetController(wstring _strName);
    Ptr<CAnimatorController> GetController() { return m_pController; }
    const wstring& GetCurControllerName();

    void RegisterAniEventInfoVOID(wstring _Key, std::function<void()> _Func);
    vector<wstring> GetAniEventList();
    std::function<void()>& GetVOID_EventFunc(wstring _Key);
    std::function<void(float)>& GetFLOAT_EventFunc(wstring _Key);
    std::function<void(int)>& GetINT_EventFunc(wstring _Key);
    std::function<void(string)>& GetSTRING_EventFunc(wstring _Key);
    std::function<void(CGameObject*)>& GetOBJ_EventFunc(wstring _Key);
    //void SetClipTime(int _iClipIdx, float _fTime);

    UINT GetBoneCount();
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    void ClearData();
    bool IsActiveAni() { return m_pController != nullptr; }
public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};