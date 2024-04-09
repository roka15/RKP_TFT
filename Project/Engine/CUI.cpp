#include "pch.h"
#include "CUI.h"


CUI::CUI():CComponent(COMPONENT_TYPE::UI)
{
}

CUI::CUI(COMPONENT_TYPE _eType) : CComponent(_eType)
{
}

CUI::CUI(const CUI& _ref):CComponent(_ref)
{
}

CUI::~CUI()
{
}
