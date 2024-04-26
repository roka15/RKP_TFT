#include "pch.h"
#include "AniControllerEditUI.h"
#include "AniControllerInspector.h"
#include "AniControllerEditParamUI.h"
#include "ImGuiMgr.h"
#include "ImGui\imgui_internal.h"
#include <Engine\CKeyMgr.h>
#include <Engine\CAnimator3D.h>
#include <Engine\AnimatorController.h>
#include <Engine\AniNode.h>
#include <Engine\Transition.h>
#include <Engine\CPathMgr.h>

int ClientNode::Index = 0;

AniControllerEditUI::AniControllerEditUI() :UI("##AniControllerEditUI")
{
	SetName("Animator");
}

AniControllerEditUI::~AniControllerEditUI()
{
}
client_ed::LinkId AniControllerEditUI::GetNextLinkId()
{
	return client_ed::LinkId(GetNextId());
}

void AniControllerEditUI::TouchNode(client_ed::NodeId id)
{

	m_NodeTouchTime[id] = m_TouchTime;

}

float AniControllerEditUI::GetTouchProgress(client_ed::NodeId id)
{
	auto it = m_NodeTouchTime.find(id);
	if (it != m_NodeTouchTime.end() && it->second > 0.0f)
		return (m_TouchTime - it->second) / m_TouchTime;
	else
		return 0.0f;
}

void AniControllerEditUI::UpdateTouch()
{
	const auto deltaTime = ImGui::GetIO().DeltaTime;
	for (auto& entry : m_NodeTouchTime)
	{
		if (entry.second > 0.0f)
			entry.second -= deltaTime;
	}
}

ClientNode* AniControllerEditUI::FindNode(client_ed::NodeId id)
{
	for (auto& node : m_Nodes)
		if (node.ID == id)
			return &node;

	return nullptr;
}

Link* AniControllerEditUI::FindLink(client_ed::LinkId id)
{
	for (auto& link : m_Links)
		if (link.ID == id)
			return &link;

	return nullptr;
}

ClientPin* AniControllerEditUI::FindPin(client_ed::PinId id)
{
	if (!id)
		return nullptr;

	for (auto& node : m_Nodes)
	{
		for (auto& pin : node.Inputs)
			if (pin.ID == id)
				return &pin;

		for (auto& pin : node.Outputs)
			if (pin.ID == id)
				return &pin;
	}

	return nullptr;
}

bool AniControllerEditUI::IsPinLinked(client_ed::PinId id)
{
	{
		if (!id)
			return false;

		for (auto& link : m_Links)
			if (link.StartPinID == id || link.EndPinID == id)
				return true;

		return false;
	}
}

bool AniControllerEditUI::CanCreateLink(ClientPin* a, ClientPin* b)
{

	if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
		return false;

	return true;

}

void AniControllerEditUI::BuildNode(ClientNode* node)
{
	for (auto& input : node->Inputs)
	{
		input.Node = node;
		input.Kind = PinKind::Input;
	}

	for (auto& output : node->Outputs)
	{
		output.Node = node;
		output.Kind = PinKind::Output;
	}
}

ClientNode* AniControllerEditUI::SpawnTreeSequenceNode()
{
	int iGetNextID = GetNextId();
	string Name = "New State " + std::to_string(ClientNode::Index);
	m_Nodes.emplace_back(iGetNextID, Name.c_str());
	m_Nodes.back().Type = NodeType::Tree;
	m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
	m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);
	m_Nodes.back().iID = iGetNextID;

	BuildNode(&m_Nodes.back());
	++ClientNode::Index;

	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	wstring wName(Name.begin(), Name.end());
	pController->CreateNode(wName, L"", iGetNextID);
	return &m_Nodes.back();
}

ClientNode* AniControllerEditUI::SpawnTreeSequenceEmptyNode()
{
	int iGetNextID = GetNextId();
	string Name = "New State " + std::to_string(ClientNode::Index);
	m_Nodes.emplace_back(iGetNextID, Name.c_str());
	m_Nodes.back().Type = NodeType::Tree;
	m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
	m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);
	m_Nodes.back().iID = iGetNextID;

	BuildNode(&m_Nodes.back());
	++ClientNode::Index;

	return &m_Nodes.back();
}

Link* AniControllerEditUI::CreateLinkAndRegister(client_ed::PinId _start, client_ed::PinId _end)
{
	int iID = GetNextId();
	ClientPin* StartPin = FindPin(_start);
	m_Links.emplace_back(Link(iID, _start, _end));
	m_Links.back().Color = GetIconColor(StartPin->Type);
	m_Links.back().iID = iID;
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	ClientNode* StartNode = FindPin(_start)->Node;
	int StartID = StartNode->iID;
	ClientNode* EndNode = FindPin(_end)->Node;
	int EndID = EndNode->iID;
	CTransition* pTransition = pController->GetTransition(StartID, EndID);
	pTransition->SetEditID(iID);
	pController->RegisterIDTransition(iID, pTransition);
	return &m_Links.back();
}

