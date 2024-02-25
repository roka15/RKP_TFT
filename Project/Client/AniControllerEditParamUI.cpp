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
	// Esc ������ ��Ȱ��ȭ
	if (KEY_TAP(KEY::ESC))
	{
		SetActive(false);

		// ��� UI ��Ŀ�� ����
		ImGui::SetWindowFocus(nullptr);
	}
}

int AniControllerEditParamUI::render_update()
{
    return S_OK;
}

