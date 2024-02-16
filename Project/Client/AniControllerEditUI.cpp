#include "pch.h"
#include "AniControllerEditUI.h"
#include "ImGuiMgr.h"
void AniControllerEditUI::init()
{
    imguiNodeEdit::Config config;
    config.SettingsFile = "Simple.json";
    m_Context = imguiNodeEdit::CreateEditor(&config);
}

void AniControllerEditUI::tick()
{
}

void AniControllerEditUI::finaltick()
{
    auto& io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();
    imguiNodeEdit::SetCurrentEditor(m_Context);
    imguiNodeEdit::Begin("My Editor", ImVec2(0.0, 0.0f));
    int uniqueId = 1;
    // Start drawing nodes.
    imguiNodeEdit::BeginNode(uniqueId++);
    ImGui::Text("Node A");
    imguiNodeEdit::BeginPin(uniqueId++, imguiNodeEdit::PinKind::Input);
    ImGui::Text("-> In");
    imguiNodeEdit::EndPin();
    ImGui::SameLine();
    imguiNodeEdit::BeginPin(uniqueId++, imguiNodeEdit::PinKind::Output);
    ImGui::Text("Out ->");
    imguiNodeEdit::EndPin();
    imguiNodeEdit::EndNode();
    imguiNodeEdit::End();
    imguiNodeEdit::SetCurrentEditor(nullptr);
}

int AniControllerEditUI::render_update()
{
    return 0;
}

AniControllerEditUI::AniControllerEditUI():UI("AniControllerEditUI")
{
}

AniControllerEditUI::~AniControllerEditUI()
{
}
