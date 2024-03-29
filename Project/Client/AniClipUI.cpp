#include "pch.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAniClip.h>
#include <Engine\CResMgr.h>
#include "AniClipUI.h"

AniClipUI::AniClipUI() :ResUI(RES_TYPE::ANICLIP),
m_curSliderFrame(0)
{
}

int AniClipUI::render_update()
{
	// Material ¿Ã∏ß
	ImGui::Text("Ani Clip");
	ImGui::SameLine();

	Ptr<CAniClip> pClip = (CAniClip*)GetTargetRes().Get();
	string strKey = string(pClip->GetKey().begin(), pClip->GetKey().end());
	ImGui::InputText("##MtrlUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	int Length = pClip->m_tInfo.iFrameLength;
	string Str = "Length    " + std::to_string(Length);
	ImGui::Text(Str.c_str());
	ImGui::Checkbox("Loop", &pClip->m_tInfo.bLoop);
	ImGui::Text("Events");
	int iFrameMin = 0;
	ImGui::SliderScalar("Frame", ImGuiDataType_U8, &m_curSliderFrame, &iFrameMin, &Length, "%u");
	if (ImGui::Button("Add Point", ImVec2(100, 30)))
	{
		t_AniEventPoint* point = new t_AniEventPoint();
		memset(point, 0, sizeof(t_AniEventPoint));
		point->Time = m_curSliderFrame;
		pClip->RegisterEvents(m_curSliderFrame, point);
		pClip->Save(pClip->GetRelativePath());
	}

	if (pClip->m_Events[m_curSliderFrame] == nullptr)
		return 0;
	char arrStrName[255] = {};
	string strFunc;
	ImGui::Text("Function");
	ImGui::SameLine();
	ImGui::InputText("##Frame Event Function", arrStrName,64);
	strFunc = arrStrName;
	wstring& wstrFunc = pClip->m_Events[m_curSliderFrame]->Function;
	wstrFunc.assign(strFunc.begin(), strFunc.end());
	

	ImGui::Text("Float");
	ImGui::SameLine();
	ImGui::DragFloat("##Frame Event Float", &pClip->m_Events[m_curSliderFrame]->Float);

	ImGui::Text("Int");
	ImGui::SameLine();
	ImGui::DragInt("##Frame Event Int", &pClip->m_Events[m_curSliderFrame]->Int);

	ImGui::Text("String");
	ImGui::SameLine();
	ImGui::InputText("##Frame Event String", (char*)pClip->m_Events[m_curSliderFrame]->String.data(), 64);




	/* ImGui::Text("")

	 ImGui::Text("Space Type (0:Object 1:Tangent)");
	 ImGui::DragInt("##UVMapping Type", (int*)&mappingData.bType);

	 ImGui::Text("Location");
	 ImGui::SameLine();
	 ImGui::DragFloat3("##UVMapping Location", mappingData.vLocation);

	 ImGui::Text("Rotation");
	 ImGui::SameLine();
	 ImGui::DragFloat3("##UVMapping Rotation", mappingData.vRotation);

	 ImGui::Text("Scale   ");
	 ImGui::SameLine();
	 ImGui::DragFloat3("##UVMapping Scale", mappingData.vScale);*/

	return 0;
}

AniClipUI::~AniClipUI()
{
}