Link* AniControllerEditUI::CreateLinkAndCreate(client_ed::PinId _start, client_ed::PinId _end)
{
	int iID = GetNextId();
	ClientPin* StartPin = FindPin(_start);
	m_Links.emplace_back(Link(iID, _start, _end));
	m_Links.back().Color = GetIconColor(StartPin->Type);
	m_Links.back().iID = iID;
	//transition 정보 생성
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	ClientNode* StartNode = FindPin(_start)->Node;
	int StartID = StartNode->iID;
	ClientNode* EndNode = FindPin(_end)->Node;
	int EndID = EndNode->iID;
	pController->CreateTransition(StartID, EndID, iID);
	return &m_Links.back();
}

void AniControllerEditUI::DeleteSequenceNode(client_ed::NodeId _id)
{
	ClientNode* Node = FindNode(_id);
	if (Node->Type == NodeType::Tree)
	{
		--ClientNode::Index;
		Ptr<CAnimatorController> pController = m_pAnimator->GetController();
		pController->DestroyNode(Node->iID);
	}
}

void AniControllerEditUI::DeleteLink(client_ed::LinkId _id)
{
	Link* pLink = FindLink(_id);
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	pController->DestroyTransition(pLink->iID);
}

void AniControllerEditUI::ReCreateFontAtlas()
{
	ImGuiIO& io = ImGui::GetIO();

	IM_DELETE(io.Fonts);

	io.Fonts = IM_NEW(ImFontAtlas);

	ImFontConfig config;
	config.OversampleH = 4;
	config.OversampleV = 4;
	config.PixelSnapH = false;

	m_DefaultFont = io.Fonts->AddFontFromFileTTF("..\\..\\OutputFile\\bin\\content\\node_editor_data\\Play-Regular.ttf", 18.0f, &config);
	m_HeaderFont = io.Fonts->AddFontFromFileTTF("..\\..\\OutputFile\\bin\\content\\node_editor_data\\Cuprum-Bold.ttf", 20.0f, &config);

	io.Fonts->Build();
}

void AniControllerEditUI::BuildNodes()
{
	for (auto& node : m_Nodes)
		BuildNode(&node);
}

void AniControllerEditUI::OnStart()
{
	ReCreateFontAtlas();
	client_ed::Config config;

	config.SettingsFile = "Blueprints.json";

	config.UserPointer = this;

	config.LoadNodeSettings = [](client_ed::NodeId nodeId, char* data, void* userPointer) -> size_t
	{
		auto self = static_cast<AniControllerEditUI*>(userPointer);

		auto node = self->FindNode(nodeId);
		if (!node)
			return 0;

		if (data != nullptr)
			memcpy(data, node->State.data(), node->State.size());
		return node->State.size();
	};

	config.SaveNodeSettings = [](client_ed::NodeId nodeId, const char* data, size_t size, client_ed::SaveReasonFlags reason, void* userPointer) -> bool
	{
		auto self = static_cast<AniControllerEditUI*>(userPointer);

		auto node = self->FindNode(nodeId);
		if (!node)
			return false;

		node->State.assign(data, size);

		self->TouchNode(nodeId);

		return true;
	};

	m_Editor = client_ed::CreateEditor(&config);
	client_ed::SetCurrentEditor(m_Editor);

	ClientNode* node;

	/*node = SpawnTreeSequenceNode();     client_ed::SetNodePosition(node->ID, ImVec2(1028, 329));
	node = SpawnTreeTaskNode();         client_ed::SetNodePosition(node->ID, ImVec2(1204, 458));
	node = SpawnTreeTask2Node();        client_ed::SetNodePosition(node->ID, ImVec2(868, 538));

	node = SpawnComment();              client_ed::SetNodePosition(node->ID, ImVec2(112, 576)); client_ed::SetGroupSize(node->ID, ImVec2(384, 154));
	node = SpawnComment();              client_ed::SetNodePosition(node->ID, ImVec2(800, 224)); client_ed::SetGroupSize(node->ID, ImVec2(640, 400));*/

	client_ed::NavigateToContent();

	BuildNodes();
}

void AniControllerEditUI::OnStop()
{
	auto releaseTexture = [this](ImTextureID& id)
	{
		if (id)
		{
			ImGui_DestroyTexture(id);
			id = nullptr;
		}
	};

	releaseTexture(m_RestoreIcon);
	releaseTexture(m_SaveIcon);
	releaseTexture(m_HeaderBackground);

	if (m_Editor)
	{
		client_ed::DestroyEditor(m_Editor);
		m_Editor = nullptr;
	}
}

ImColor AniControllerEditUI::GetIconColor(PinType type)
{
	{
		switch (type)
		{
		default:
		case PinType::Flow:     return ImColor(255, 255, 255);
		case PinType::Bool:     return ImColor(220, 48, 48);
		case PinType::Int:      return ImColor(68, 201, 156);
		case PinType::Float:    return ImColor(147, 226, 74);
		case PinType::String:   return ImColor(124, 21, 153);
		case PinType::Object:   return ImColor(51, 150, 215);
		case PinType::Function: return ImColor(218, 0, 183);
		case PinType::Delegate: return ImColor(255, 48, 48);
		}
	};
}

