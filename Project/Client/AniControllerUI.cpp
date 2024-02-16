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
        ImGui::OpenPopup("Delete?");
	}
    if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
        ImGui::Separator();

        //static int unused_i = 0;
        //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

        static bool dont_ask_me_next_time = false;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ImGui::PopStyleVar();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }


	return 0;
}


