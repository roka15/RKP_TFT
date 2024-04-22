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
	wstring wstrKey = pController->GetKey();
	string strKey = string(pController->GetKey().begin(), pController->GetKey().end());
	ImGui::InputText("##AniControllerUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	return 0;
}