void AniControllerEditUI::DrawPinIcon(const ClientPin& pin, bool connected, int alpha)
{
	{
		IconType iconType;
		ImColor  color = GetIconColor(pin.Type);
		color.Value.w = alpha / 255.0f;
		switch (pin.Type)
		{
		case PinType::Flow:     iconType = IconType::Flow;   break;
		case PinType::Bool:     iconType = IconType::Circle; break;
		case PinType::Int:      iconType = IconType::Circle; break;
		case PinType::Float:    iconType = IconType::Circle; break;
		case PinType::String:   iconType = IconType::Circle; break;
		case PinType::Object:   iconType = IconType::Circle; break;
		case PinType::Function: iconType = IconType::Circle; break;
		case PinType::Delegate: iconType = IconType::Square; break;
		default:
			return;
		}

		ax::Widgets::Icon(ImVec2(static_cast<float>(m_PinIconSize), static_cast<float>(m_PinIconSize)), iconType, connected, color, ImColor(32, 32, 32, alpha));
	};
}

void AniControllerEditUI::ShowStyleEditor(bool* show)
{
	{
		if (!ImGui::Begin("Style", show))
		{
			ImGui::End();
			return;
		}

		auto paneWidth = ImGui::GetContentRegionAvail().x;

		auto& editorStyle = client_ed::GetStyle();
		ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
		ImGui::TextUnformatted("Values");
		ImGui::Spring();
		if (ImGui::Button("Reset to defaults"))
			editorStyle = client_ed::Style();
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
		for (int i = 0; i < client_ed::StyleColor_Count; ++i)
		{
			auto name = client_ed::GetStyleColorName((client_ed::StyleColor)i);
			if (!filter.PassFilter(name))
				continue;

			ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
		}
		ImGui::PopItemWidth();

		ImGui::End();
	}

}

void AniControllerEditUI::ShowLeftPane(float paneWidth)
{
	{
		auto& io = ImGui::GetIO();

		ImGui::BeginChild("Selection", ImVec2(paneWidth, 0));

		paneWidth = ImGui::GetContentRegionAvail().x;

		static bool showStyleEditor = false;
		ImGui::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));
		ImGui::Spring(0.0f, 0.0f);
		if (ImGui::Button("Zoom to Content"))
			client_ed::NavigateToContent();
		ImGui::Spring(0.0f);
		if (ImGui::Button("Show Flow"))
		{
			for (auto& link : m_Links)
				client_ed::Flow(link.ID);
		}
		ImGui::Spring();
		if (ImGui::Button("Edit Style"))
			showStyleEditor = true;
		ImGui::EndHorizontal();
		ImGui::Checkbox("Show Ordinals", &m_ShowOrdinals);

		if (showStyleEditor)
			ShowStyleEditor(&showStyleEditor);

		std::vector<client_ed::NodeId> selectedNodes;
		std::vector<client_ed::LinkId> selectedLinks;
		selectedNodes.resize(client_ed::GetSelectedObjectCount());
		selectedLinks.resize(client_ed::GetSelectedObjectCount());

		int nodeCount = client_ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
		int linkCount = client_ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

		selectedNodes.resize(nodeCount);
		selectedLinks.resize(linkCount);


		int saveIconWidth = ImGuiMgr::GetInst()->GetTextureWidth(m_SaveIcon);
		int saveIconHeight = ImGuiMgr::GetInst()->GetTextureHeight(m_SaveIcon);
		int restoreIconWidth = ImGuiMgr::GetInst()->GetTextureWidth(m_RestoreIcon);
		int restoreIconHeight = ImGuiMgr::GetInst()->GetTextureHeight(m_RestoreIcon);

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
						client_ed::SelectNode(node.ID, true);
					else
						client_ed::DeselectNode(node.ID);
				}
				else
					client_ed::SelectNode(node.ID, false);

				client_ed::NavigateToSelection();
			}
			if (ImGui::IsItemHovered() && !node.State.empty())
				ImGui::SetTooltip("State: %s", node.State.c_str());

			auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node.ID.AsPointer())) + ")";
			auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
			auto iconPanelPos = start + ImVec2(
				paneWidth - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing - saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
				(ImGui::GetTextLineHeight() - saveIconHeight) / 2);
			ImGui::GetWindowDrawList()->AddText(
				ImVec2(iconPanelPos.x - textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
				IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);

			auto drawList = ImGui::GetWindowDrawList();
			ImGui::SetCursorScreenPos(iconPanelPos);
# if IMGUI_VERSION_NUM < 18967
			ImGui::SetItemAllowOverlap();
# else
			ImGui::SetNextItemAllowOverlap();
# endif
			if (node.SavedState.empty())
			{
				if (ImGui::InvisibleButton("save", ImVec2((float)saveIconWidth, (float)saveIconHeight)))
					node.SavedState = node.State;

				if (ImGui::IsItemActive())
					drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
				else if (ImGui::IsItemHovered())
					drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
				else
					drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
			}
			else
			{
				ImGui::Dummy(ImVec2((float)saveIconWidth, (float)saveIconHeight));
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
				if (ImGui::InvisibleButton("restore", ImVec2((float)restoreIconWidth, (float)restoreIconHeight)))
				{
					node.State = node.SavedState;
					client_ed::RestoreNodeState(node.ID);
					node.SavedState.clear();
				}

				if (ImGui::IsItemActive())
					drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
				else if (ImGui::IsItemHovered())
					drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
				else
					drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
			}
			else
			{
				ImGui::Dummy(ImVec2((float)restoreIconWidth, (float)restoreIconHeight));
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
			}

			ImGui::SameLine(0, 0);
# if IMGUI_VERSION_NUM < 18967
			ImGui::SetItemAllowOverlap();
# endif
			ImGui::Dummy(ImVec2(0, (float)restoreIconHeight));

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
			client_ed::ClearSelection();
		ImGui::EndHorizontal();
		ImGui::Indent();
		for (int i = 0; i < nodeCount; ++i) ImGui::Text("Node (%p)", selectedNodes[i].AsPointer());
		for (int i = 0; i < linkCount; ++i) ImGui::Text("Link (%p)", selectedLinks[i].AsPointer());
		ImGui::Unindent();

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
			for (auto& link : m_Links)
				client_ed::Flow(link.ID);

		if (client_ed::HasSelectionChanged())
			++changeCount;

		ImGui::EndChild();
	}
}

