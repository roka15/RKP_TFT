#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnimation3D;

class CAnimator3D :
    public CComponent
{
private:
    vector<CAnimation3D*>       m_AniList;
    int                         m_iCurIdx;
    bool                        m_bBlending;
    CStructuredBuffer* m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
 

public:
    virtual void finaltick() override;
    bool UpdateData();

public:
    void RegisterAniClip(const vector<wstring>& _vecAnimClipList);
    int RegisterAniClip(const wstring& _strAnimClip);
    void RemoveAniClip(const vector<wstring>& _vecAnimClipList);
    void RemoveAniClip(const wstring& _strAnimClip);
    void ChangeAniClip(const wstring& _strAnimClip);

    CAnimation3D* GetAnimation();
    //void SetClipTime(int _iClipIdx, float _fTime);

    UINT GetBoneCount();
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    void ClearData();
  
private:
    void check_bone();

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};