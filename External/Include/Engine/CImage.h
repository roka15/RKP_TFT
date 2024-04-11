#pragma once
#include "CUI.h"
class CImage :
    public CUI
{
private:
    bool            m_bActiveImage;
    Vec4            m_vec4Color;
    wstring         m_NormalTexKey;
    wstring         m_EnableTexKey;
public:
    virtual void finaltick()override {}
    virtual void SaveToLevelFile(FILE* _File)override {};
    virtual void LoadFromLevelFile(FILE* _FILE) override {};
public:
    void UpdateData();
    void SetActiveImage(bool _flag) { m_bActiveImage = _flag; }
    void SetEnableTexKey(wstring _key) { m_EnableTexKey = _key; }
    void SetNormalTexKey(wstring _key) { m_NormalTexKey = _key; }
    void SetColor(Vec4 _color) { m_vec4Color = _color; }
    const wstring& GetTextureKey() { return m_NormalTexKey; }
    const Vec4& GetColor() { return m_vec4Color; }
public:
    CLONE(CImage);
    CImage();
    CImage(const CImage& _ref);
    virtual ~CImage();
};

