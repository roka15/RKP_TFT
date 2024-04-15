#include "pch.h"
#include "BillbardUI.h"
#include <Engine\CBillboard.h>
#include <Engine\CResMgr.h>

int BillbardUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	char szBuff[50] = {};

	Ptr<CMesh> pMesh = GetTarget()->Billboard()->GetMesh();
	Ptr<CMaterial> pMtrl = GetTarget()->Billboard()->GetMaterial(0);

	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	GetResKey(pMesh.Get(), szBuff, 50);
	ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Material    ");
	ImGui::SameLine();
	GetResKey(pMtrl.Get(), szBuff, 50);
	ImGui::InputText("##MaterialName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
    return 0;
}

void BillbardUI::SelectMesh(DWORD_PTR _Key)
{
}

void BillbardUI::SelectMaterial(DWORD_PTR _Key)
{
}

BillbardUI::BillbardUI()
	: ComponentUI("##Billboard", COMPONENT_TYPE::BILLBOARD)
{
	SetName("Billboard");
}

BillbardUI::~BillbardUI()
{
}