void AniControllerEditUI::OnFrame(float deltaTime)
{
	UpdateTouch();

	auto& io = ImGui::GetIO();

	client_ed::SetCurrentEditor(m_Editor);

	auto& style = ImGui::GetStyle();

# if 0
	{
		for (auto x = -io.DisplaySize.y; x < io.DisplaySize.x; x += 10.0f)
		{
			ImGui::GetWindowDrawList()->AddLine(ImVec2(x, 0), ImVec2(x + io.DisplaySize.y, io.DisplaySize.y),
				IM_COL32(255, 255, 0, 255));
		}
	}
# endif

	static client_ed::NodeId contextNodeId = 0;
	static client_ed::LinkId contextLinkId = 0;
	static client_ed::PinId  contextPinId = 0;
	static bool createNewNode = false;
	static ClientPin* newNodeLinkPin = nullptr;
	static ClientPin* newLinkPin = nullptr;

	ImGui::SameLine(0.0f, 12.0f);

	client_ed::Begin("Node editor");
	{
		auto cursorTopLeft = ImGui::GetCursorScreenPos();

		for (auto& node : m_Nodes)
		{
			if (node.Type != NodeType::Tree)
				continue;

			const float rounding = 5.0f;
			const float padding = 12.0f;

			const auto pinBackground = client_ed::GetStyle().Colors[client_ed::StyleColor_NodeBg];

			client_ed::PushStyleColor(client_ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
			client_ed::PushStyleColor(client_ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
			client_ed::PushStyleColor(client_ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
			client_ed::PushStyleColor(client_ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

			client_ed::PushStyleVar(client_ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
			client_ed::PushStyleVar(client_ed::StyleVar_NodeRounding, rounding);
			client_ed::PushStyleVar(client_ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
			client_ed::PushStyleVar(client_ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
			client_ed::PushStyleVar(client_ed::StyleVar_LinkStrength, 0.0f);
			client_ed::PushStyleVar(client_ed::StyleVar_PinBorderWidth, 1.0f);
			client_ed::PushStyleVar(client_ed::StyleVar_PinRadius, 5.0f);
			client_ed::BeginNode(node.ID);

			ImGui::BeginVertical(node.ID.AsPointer());
			ImGui::BeginHorizontal("inputs");
			ImGui::Spring(0, padding * 2);

			ImRect inputsRect;
			int inputAlpha = 200;
			if (!node.Inputs.empty())
			{
				auto& pin = node.Inputs[0];
				ImGui::Dummy(ImVec2(0, padding));
				ImGui::Spring(1, 0);
				inputsRect = ImGui_GetItemRect();
				client_ed::PushStyleVar(client_ed::StyleVar_PinArrowSize, 10.0f);
				client_ed::PushStyleVar(client_ed::StyleVar_PinArrowWidth, 10.0f);
#if IMGUI_VERSION_NUM > 18101
				client_ed::PushStyleVar(client_ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersBottom);
#else
				client_ed::PushStyleVar(client_ed::StyleVar_PinCorners, 12);
#endif
				client_ed::BeginPin(pin.ID, client_ed::PinKind::Input);
				client_ed::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
				client_ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
				client_ed::EndPin();
				client_ed::PopStyleVar(3);

				if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
					inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
			}
			else
				ImGui::Dummy(ImVec2(0, padding));

			ImGui::Spring(0, padding * 2);
			ImGui::EndHorizontal();

			ImGui::BeginHorizontal("content_frame");
			ImGui::Spring(1, padding);

			ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
			ImGui::Dummy(ImVec2(160, 0));
			ImGui::Spring(1);
			ImGui::TextUnformatted(node.Name.c_str());
			ImGui::Spring(1);
			ImGui::EndVertical();
			auto contentRect = ImGui_GetItemRect();
			ImGui::Spring(1, padding);
			ImGui::EndHorizontal();

			ImGui::BeginHorizontal("outputs");
			ImGui::Spring(0, padding * 2);

			ImRect outputsRect;
			int outputAlpha = 200;
			if (!node.Outputs.empty())
			{
				auto& pin = node.Outputs[0];
				ImGui::Dummy(ImVec2(0, padding));
				ImGui::Spring(1, 0);
				outputsRect = ImGui_GetItemRect();
#if IMGUI_VERSION_NUM > 18101
				client_ed::PushStyleVar(client_ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersTop);
#else
				client_ed::PushStyleVar(client_ed::StyleVar_PinCorners, 3);
#endif
				client_ed::BeginPin(pin.ID, client_ed::PinKind::Output);
				client_ed::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
				client_ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
				client_ed::EndPin();
				client_ed::PopStyleVar();

				if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
					outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
			}
			else
				ImGui::Dummy(ImVec2(0, padding));

			ImGui::Spring(0, padding * 2);
			ImGui::EndHorizontal();

			ImGui::EndVertical();

			client_ed::EndNode();
			client_ed::PopStyleVar(7);
			client_ed::PopStyleColor(4);

			auto drawList = client_ed::GetNodeBackgroundDrawList(node.ID);

			//const auto fringeScale = ImGui::GetStyle().AntiAliasFringeScale;
			//const auto unitSize    = 1.0f / fringeScale;

			//const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
			//{
			//    if ((col >> 24) == 0)
			//        return;
			//    drawList->PathRect(a, b, rounding, rounding_corners);
			//    drawList->PathStroke(col, true, thickness);
			//};

#if IMGUI_VERSION_NUM > 18101
			const auto    topRoundCornersFlags = ImDrawFlags_RoundCornersTop;
			const auto bottomRoundCornersFlags = ImDrawFlags_RoundCornersBottom;
#else
			const auto    topRoundCornersFlags = 1 | 2;
			const auto bottomRoundCornersFlags = 4 | 8;
#endif

			drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
			//ImGui::PopStyleVar();
			drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
			//ImGui::PopStyleVar();
			drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			drawList->AddRect(
				contentRect.GetTL(),
				contentRect.GetBR(),
				IM_COL32(48, 128, 255, 100), 0.0f);
			//ImGui::PopStyleVar();
		}




		for (auto& node : m_Nodes)
		{
			if (node.Type != NodeType::Comment)
				continue;

			const float commentAlpha = 0.75f;

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
			client_ed::PushStyleColor(client_ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
			client_ed::PushStyleColor(client_ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
			client_ed::BeginNode(node.ID);
			ImGui::PushID(node.ID.AsPointer());
			ImGui::BeginVertical("content");
			ImGui::BeginHorizontal("horizontal");
			ImGui::Spring(1);
			ImGui::TextUnformatted(node.Name.c_str());
			ImGui::Spring(1);
			ImGui::EndHorizontal();
			client_ed::Group(node.Size);
			ImGui::EndVertical();
			ImGui::PopID();
			client_ed::EndNode();
			client_ed::PopStyleColor(2);
			ImGui::PopStyleVar();

			if (client_ed::BeginGroupHint(node.ID))
			{
				//auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
				auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

				//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

				auto min = client_ed::GetGroupMin();
				//auto max = client_ed::GetGroupMax();

				ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
				ImGui::BeginGroup();
				ImGui::TextUnformatted(node.Name.c_str());
				ImGui::EndGroup();

				auto drawList = client_ed::GetHintBackgroundDrawList();

				auto hintBounds = ImGui_GetItemRect();
				auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

				drawList->AddRectFilled(
					hintFrameBounds.GetTL(),
					hintFrameBounds.GetBR(),
					IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f);

				drawList->AddRect(
					hintFrameBounds.GetTL(),
					hintFrameBounds.GetBR(),
					IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f);

				//ImGui::PopStyleVar();
			}
			client_ed::EndGroupHint();
		}

		for (auto& link : m_Links)
		{
			client_ed::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);
		}

		if (!createNewNode)
		{
			if (client_ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
			{
				auto showLabel = [](const char* label, ImColor color)
				{
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
					auto size = ImGui::CalcTextSize(label);

					auto padding = ImGui::GetStyle().FramePadding;
					auto spacing = ImGui::GetStyle().ItemSpacing;

					ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

					auto rectMin = ImGui::GetCursorScreenPos() - padding;
					auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

					auto drawList = ImGui::GetWindowDrawList();
					drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
					ImGui::TextUnformatted(label);
				};

				client_ed::PinId startPinId = 0, endPinId = 0;
				if (client_ed::QueryNewLink(&startPinId, &endPinId))
				{
					auto startPin = FindPin(startPinId);
					auto endPin = FindPin(endPinId);

					newLinkPin = startPin ? startPin : endPin;

					if (startPin->Kind == PinKind::Input)
					{
						std::swap(startPin, endPin);
						std::swap(startPinId, endPinId);
					}

					if (startPin && endPin)
					{
						if (endPin == startPin)
						{
							client_ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
						}
						else if (endPin->Kind == startPin->Kind)
						{
							showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
							client_ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
						}
						//else if (endPin->Node == startPin->Node)
						//{
						//    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
						//    client_ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
						//}
						else if (endPin->Type != startPin->Type)
						{
							showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
							client_ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
						}
						else
						{
							showLabel("+ Create Link", ImColor(32, 45, 32, 180));
							if (client_ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
							{
								CreateLinkAndCreate(startPinId, endPinId);
							}
						}
					}
				}

				client_ed::PinId pinId = 0;
				if (client_ed::QueryNewNode(&pinId))
				{
					newLinkPin = FindPin(pinId);
					if (newLinkPin)
						showLabel("+ Create Node", ImColor(32, 45, 32, 180));

					if (client_ed::AcceptNewItem())
					{
						createNewNode = true;
						newNodeLinkPin = FindPin(pinId);
						newLinkPin = nullptr;
						client_ed::Suspend();
						ImGui::OpenPopup("Create New Node");
						client_ed::Resume();
					}
				}
			}
			else
				newLinkPin = nullptr;

			client_ed::EndCreate();

			if (client_ed::BeginDelete())
			{
				client_ed::NodeId nodeId = 0;
				while (client_ed::QueryDeletedNode(&nodeId))
				{
					if (client_ed::AcceptDeletedItem())
					{
						DeleteSequenceNode(nodeId);

						auto id = std::find_if(m_Nodes.begin(), m_Nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
						if (id != m_Nodes.end())
							m_Nodes.erase(id);
					}
				}

				client_ed::LinkId linkId = 0;
				while (client_ed::QueryDeletedLink(&linkId))
				{
					if (client_ed::AcceptDeletedItem())
					{
						DeleteLink(linkId);

						auto id = std::find_if(m_Links.begin(), m_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
						if (id != m_Links.end())
							m_Links.erase(id);
					}
				}
			}
			client_ed::EndDelete();
		}

		ImGui::SetCursorScreenPos(cursorTopLeft);
	}

# if 1
	auto openPopupPosition = ImGui::GetMousePos();
	client_ed::Suspend();

	if (client_ed::ShowNodeContextMenu(&contextNodeId))
		ImGui::OpenPopup("Node Context Menu");
	else if (client_ed::ShowPinContextMenu(&contextPinId))
		ImGui::OpenPopup("Pin Context Menu");
	else if (client_ed::ShowLinkContextMenu(&contextLinkId))
		ImGui::OpenPopup("Link Context Menu");
	else if (client_ed::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
		newNodeLinkPin = nullptr;
	}
	client_ed::Resume();

	client_ed::Suspend();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("Node Context Menu"))
	{
		auto node = FindNode(contextNodeId);

		ImGui::TextUnformatted("Node Context Menu");
		ImGui::Separator();
		if (node)
		{
			ImGui::Text("ID: %p", node->ID.AsPointer());
			ImGui::Text("Type: %s", node->Type == NodeType::Blueprint ? "Blueprint" : (node->Type == NodeType::Tree ? "Tree" : "Comment"));
			ImGui::Text("Inputs: %d", (int)node->Inputs.size());
			ImGui::Text("Outputs: %d", (int)node->Outputs.size());
			ImGui::Text("Pos: %f , %f", (float)node->Pos.x, (float)node->Pos.y);
		}
		else
			ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
		ImGui::Separator();
		if (ImGui::MenuItem("Delete"))
			client_ed::DeleteNode(contextNodeId);
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Pin Context Menu"))
	{
		auto pin = FindPin(contextPinId);

		ImGui::TextUnformatted("Pin Context Menu");
		ImGui::Separator();
		if (pin)
		{
			ImGui::Text("ID: %p", pin->ID.AsPointer());
			if (pin->Node)
				ImGui::Text("Node: %p", pin->Node->ID.AsPointer());
			else
				ImGui::Text("Node: %s", "<none>");
		}
		else
			ImGui::Text("Unknown pin: %p", contextPinId.AsPointer());

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Link Context Menu"))
	{
		auto link = FindLink(contextLinkId);

		ImGui::TextUnformatted("Link Context Menu");
		ImGui::Separator();
		if (link)
		{
			ImGui::Text("ID: %p", link->ID.AsPointer());
			ImGui::Text("From: %p", link->StartPinID.AsPointer());
			ImGui::Text("To: %p", link->EndPinID.AsPointer());
		}
		else
			ImGui::Text("Unknown link: %p", contextLinkId.AsPointer());
		ImGui::Separator();
		if (ImGui::MenuItem("Delete"))
			client_ed::DeleteLink(contextLinkId);
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Create New Node"))
	{
		auto newNodePostion = openPopupPosition;
		//ImGui::SetCursorScreenPos(ImGui::GetMousePosOnOpeningCurrentPopup());

		//auto drawList = ImGui::GetWindowDrawList();
		//drawList->AddCircleFilled(ImGui::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);

		ClientNode* node = nullptr;

		if (ImGui::MenuItem("Comment"))
			node = SpawnComment();
		ImGui::Separator();
		if (ImGui::MenuItem("Sequence"))
			node = SpawnTreeSequenceNode();

		if (ImGui::MenuItem("Move To"))
			node = SpawnTreeTaskNode();
		if (ImGui::MenuItem("Random Wait"))
			node = SpawnTreeTask2Node();
		ImGui::Separator();

		if (node)
		{
			node->SetPos(newNodePostion);
			BuildNodes();

			createNewNode = false;

			client_ed::SetNodePosition(node->ID, newNodePostion);

			if (auto startPin = newNodeLinkPin)
			{
				auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

				for (auto& pin : pins)
				{
					if (CanCreateLink(startPin, &pin))
					{
						auto endPin = &pin;
						if (startPin->Kind == PinKind::Input)
							std::swap(startPin, endPin);

						m_Links.emplace_back(Link(GetNextId(), startPin->ID, endPin->ID));
						m_Links.back().Color = GetIconColor(startPin->Type);

						break;
					}
				}
			}
		}

		ImGui::EndPopup();
	}
	else
		createNewNode = false;
	ImGui::PopStyleVar();
	client_ed::Resume();
# endif


	/*
		cubic_bezier_t c;
		c.p0 = pointf(100, 600);
		c.p1 = pointf(300, 1200);
		c.p2 = pointf(500, 100);
		c.p3 = pointf(900, 600);

		auto drawList = ImGui::GetWindowDrawList();
		auto offset_radius = 15.0f;
		auto acceptPoint = [drawList, offset_radius](const bezier_subdivide_result_t& r)
		{
			drawList->AddCircle(to_imvec(r.point), 4.0f, IM_COL32(255, 0, 255, 255));

			auto nt = r.tangent.normalized();
			nt = pointf(-nt.y, nt.x);

			drawList->AddLine(to_imvec(r.point), to_imvec(r.point + nt * offset_radius), IM_COL32(255, 0, 0, 255), 1.0f);
		};

		drawList->AddBezierCurve(to_imvec(c.p0), to_imvec(c.p1), to_imvec(c.p2), to_imvec(c.p3), IM_COL32(255, 255, 255, 255), 1.0f);
		cubic_bezier_subdivide(acceptPoint, c);
	*/

	client_ed::End();

	auto editorMin = ImGui::GetItemRectMin();
	auto editorMax = ImGui::GetItemRectMax();

	if (m_ShowOrdinals)
	{
		int nodeCount = client_ed::GetNodeCount();
		std::vector<client_ed::NodeId> orderedNodeIds;
		orderedNodeIds.resize(static_cast<size_t>(nodeCount));
		client_ed::GetOrderedNodeIds(orderedNodeIds.data(), nodeCount);


		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushClipRect(editorMin, editorMax);

		int ordinal = 0;
		for (auto& nodeId : orderedNodeIds)
		{
			auto p0 = client_ed::GetNodePosition(nodeId);
			auto p1 = p0 + client_ed::GetNodeSize(nodeId);
			p0 = client_ed::CanvasToScreen(p0);
			p1 = client_ed::CanvasToScreen(p1);


			ImGuiTextBuffer builder;
			builder.appendf("#%d", ordinal++);

			auto textSize = ImGui::CalcTextSize(builder.c_str());
			auto padding = ImVec2(2.0f, 2.0f);
			auto widgetSize = textSize + padding * 2;

			auto widgetPosition = ImVec2(p1.x, p0.y) + ImVec2(0.0f, -widgetSize.y);

			drawList->AddRectFilled(widgetPosition, widgetPosition + widgetSize, IM_COL32(100, 80, 80, 190), 3.0f, ImDrawFlags_RoundCornersAll);
			drawList->AddRect(widgetPosition, widgetPosition + widgetSize, IM_COL32(200, 160, 160, 190), 3.0f, ImDrawFlags_RoundCornersAll);
			drawList->AddText(widgetPosition + padding, IM_COL32(255, 255, 255, 255), builder.c_str());
		}

		drawList->PopClipRect();
	}

	UI* InspectorUI = ImGuiMgr::GetInst()->FindUI("##AniControllerInspector");
	AniControllerInspector* pInspector = dynamic_cast<AniControllerInspector*>(InspectorUI);
	client_ed::NodeId DoubleClickNodeId = 0;
	client_ed::LinkId DoubleClickLinkId = 0;
	DoubleClickNodeId = client_ed::GetDoubleClickedNode();
	if (DoubleClickNodeId.AsPointer() != nullptr)
	{
		auto node = FindNode(DoubleClickNodeId);
		pInspector->SetTargetNodeID(node->iID);
		pInspector->SetTargetLinkID(-1);
	}

	DoubleClickLinkId = client_ed::GetDoubleClickedLink();
	if (DoubleClickLinkId.AsPointer() != nullptr)
	{
		auto link = FindLink(DoubleClickLinkId);
		pInspector->SetTargetLinkID(link->iID);
		pInspector->SetTargetNodeID(-1);
	}

	UI* paramUI = ImGuiMgr::GetInst()->FindUI("##AniParameters");
	AniControllerEditParamUI* pParam = dynamic_cast<AniControllerEditParamUI*>(paramUI);
	const bool& bFlow = pParam->IsFlow();
	if (bFlow)
	{
		Ptr<CAnimatorController> pController = m_pAnimator->GetController();
		const int& id = pController->GetFlowLinkID();
		if (id >= 0)
		{
			Link* pLink = GetLink(id);
			client_ed::Flow(pLink->ID);
		}
	}
	//ImGui::ShowTestWindow();
	//ImGui::ShowMetricsWindow();
}

void AniControllerEditUI::SaveNodeInfo()
{
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	vector<CAniNode*> vecNodes = pController->GetAllNode();
	vector<ImVec2> vecPos;

	for (auto node : vecNodes)
	{
		ImVec2 position = client_ed::GetNodePosition(node->GetEditorNodeID());
		vecPos.push_back(position);
	}

	wstring path;
	path += pController->GetEditPath();
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, path.c_str(), L"wb");
	assert(pFile);
	int loopSize = vecPos.size();
	fwrite(&loopSize, sizeof(int), 1, pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		fwrite(&(vecPos[i]), sizeof(ImVec2), 1, pFile);
	}
	fclose(pFile);
}

void AniControllerEditUI::LoadNodeInfo()
{
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	vector<CAniNode*> Nodes = pController->GetAllNode();

	vector<ImVec2> vecPos;
	wstring path;
	path += pController->GetEditPath();

	FILE* pFile = NULL;
	_wfopen_s(&pFile, path.c_str(), L"rb");

	//Edit File 찾을 수 없음. (처음 실행 한 경우)
	if (pFile == nullptr)
		return;
	
	int loopSize = 0;
	fread(&loopSize, sizeof(int), 1, pFile);
	for (int i = 0; i < loopSize; ++i)
	{
		ImVec2 position = {};
		fread(&position, sizeof(ImVec2), 1, pFile);
		vecPos.push_back(position);
	}
	fclose(pFile);
	if (vecPos.size() == 0)
		return;
	int i = 0;
	for (auto node : Nodes)
	{
		//create
		ClientNode* Node = SpawnTreeSequenceEmptyNode();
		//name
		const wstring& Name = node->GetName();
		string strName(Name.begin(), Name.end());
		Node->Name = strName;
		//position
		ImVec2 Position = ImVec2(vecPos[i].x, vecPos[i].y);
		client_ed::SetNodePosition(Node->ID, Position);
		//engine node 에 id 등록
		node->SetEditorNodeID(Node->iID);
		pController->RegisterIDNode(Node->iID, node);
		i++;
	}
	BuildNodes();
}

void AniControllerEditUI::LoadLink(CAniNode* StartNode)
{
	if (StartNode == nullptr)
		return;
	if (StartNode->GetName() == L"Exit")
		return;
	ClientNode* EDStartNode = GetNode(StartNode->GetEditorNodeID());
	vector<CTransition*> vecLink = StartNode->GetOutTransitionAll();
	for (int i = 0; i < vecLink.size(); ++i)
	{
		CAniNode* EndNode = vecLink[i]->GetConnectNode();
		ClientNode* EDEndNode = GetNode(EndNode->GetEditorNodeID());
		ClientPin StartPin = EDStartNode->Outputs[0];
		ClientPin EndPin = EDEndNode->Inputs[0];

		CreateLinkAndRegister(StartPin.ID, EndPin.ID);
		LoadLink(EndNode);
	}
}


void AniControllerEditUI::DefaultNode()
{
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	CAniNode* Entry = pController->GetNode(L"Entry");
	CAniNode* Exit = pController->GetNode(L"Exit");
	CAniNode* AnyState = pController->GetNode(L"AnyState");
	if (Entry == nullptr && Exit == nullptr && AnyState == nullptr)
		pController->Init();

	ClientNode* EntryNode = SpawnTreeSequenceEmptyNode();
	EntryNode->Name = "Entry";
	client_ed::SetNodePosition(EntryNode->ID, ImVec2(0, 0));
	pController->RegisterIDNode(EntryNode->iID, Entry);

	ClientNode* ExitNode = SpawnTreeSequenceEmptyNode();
	ExitNode->Name = "Exit";
	client_ed::SetNodePosition(ExitNode->ID, ImVec2(-100, 100));
	pController->RegisterIDNode(ExitNode->iID, Exit);

	ClientNode* AnyStateNode = SpawnTreeSequenceEmptyNode();
	AnyStateNode->Name = "AnyState";
	client_ed::SetNodePosition(AnyStateNode->ID, ImVec2(100, 100));
	pController->RegisterIDNode(AnyStateNode->iID, AnyState);
}


void AniControllerEditUI::OpenEditInit()
{
	for (auto itr = m_Nodes.begin(); itr != m_Nodes.end();)
	{
		itr = m_Nodes.erase(itr);
	}
	for (auto itr = m_Links.begin(); itr != m_Links.end();)
	{
		itr = m_Links.erase(itr);
	}
	ClientNode::Index = 0;
	m_NextId = 1;
	
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	pController->EditInfoClear();
	pController->CopyParams(m_pAnimator);
}

void AniControllerEditUI::init()
{
	OnStart();
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

void AniControllerEditUI::end()
{
	OnStop();
}

int AniControllerEditUI::render_update()
{
	auto& io = ImGui::GetIO();

	OnFrame(io.DeltaTime);

	return 0;
}

void AniControllerEditUI::SetLink(CAnimator3D* _pAnimator)
{
	m_pAnimator = _pAnimator;
	OpenEditInit();

	LoadNodeInfo();
	Ptr<CAnimatorController> pController = m_pAnimator->GetController();
	CAniNode* EntryNode = pController->GetNode(L"Entry");
	CAniNode* AnyStateNode = pController->GetNode(L"AnyState");
	LoadLink(EntryNode);
	LoadLink(AnyStateNode);

	if (m_Nodes.size() == 0)
		DefaultNode();
}

ClientNode* AniControllerEditUI::GetNode(const int& _iID)
{
	for (int i = 0; i < m_Nodes.size(); ++i)
	{
		if (m_Nodes[i].iID == _iID)
			return &m_Nodes[i];
	}

	return nullptr;
}

Link* AniControllerEditUI::GetLink(const int& _iID)
{
	for (int i = 0; i < m_Links.size(); ++i)
	{
		if (m_Links[i].iID == _iID)
			return &m_Links[i];
	}

	return nullptr;
}


