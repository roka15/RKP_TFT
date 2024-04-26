#pragma once
#include "UI.h"
#include "ImGui\imgui_node_editor.h"
#include "ImGui\imgui_internal.h"
#include "builders.h"
#include "widgets.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
static inline ImRect ImGui_GetItemRect()
{
	return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
	auto result = rect;
	result.Min.x -= x;
	result.Min.y -= y;
	result.Max.x += x;
	result.Max.y += y;
	return result;
}

namespace client_ed = ax::NodeEditor;
namespace client_util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

static client_ed::EditorContext* m_Editor = nullptr;

enum class PinType
{
	Flow,
	Bool,
	Int,
	Float,
	String,
	Object,
	Function,
	Delegate,
};

enum class PinKind
{
	Output,
	Input
};

enum class NodeType
{
	Blueprint,
	Simple,
	Tree,
	Comment,
	Houdini
};

struct ClientNode;

struct ClientPin
{
	client_ed::PinId   ID;
	::ClientNode* Node;
	std::string Name;
	PinType     Type;
	PinKind     Kind;

	ClientPin(int id, const char* name, PinType type) :
		ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
	{
	}
};

struct ClientNode
{
	static int Index;
	client_ed::NodeId ID;
	int  iID;
	std::string Name;
	int  iAniKey;
	std::vector<ClientPin> Inputs;
	std::vector<ClientPin> Outputs;
	ImVec2                 Pos;
	ImColor Color;
	NodeType Type;
	ImVec2 Size;

	std::string State;
	std::string SavedState;

	ClientNode(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
		ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0),iID(0),iAniKey(0)
	{
	}
	void SetPos(ImVec2 _pos)
	{
		Pos = _pos;
	}
};


struct Link
{
	client_ed::LinkId ID;
	int iID;
	std::string Name;
	bool        HasExitTime;
	float       BlendTime;
	client_ed::PinId StartPinID;
	client_ed::PinId EndPinID;

	ImColor Color;

	Link(client_ed::LinkId id, client_ed::PinId startPinId, client_ed::PinId endPinId) :
		ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255),Name(),HasExitTime(false),BlendTime(0.f)
	{
	}
};

struct NodeIdLess
{
	bool operator()(const client_ed::NodeId& lhs, const client_ed::NodeId& rhs) const
	{
		return lhs.AsPointer() < rhs.AsPointer();
	}
};

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}
class CAnimator3D;
class CAniNode;
class CAnimatorController;
class AniControllerEditUI
	:public UI
{
	int GetNextId()
	{
		return m_NextId++;
	}

	//client_ed::NodeId GetNextNodeId()
	//{
	//    return client_ed::NodeId(GetNextId());
	//}

	client_ed::LinkId GetNextLinkId();
	void TouchNode(client_ed::NodeId id);
	float GetTouchProgress(client_ed::NodeId id);

	void UpdateTouch();
	ClientNode* FindNode(client_ed::NodeId id);
	Link* FindLink(client_ed::LinkId id);
	ClientPin* FindPin(client_ed::PinId id);
	bool IsPinLinked(client_ed::PinId id);
	bool CanCreateLink(ClientPin* a, ClientPin* b);
	void BuildNode(ClientNode* node);
	ClientNode* SpawnTreeSequenceNode();
	ClientNode* SpawnTreeSequenceEmptyNode();
	ClientNode* SpawnTreeTaskNode()
	{
		m_Nodes.emplace_back(GetNextId(), "Move To");
		m_Nodes.back().Type = NodeType::Tree;
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);

		BuildNode(&m_Nodes.back());

		return &m_Nodes.back();
	}
	ClientNode* SpawnTreeTask2Node()
	{
		m_Nodes.emplace_back(GetNextId(), "Random Wait");
		m_Nodes.back().Type = NodeType::Tree;
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);

		BuildNode(&m_Nodes.back());

		return &m_Nodes.back();
	}
	ClientNode* SpawnComment()
	{
		m_Nodes.emplace_back(GetNextId(), "Test Comment");
		m_Nodes.back().Type = NodeType::Comment;
		m_Nodes.back().Size = ImVec2(300, 200);

		return &m_Nodes.back();
	}
	
	void DeleteSequenceNode(client_ed::NodeId _id);
	void DeleteLink(client_ed::LinkId _id);
	
	void ReCreateFontAtlas();
	void BuildNodes();
	void OnStart();
	void OnStop();
	ImColor GetIconColor(PinType type);
	void DrawPinIcon(const ClientPin& pin, bool connected, int alpha);
	void ShowStyleEditor(bool* show = nullptr);
	void ShowLeftPane(float paneWidth);
	void OnFrame(float deltaTime);

private:
	void SaveNodeInfo();
	void LoadNodeInfo();
	void LoadLink(CAniNode* StartNode);
	Link* CreateLinkAndRegister(client_ed::PinId _start, client_ed::PinId _end);
	Link* CreateLinkAndCreate(client_ed::PinId _start, client_ed::PinId _end);
	void DefaultNode();
	void OpenEditInit();

public:
	virtual void init()override;
	virtual void tick()override;
	virtual void finaltick()override;
	virtual void end()override;
	virtual int render_update();
	void SetLink(CAnimator3D* _pAnimator); 
	ClientNode* GetNode(const int& _iID);
	Link* GetLink(const int& _iID);
public:
	AniControllerEditUI();
	virtual ~AniControllerEditUI();
private:
	friend class AniControllerEditParamUI;
private:
	int							 m_NextId = 1;
	const int					 m_PinIconSize = 24;
	std::vector<ClientNode>		 m_Nodes;
	std::vector<Link>			 m_Links;
	ImTextureID					 m_HeaderBackground = nullptr;
	ImTextureID					 m_SaveIcon = nullptr;
	ImTextureID					 m_RestoreIcon = nullptr;
	const float					 m_TouchTime = 1.0f;
	std::map<client_ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
	bool										   m_ShowOrdinals = false;
	ImFont*										   m_DefaultFont = nullptr;
	ImFont*										   m_HeaderFont = nullptr;

	CAnimator3D*								   m_pAnimator;
};


