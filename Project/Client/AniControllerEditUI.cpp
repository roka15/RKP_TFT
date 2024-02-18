#include "pch.h"
#include "AniControllerEditUI.h"
#include "ImGuiMgr.h"
#include "ImGui\imgui_internal.h"
#include <Engine\CKeyMgr.h>
AniControllerEditUI::AniControllerEditUI() :UI("##AniControllerEditUI")
{
	SetName("Animator");
}

AniControllerEditUI::~AniControllerEditUI()
{
}
bool AniControllerEditUI::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	using namespace ImGui;
	ImGuiContext* g = ImGuiMgr::GetInst()->GetGuiContext();
	ImGuiWindow* window = g->CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}
void AniControllerEditUI::ShowLeftPane(float paneWidth)
{
	auto& io = ImGui::GetIO();

	ImGui::BeginChild("Selection", ImVec2(paneWidth, 0));

	paneWidth = ImGui::GetContentRegionAvail().x;

	static bool showStyleEditor = false;
	ImGui::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));
	ImGui::Spring(0.0f, 0.0f);
	if (ImGui::Button("Zoom to Content"))
		imguiNodeEdit::NavigateToContent();
	ImGui::Spring(0.0f);
	if (ImGui::Button("Show Flow"))
	{
		for (auto& link : m_Links)
			imguiNodeEdit::Flow(link.ID);
	}
	ImGui::Spring();
	if (ImGui::Button("Edit Style"))
		showStyleEditor = true;
	ImGui::EndHorizontal();
	ImGui::Checkbox("Show Ordinals", &m_ShowOrdinals);

	if (showStyleEditor)
		ShowStyleEditor(&showStyleEditor);

	std::vector<imguiNodeEdit::NodeId> selectedNodes;
	std::vector<imguiNodeEdit::LinkId> selectedLinks;
	selectedNodes.resize(imguiNodeEdit::GetSelectedObjectCount());
	selectedLinks.resize(imguiNodeEdit::GetSelectedObjectCount());

	int nodeCount = imguiNodeEdit::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
	int linkCount = imguiNodeEdit::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

	selectedNodes.resize(nodeCount);
	selectedLinks.resize(linkCount);

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
		ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::TextUnformatted("Nodes");
	ImGui::Indent();
	for (auto& node : m_Nodes)
	{
		ImGui::PushID(node.ID.AsPointer());
		auto start = ImGui::GetCursorScreenPos();

		if (const auto progress = GetTouchProgress(node.ID))
		{
			ImGui::GetWindowDrawList()->AddLine(
				start + ImVec2(-8, 0),
				start + ImVec2(-8, ImGui::GetTextLineHeight()),
				IM_COL32(255, 0, 0, 255 - (int)(255 * progress)), 4.0f);
		}

		bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node.ID) != selectedNodes.end();
# if IMGUI_VERSION_NUM >= 18967
		ImGui::SetNextItemAllowOverlap();
# endif
		if (ImGui::Selectable((node.Name + "##" + std::to_string(reinterpret_cast<uintptr_t>(node.ID.AsPointer()))).c_str(), &isSelected))
		{
			if (io.KeyCtrl)
			{
				if (isSelected)
					imguiNodeEdit::SelectNode(node.ID, true);
				else
					imguiNodeEdit::DeselectNode(node.ID);
			}
			else
				imguiNodeEdit::SelectNode(node.ID, false);

			imguiNodeEdit::NavigateToSelection();
		}
		if (ImGui::IsItemHovered() && !node.State.empty())
			ImGui::SetTooltip("State: %s", node.State.c_str());

		auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node.ID.AsPointer())) + ")";
		auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
		auto drawList = ImGui::GetWindowDrawList();

# if IMGUI_VERSION_NUM < 18967
		ImGui::SetItemAllowOverlap();
# else
		ImGui::SetNextItemAllowOverlap();
# endif
		if (node.SavedState.empty())
		{
			if (ImGui::IsItemActive())
				drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
			else if (ImGui::IsItemHovered())
				drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			else
				drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
		}
		else
		{
			drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
		}

		ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
# if IMGUI_VERSION_NUM < 18967
		ImGui::SetItemAllowOverlap();
# else
		ImGui::SetNextItemAllowOverlap();
# endif
		if (!node.SavedState.empty())
		{
			if (ImGui::IsItemActive())
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
			else if (ImGui::IsItemHovered())
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			else
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
		}
		else
		{
			drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
		}

		ImGui::SameLine(0, 0);
# if IMGUI_VERSION_NUM < 18967
		ImGui::SetItemAllowOverlap();
