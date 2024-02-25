#include "pch.h"
#include "AniControllerEditParamUI.h"
#include <Engine\CKeyMgr.h>

#include "TreeUI.h"

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
}

void AniControllerEditParamUI::finaltick()
{
	UI::finaltick();
	// Esc 눌리면 비활성화
	if (KEY_TAP(KEY::ESC))
	{
		SetActive(false);

		// 모든 UI 포커스 해제
		ImGui::SetWindowFocus(nullptr);
	}
}

int AniControllerEditParamUI::render_update()
{
    return S_OK;
}

