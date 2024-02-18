#include "pch.h"
#include "AniControllerUI.h"
#include <Engine\AnimatorController.h>

AniControllerUI::AniControllerUI()
	: ResUI(RES_TYPE::ANICONTROLLER)
{
	SetName("AniController");
}

AniControllerUI::~AniControllerUI()
{
}

int AniControllerUI::render_update()
{
	ResUI::render_update();
	ImGui::Text("AniController");
	ImGui::SameLine();
	Ptr<CAnimatorController> pController = (CAnimatorController*)GetTargetRes().Get();
	string strKey = string(pController->GetKey().begin(), pController->GetKey().end());
	ImGui::InputText("##AniControllerUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	
	if (ImGui::Button("Edit##AniControllerEditBtn", ImVec2(40, 30)))
	{
		//ImGuiMgr::GetInst()->FindUI("##AniControllerEditUI")->SetActive(true);
		ImGuiMgr::GetInst()->FindUI("##AniParameters")->SetActive(true);
	}
  
	return 0;
}


