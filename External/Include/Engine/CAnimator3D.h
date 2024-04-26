#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnimation3D;
class CAnimatorController;

struct t_AniParams
{
    map<wstring, int>   mapIntParams;
    map<wstring, float> mapFloatParams;
    map<wstring, bool>  mapBoolParams;
    map<wstring, bool>  mapTriggerParams;
};
class CAnimator3D :
    public CComponent
{
private:
    int                                         m_iCurIdx;
    bool                                        m_bBlending;
    float                                       m_fBlendingTime;
    float                                       m_fCurTime;
  
    CStructuredBuffer*                          m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
    Ptr<CAnimatorController>                    m_pController;
    map<wstring, CAnimation3D*>                 m_mapAnimation;
    CAnimation3D*                               m_pCurAnimation;
    CAnimation3D*                               m_pNextAnimation;
    t_AniParams                                 m_AniParams;
private:
    bool ChangeAnimation(wstring _AniKey,float _fBlendTime);
  
public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetController(Ptr<CAnimatorController> _pController);
    void SetController(wstring _strName);
    Ptr<CAnimatorController> GetController() { return m_pController; }
    const wstring& GetCurControllerName();

    //void SetClipTime(int _iClipIdx, float _fTime);

    void RegisterAnimation(wstring _AniClipName);
 
    //params
    bool SetIntParam(wstring _strName, int _iValue);
    bool SetFloatParam(wstring _strName, float _fValue);
    bool SetTriggerParam(wstring _strName, bool _bValue);
    bool SetBoolParam(wstring _strName, bool _bValue);
    const t_AniParams& GetAniParamInfo() { return m_AniParams; }
    
    const int& GetIntParam(wstring _strName);
    const float& GetFloatParam(wstring _strName);
    const bool& GetBoolParam(wstring _strName);
    const bool& GetTriggerParam(wstring _strName);

    UINT GetBoneCount();
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    bool IsBlending() { return m_bBlending; }
    float GetBlendRatio() { return m_fCurTime / m_fBlendingTime; }
    void ClearData();
    bool IsActiveAni() { return m_pCurAnimation != nullptr; }
    std::vector<wstring> GetAniList();
    void BlendingEnd();
public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);
    friend class CAnimatorController;
    friend class CTransition;
public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};