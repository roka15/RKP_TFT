#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnimation3D;
class CAnimatorController;

class CAnimator3D :
    public CComponent
{
private:
    int                         m_iCurIdx;
    bool                        m_bBlending;
    CStructuredBuffer* m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
    Ptr<CAnimatorController> m_pController;
public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetController(Ptr<CAnimatorController> _pController) { m_pController = _pController; }
    void SetController(wstring _strName);
    Ptr<CAnimatorController> GetController() { return m_pController; }
    const wstring& GetCurControllerName();
    //void SetClipTime(int _iClipIdx, float _fTime);

    UINT GetBoneCount();
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    void ClearData();

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};