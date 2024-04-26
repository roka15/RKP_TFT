#include "pch.h"
#include "AniControllerEditParamUI.h"
#include "AniControllerEditUI.h"
#include <Engine\CKeyMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\AnimatorController.h>
#include <Engine\CAnimator3D.h>
#include "TreeUI.h"
#include "ListUI.h"


AniControllerEditParamUI::AniControllerEditParamUI() : UI("##AniParameters"),
m_bFlow(false),
m_ParamTypeNames{ "Int","Float","Bool","Trigger" }
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

	int selected_fish = -1;
	if (ImGui::Button("+ Add Param", ImVec2(100, 30)))
		ImGui::OpenPopup("## Ani Controller Add Param Type Popup");

	ImGui::SameLine();
	if (ImGui::BeginPopup("## Ani Controller Add Param Type Popup"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_ParamTypeNames); i++)
			if (ImGui::Selectable(m_ParamTypeNames[i]))
				selected_fish = i;
		ImGui::EndPopup();
	}

	switch (selected_fish)
	{
	case 0://int
		pController->RegisterParam(L"Int_Param" + std::to_wstring(m_intcnt), 0);
		pController->CopyParams(m_pLink);
		break;
	case 1://float
		pController->RegisterParam(L"Float_Param" + std::to_wstring(m_floatcnt), 0.f);
		pController->CopyParams(m_pLink);
		break;
	case 2://bool
		pController->RegisterParam(L"Bool_Param" + std::to_wstring(m_boolcnt), false,false);
		pController->CopyParams(m_pLink);
		break;
	case 3://trigger
		pController->RegisterParam(L"Trigger_Param" + std::to_wstring(m_triggercnt), false,true);
		pController->CopyParams(m_pLink);
		break;
	}
	ImGui::SameLine();
	string label = m_bFlow == true ? "> Show Flow [ON]" : "> Show Flow [OFF]";
	if (ImGui::Button(label.c_str(), ImVec2(150, 30)))
	{
		m_bFlow = !m_bFlow;
	}

	t_AniParams tParams = m_pLink->GetAniParamInfo();
	int i = 0;
	const map<wstring, int>& mapIntParam = tParams.mapIntParams;
	for (auto itr = mapIntParam.begin(); itr != mapIntParam.end(); ++itr)
	{
		wstring PrevParamName = itr->first;
		string  ParamName = std::string(PrevParamName.begin(), PrevParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(),MAXLEN);
		ImGui::SameLine();
		wstring NextParamName(ParamName.begin(), ParamName.end());
		label = "Int" + std::to_string(i);
		int iValue = itr->second;
		ImGui::InputInt(label.c_str(), &iValue);

		if (CKeyMgr::GetInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
		{
			if (PrevParamName != NextParamName)
			{
				pController->ChangeParamName(PARAM_TYPE::INT, PrevParamName, NextParamName);
				pController->CopyParams(m_pLink);
			}
		}
		
		m_pLink->SetIntParam(NextParamName, iValue);
		i++;
	}
	i = 0;
	const map<wstring, float>& mapFloatParam = tParams.mapFloatParams;
	for (auto itr = mapFloatParam.begin(); itr != mapFloatParam.end(); ++itr)
	{
		wstring PrevParamName = itr->first;
		string  ParamName = std::string(PrevParamName.begin(), PrevParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(), MAXLEN);
		ImGui::SameLine();
		wstring NextParamName(ParamName.begin(), ParamName.end());
		label = "Float" + std::to_string(i);;
		float fValue = itr->second;
		ImGui::InputFloat(label.c_str(), &fValue);

		if (CKeyMgr::GetInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
		{
			if (PrevParamName != NextParamName)
			{
				pController->ChangeParamName(PARAM_TYPE::FLOAT, PrevParamName, NextParamName);
				pController->CopyParams(m_pLink);
			}
		}
		m_pLink->SetFloatParam(NextParamName, fValue);
		i++;
	}

	i = 0;
	const map<wstring, bool>& mapBoolParam = tParams.mapBoolParams;
	for (auto itr = mapBoolParam.begin(); itr != mapBoolParam.end(); ++itr)
	{
		wstring PrevParamName = itr->first;
		string  ParamName = std::string(PrevParamName.begin(), PrevParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(), MAXLEN);
		ImGui::SameLine();
		wstring NextParamName(ParamName.begin(), ParamName.end());
	
		label = "Bool" + std::to_string(i);;
		bool bValue = itr->second;
	
		ImGui::Checkbox(label.c_str(), &bValue);
		if (CKeyMgr::GetInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
		{
			if (PrevParamName != NextParamName)
			{
				pController->ChangeParamName(PARAM_TYPE::BOOL, PrevParamName, NextParamName);
				pController->CopyParams(m_pLink);
			}
		}
		m_pLink->SetBoolParam(NextParamName, bValue);
		i++;
	}
	i = 0;
	const map<wstring, bool>& mapTriggerParam = tParams.mapTriggerParams;
	for (auto itr = mapTriggerParam.begin(); itr != mapTriggerParam.end(); ++itr)
	{
		wstring PrevParamName = itr->first;
		string  ParamName = std::string(PrevParamName.begin(), PrevParamName.end());
		string label = "##" + ParamName;
		ImGui::InputText(label.c_str(), (char*)ParamName.c_str(), MAXLEN);
		ImGui::SameLine();
		wstring NextParamName(ParamName.begin(), ParamName.end());
		label = "Trigger" + std::to_string(i);;
		bool bValue = itr->second;
		ImGui::Checkbox(label.c_str(), &bValue);
		if (CKeyMgr::GetInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
		{
			if (PrevParamName != NextParamName)
			{
				pController->ChangeParamName(PARAM_TYPE::TRIGGER, PrevParamName, NextParamName);
				pController->CopyParams(m_pLink);
			}
		}
		m_pLink->SetTriggerParam(NextParamName, bValue);
		i++;
	}

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.96f, 0.6f, 0.58f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.95f, 0.92f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.95f, 0.42f, 0.30f));
	if (ImGui::Button("Save Controller", ImVec2(120, 30)))
	{
		UI* pUI = ImGuiMgr::GetInst()->FindUI("##AniControllerEditUI");
		AniControllerEditUI* pEditUI = dynamic_cast<AniControllerEditUI*>(pUI);
		pEditUI->SaveNodeInfo();
		Ptr<CAnimatorController> pController = m_pLink->GetController();
		wstring Name = pController->GetKey();
		pController->Save(Name);
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();
    return S_OK;
}

void AniControllerEditParamUI::SetLink(CAnimator3D* _pAni)
{
	 m_pLink = _pAni; 
	 m_bFlow = false;
	 m_intcnt = 0;
	 m_floatcnt = 0;
	 m_boolcnt = 0;
	 m_triggercnt = 0;
}

