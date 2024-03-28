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
        t_AniEventPoint point = {};
        point.Time = m_curSliderFrame;
        pClip->RegisterEvents(point);
    }
    for (int i = 0; i < pClip->m_Events.size(); ++i)
    {
        if (m_curSliderFrame == pClip->m_Events[i].Time)
        {
            ImGui::Text("Float");
            ImGui::SameLine();
            ImGui::DragFloat("##Frame Event Float",&pClip->m_Events[i].Float);

            ImGui::Text("Int");
            ImGui::SameLine();
            ImGui::DragInt("##Frame Event Int", &pClip->m_Events[i].Int);

            ImGui::Text("String");
            ImGui::SameLine();
            ImGui::InputText("64 ch",(char*)pClip->m_Events[i].String.data(), 64);


        }
    }
   
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



