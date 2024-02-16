#include "pch.h"
#include "AniControllerUI.h"
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
	/*ImGui::Text("AniController");
	ImGui::SameLine();*/

	return 0;
}


