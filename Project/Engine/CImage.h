#pragma once
#include "CUI.h"
class CImage :
    public CUI
{
private:
    Vec4            m_vec4Color;
    wstring         m_TextureKey;
public:
    virtual void finaltick()override {}
    virtual void SaveToLevelFile(FILE* _File)override {};
    virtual void LoadFromLevelFile(FILE* _FILE) override {};
public:
    void UpdateData();
    void SetTextureKey(wstring _key) { m_TextureKey = _key; }
    void SetColor(Vec4 _color) { m_vec4Color = _color; }
    const wstring& GetTextureKey() { return m_TextureKey; }
    const Vec4& GetColor() { return m_vec4Color; }
public:
    CLONE(CImage);
    CImage();
    CImage(const CImage& _ref);
    virtual ~CImage();
};