# endif

		ImGui::PopID();
	}
	ImGui::Unindent();

	static int changeCount = 0;

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
		ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::TextUnformatted("Selection");

	ImGui::BeginHorizontal("Selection Stats", ImVec2(paneWidth, 0));
	ImGui::Text("Changed %d time%s", changeCount, changeCount > 1 ? "s" : "");
	ImGui::Spring();
	if (ImGui::Button("Deselect All"))
		imguiNodeEdit::ClearSelection();
	ImGui::EndHorizontal();
	ImGui::Indent();
	for (int i = 0; i < nodeCount; ++i) ImGui::Text("Node (%p)", selectedNodes[i].AsPointer());
	for (int i = 0; i < linkCount; ++i) ImGui::Text("Link (%p)", selectedLinks[i].AsPointer());
	ImGui::Unindent();

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
		for (auto& link : m_Links)
			imguiNodeEdit::Flow(link.ID);

	if (imguiNodeEdit::HasSelectionChanged())
		++changeCount;

	ImGui::EndChild();
}
void AniControllerEditUI::ShowStyleEditor(bool* show)
{
	if (!ImGui::Begin("Style", show))
	{
		ImGui::End();
		return;
	}

	auto paneWidth = ImGui::GetContentRegionAvail().x;

	auto& editorStyle = imguiNodeEdit::GetStyle();
	ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
	ImGui::TextUnformatted("Values");
	ImGui::Spring();
	if (ImGui::Button("Reset to defaults"))
		editorStyle = imguiNodeEdit::Style();
	ImGui::EndHorizontal();
	ImGui::Spacing();
	ImGui::DragFloat4("Node Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Node Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Node Border Width", &editorStyle.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Hovered Node Border Width", &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Hovered Node Border Offset", &editorStyle.HoverNodeBorderOffset, 0.1f, -40.0f, 40.0f);
	ImGui::DragFloat("Selected Node Border Width", &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Selected Node Border Offset", &editorStyle.SelectedNodeBorderOffset, 0.1f, -40.0f, 40.0f);
	ImGui::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f, 500.0f);
	//ImVec2  SourceDirection;
	//ImVec2  TargetDirection;
	ImGui::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
	ImGui::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f, 2000.0f);
	ImGui::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f, 5.0f);
	//ImVec2  PivotAlignment;
	//ImVec2  PivotSize;
	//ImVec2  PivotScale;
	//float   PinCorners;
	//float   PinRadius;
	//float   PinArrowSize;
	//float   PinArrowWidth;
	ImGui::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth, 0.1f, 0.0f, 15.0f);

	ImGui::Separator();

	static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_DisplayRGB;
	ImGui::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
	ImGui::TextUnformatted("Filter Colors");
	ImGui::Spring();
	ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_DisplayRGB);
	ImGui::Spring(0);
	ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_DisplayHSV);
	ImGui::Spring(0);
	ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_DisplayHex);
	ImGui::EndHorizontal();

	static ImGuiTextFilter filter;
	filter.Draw("##filter", paneWidth);

	ImGui::Spacing();

	ImGui::PushItemWidth(-160);
	for (int i = 0; i < imguiNodeEdit::StyleColor_Count; ++i)
	{
		auto name = imguiNodeEdit::GetStyleColorName((imguiNodeEdit::StyleColor)i);
		if (!filter.PassFilter(name))
			continue;

		ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
	}
	ImGui::PopItemWidth();

	ImGui::End();
}

float AniControllerEditUI::GetTouchProgress(imguiNodeEdit::NodeId id)
{
	auto it = m_NodeTouchTime.find(id);
	if (it != m_NodeTouchTime.end() && it->second > 0.0f)
		return (m_TouchTime - it->second) / m_TouchTime;
	else
		return 0.0f;
}

void AniControllerEditUI::init()
{
	//SetSize(1000, 1000);
	imguiNodeEdit::Config config;
	config.SettingsFile = "Simple.json";
	m_Context = imguiNodeEdit::CreateEditor(&config);
}

void AniControllerEditUI::tick()
{
}

void AniControllerEditUI::finaltick()
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

int AniControllerEditUI::render_update()
{
	auto& io = ImGui::GetIO();
	ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
	imguiNodeEdit::SetCurrentEditor(m_Context);
	imguiNodeEdit::Begin("My Editor", ImVec2(0.0, 0.0f));
	int uniqueId = 1;

	//static float leftPaneWidth = 400.0f;
	//static float rightPaneWidth = 800.0f;
	//Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
	//ShowLeftPane(leftPaneWidth - 4.0f);
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
	return 0;
}


