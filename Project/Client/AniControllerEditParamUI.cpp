#include "pch.h"
#include "AniControllerEditParamUI.h"
#include <Engine\CKeyMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\CAnimator3D.h>
#include "TreeUI.h"
#include "ListUI.h"


AniControllerEditParamUI::AniControllerEditParamUI() : UI("##AniParameters")
{
	SetName("Parameters");
}

AniControllerEditParamUI::~AniControllerEditParamUI()
{
}


void AniControllerEditParamUI::init()
{
	
}

void AniControllerEditParamUI::tick()
{
	// 항목 선택시 호출받을 델리게이트 등록
	//pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator3DUI::SelectClip);
}

void AniControllerEditParamUI::finaltick()
{
	UI::finaltick();
	// Esc 눌리면 비활성화
	if (KEY_TAP(KEY::ESC))
	{
		SetActive(false);
		SetLinkKey("");
		SetLinkKey(L"");

		// 모든 UI 포커스 해제
		ImGui::SetWindowFocus(nullptr);
	}
}

int AniControllerEditParamUI::render_update()
{
	wstring wKey = GetWstrLinkKey();
	if (wKey.size() == 0)
		return 0;
	const map<wstring, Ptr<CRes>>& mapController = CResMgr::GetInst()->GetResources(RES_TYPE::ANICONTROLLER);

	Ptr<CRes> pRes = nullptr;
	for (const auto& pair : mapController)
	{
		if (pair.first == wKey)
			pRes = pair.second;
	}
	CAnimatorController* pController = dynamic_cast<CAnimatorController*>(pRes.Get());
	t_AniParams tParams = m_pLink->GetAniParamInfo();
	const map<wstring, int>& mapIntParam = tParams.mapIntParams;
	for (auto itr = mapIntParam.begin(); itr != mapIntParam.end(); ++itr)
	{
		wstring wParamName = itr->first;
		string  ParamName = std::string(wParamName.begin(), wParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(),MAXLEN);
		ImGui::SameLine();
		label += "Int";
		int iValue = itr->second;
		ImGui::InputInt(label.c_str(), &iValue);
		m_pLink->SetIntParam(wParamName, iValue);
	}
	const map<wstring, float>& mapFloatParam = tParams.mapFloatParams;
	for (auto itr = mapFloatParam.begin(); itr != mapFloatParam.end(); ++itr)
	{
		wstring wParamName = itr->first;
		string  ParamName = std::string(wParamName.begin(), wParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(), MAXLEN);
		ImGui::SameLine();
		label += "Float";
		float fValue = itr->second;
		ImGui::InputFloat(label.c_str(), &fValue);
		m_pLink->SetFloatParam(wParamName, fValue);
	}
	const map<wstring, bool>& mapBoolParam = tParams.mapBoolParams;
	for (auto itr = mapBoolParam.begin(); itr != mapBoolParam.end(); ++itr)
	{
		wstring wParamName = itr->first;
		string  ParamName = std::string(wParamName.begin(), wParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(), MAXLEN);
		ImGui::SameLine();
		label += "Bool";
		bool bValue = itr->second;
		if (bValue)
			int a = 0;
		ImGui::Checkbox(label.c_str(), &bValue);
		m_pLink->SetBoolParam(wParamName, bValue);
	}
	const map<wstring, bool>& mapTriggerParam = tParams.mapTriggerParams;
	for (auto itr = mapTriggerParam.begin(); itr != mapTriggerParam.end(); ++itr)
	{
		wstring wParamName = itr->first;
		string  ParamName = std::string(wParamName.begin(), wParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(), MAXLEN);
		ImGui::SameLine();
		label += "Trigger";
		bool bValue = itr->second;
		ImGui::Checkbox(label.c_str(), &bValue);
		m_pLink->SetTriggerParam(wParamName, bValue);
	}
    return S_OK;
}

