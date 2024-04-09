#pragma once
#include "CUI.h"
class CText :
    public CUI
{
private:
    wstring     m_Text;
    float       m_fSize;
    Vec2        m_v2Pos;
    Vec4        m_v4Color;
public:
    virtual void finaltick()override;
    virtual void SaveToLevelFile(FILE* _File)override {};
    virtual void LoadFromLevelFile(FILE* _FILE)override {};
public:
    void UpdateData();
    void SetText(wstring _text) { m_Text = _text; }
    void SetSize(float _size) { m_fSize = _size; }
    void SetColor(Vec4 _color) { m_v4Color = _color; }
    const wstring& GetText() { return m_Text; }
    const float& GetSize() { return m_fSize;}
    UINT GetColor();
public:
    CLONE(CText);
    CText();
    CText(const CText& _ref);
    virtual ~CText();
};

