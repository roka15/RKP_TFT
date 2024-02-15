#include "pch.h"
#include "Animator3DUI.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnimation3D.h>
#include <Engine\CResMgr.h>

#include "AniClipUI.h"
#include "ListUI.h"
#include "TreeUI.h"
Animator3DUI::Animator3DUI() 
	: ComponentUI("##Animator3D", COMPONENT_TYPE::ANIMATOR3D)
{
	SetName("Animator3D");
}

Animator3DUI::~Animator3DUI()
{
}
void Animator3DUI::SelectClip(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CAniClip> pClip = CResMgr::GetInst()->FindRes<CAniClip>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->Animator3D()->ChangeAniClip(pClip->GetName());
}
int Animator3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	char szBuff[MAXLEN] = {};

	CAnimation3D* pAni = GetTarget()->Animator3D()->GetAnimation();
	Ptr<CAniClip> pClip;
	if (pAni == nullptr)
	{
		pClip = nullptr;
	}
	else
	{
		pClip = pAni->GetClip();
		GetResKey(pClip.Get(), szBuff, MAXLEN);
	}
	ImGui::Text("Clip    ");
	ImGui::SameLine();
	ImGui::InputText("##ClipName", szBuff, MAXLEN, ImGuiInputTextFlags_ReadOnly);

	
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
		if (pPayLoad)
		{
			TreeNode* pNode = (TreeNode*)pPayLoad->Data;
			CRes* pRes = (CRes*)pNode->GetData();
			if (RES_TYPE::ANICLIP == pRes->GetType())
			{
				GetTarget()->Animator3D()->ChangeAniClip(pRes->GetName());
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();

	if (ImGui::Button("##ClipSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapClip = CResMgr::GetInst()->GetResources(RES_TYPE::ANICLIP);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Clip List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapClip)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator3DUI::SelectClip);
	}
	return TRUE;
}