#include "pch.h"
#include "ButtonUI.h"
#include <Engine\define.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\CButton.h>
ButtonUI::ButtonUI() : ComponentUI("##BUTTON_COMPONENT", COMPONENT_TYPE::BUTTON),
m_iCurObjIdx(0),
m_iCurFuncIdx(0),
m_iObjParamIdx(0),
m_iIntParam(0),
m_fFloatParam(0.0f),
m_strStringParam("")
{
	SetName("BUTTON_COMPONENT");
}

ButtonUI::~ButtonUI()
{
}

void ButtonUI::LinkFuncPtrVoid(CGameObject* _pLinkObj, wstring _strFunc)
{
	CGameObject* pButtonObj = GetTarget();
	CButton* pButton = pButtonObj->Button();

	pButton->RegisterLinkFuncPtr(std::bind([_pLinkObj, _strFunc]()->void
		{
			auto Value = _pLinkObj->GetFuncPtrVOID(_strFunc);
			std::function<void()>& FuncRef = Value.value().get();
			FuncRef();
		}));
}

void ButtonUI::LinkFuncPtrInt(CGameObject* _pLinkObj, wstring _strFunc)
{
	CGameObject* pButtonObj = GetTarget();
	CButton* pButton = pButtonObj->Button();


	ImGui::Text("int");
	ImGui::SameLine();
	ImGui::DragInt("##Func Int Param", &m_iIntParam);
	int iVelue = m_iIntParam;
	pButton->RegisterLinkFuncPtr(std::bind([_pLinkObj, iVelue, _strFunc]()->void
		{
			auto Value = _pLinkObj->GetFuncPtrINT(_strFunc);
			std::function<void(int)>& FuncRef = Value.value().get();
			FuncRef(iVelue);
		}));
}

void ButtonUI::LinkFuncPtrFloat(CGameObject* _pLinkObj, wstring _strFunc)
{
	CGameObject* pButtonObj = GetTarget();
	CButton* pButton = pButtonObj->Button();

	ImGui::Text("float");
	ImGui::SameLine();
	ImGui::DragFloat("##Func Float Param", &m_fFloatParam);
	float fParam = m_fFloatParam;
	pButton->RegisterLinkFuncPtr(std::bind([_pLinkObj, fParam, _strFunc]()->void
		{
			auto Value = _pLinkObj->GetFuncPtrFLOAT(_strFunc);
			std::function<void(float)>& FuncRef = Value.value().get();
			FuncRef(fParam);
		}));
}

void ButtonUI::LinkFuncPtrString(CGameObject* _pLinkObj, wstring _strFunc)
{
	CGameObject* pButtonObj = GetTarget();
	CButton* pButton = pButtonObj->Button();
	char strParam[MAX_PATH] = {};
	if (m_strStringParam.size() != 0)
		strcpy_s(strParam, m_strStringParam.size() + 1, m_strStringParam.c_str());
	ImGui::Text("string");
	ImGui::SameLine();
	ImGui::InputText("##Func String Param", strParam, MAX_PATH);
	m_strStringParam = strParam;
	pButton->RegisterLinkFuncPtr(std::bind([_pLinkObj, strParam, _strFunc]()->void
		{
			auto Value = _pLinkObj->GetFuncPtrSTRING(_strFunc);
			std::function<void(string)>& FuncRef = Value.value().get();
			FuncRef(strParam);
		}));
}

void ButtonUI::LinkFuncPtrObject(CGameObject* _pLinkObj, wstring _strFunc)
{
	CGameObject* pButtonObj = GetTarget();
	CButton* pButton = pButtonObj->Button();

	vector<CGameObject*> vecAllObj = GetObjectList();
	ImGui::Combo("Object Param", &m_iObjParamIdx, &ButtonUIFunc::ObjGetter, vecAllObj.data(), vecAllObj.size());
	CGameObject* objParam = vecAllObj[m_iObjParamIdx];

	pButton->RegisterLinkFuncPtr(std::bind([_pLinkObj, objParam, _strFunc]()->void
		{
			auto Value = _pLinkObj->GetFuncPtrOBJ(_strFunc);
			std::function<void(CGameObject*)>& FuncRef = Value.value().get();
			FuncRef(objParam);
		}));
}

vector<CGameObject*> ButtonUI::GetObjectList()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
	vector<CGameObject*> vecAllObj;
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();
		vecAllObj.insert(vecAllObj.end(), vecObj.begin(), vecObj.end());
	}
	return vecAllObj;
}

int ButtonUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;


	vector<CGameObject*> vecAllObj = GetObjectList();
	//이전 선택된 Object와 현재 인덱스의 Object 이름이 다른 경우 Object의 index가 변화했기 때문에 다시 index 검색.
	if (m_wstrCurObject.size() != 0 && vecAllObj[m_iCurObjIdx]->GetName().compare(m_wstrCurObject) != 0)
	{
		for (int i = 0; i < vecAllObj.size(); ++i)
		{
			if (vecAllObj[i]->GetName().compare(m_wstrCurObject) == 0)
			{
				m_iCurObjIdx = i;
			}
		}
	}
	ImGui::Combo("GameObject", &m_iCurObjIdx, &ButtonUIFunc::ObjGetter, vecAllObj.data(), vecAllObj.size());
	CGameObject* pLinkObj = vecAllObj[m_iCurObjIdx];
	wstring wstrLink = pLinkObj->GetName();
	m_wstrCurObject = wstrLink;
	vector<wstring> FuncPtrList = pLinkObj->GetFuncPtrListName();
	vector<string> strList;
	for (int i = 0; i < FuncPtrList.size(); ++i)
	{
		wstring wKey = FuncPtrList[i];
		string sKey;
		sKey.assign(wKey.begin(), wKey.end());
		sKey.push_back('\0');
		strList.push_back(sKey);
	}

	if (strList.size() == 0)
		return TRUE;
	ImGui::Combo("Function", &m_iCurFuncIdx, &ButtonUIFunc::FuncGetter, strList.data(), strList.size());

	if (m_iCurFuncIdx > m_iCurFuncIdx)
		return TRUE;

	string strCurFunc = strList[m_iCurFuncIdx];
	PARAM_TYPE eParamType = pLinkObj->GetFindFuncType(FuncPtrList[m_iCurFuncIdx]);
	switch (eParamType)
	{
	case PARAM_TYPE::VOID_TYPE:
		LinkFuncPtrVoid(pLinkObj, FuncPtrList[m_iCurFuncIdx]);
		break;
	case PARAM_TYPE::INT:
		LinkFuncPtrInt(pLinkObj, FuncPtrList[m_iCurFuncIdx]);
		break;
	case PARAM_TYPE::FLOAT:
		LinkFuncPtrFloat(pLinkObj, FuncPtrList[m_iCurFuncIdx]);
		break;
	case PARAM_TYPE::STRING:
		LinkFuncPtrString(pLinkObj, FuncPtrList[m_iCurFuncIdx]);
		break;
	case PARAM_TYPE::OBJECT:
		LinkFuncPtrObject(pLinkObj, FuncPtrList[m_iCurFuncIdx]);
		break;
	}
	CGameObject* pOwner = GetTarget();
	CButton* pButton = (CButton*)pOwner->GetComponent(COMPONENT_TYPE::BUTTON);
	//pButton->RegisterLinkFuncPtr(pLinkObj, std::bind([pLinkObj]()->void{pLinkObj->}));
	return TRUE;
}
