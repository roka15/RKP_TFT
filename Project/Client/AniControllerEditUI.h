#pragma once
#include "UI.h"
#include "ImGui\imgui_node_editor.h"
namespace imguiNodeEdit = ax::NodeEditor;
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


struct Node;

struct Pin
{
    imguiNodeEdit::PinId   ID;
    ::Node* Node;
    std::string Name;
    PinType     Type;
    PinKind     Kind;

    Pin(int id, const char* name, PinType type) :
        ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
    {
    }
};

struct Node
{
    imguiNodeEdit::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;

    std::string State;
    std::string SavedState;

    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
        ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
    {
    }
};

struct Link
{
    imguiNodeEdit::LinkId ID;

    imguiNodeEdit::PinId StartPinID;
    imguiNodeEdit::PinId EndPinID;

    ImColor Color;

    Link(imguiNodeEdit::LinkId id, imguiNodeEdit::PinId startPinId, imguiNodeEdit::PinId endPinId) :
        ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {
    }
};

struct NodeIdLess
{
    bool operator()(const imguiNodeEdit::NodeId& lhs, const imguiNodeEdit::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};
class AniControllerEditUI
	:public UI
{
private:
	imguiNodeEdit::EditorContext* m_Context = nullptr;
    int                  m_NextId = 1;
    const int            m_PinIconSize = 24;
    std::vector<Node>    m_Nodes;
    std::vector<Link>    m_Links;
    ImTextureID          m_HeaderBackground = nullptr;
    ImTextureID          m_SaveIcon = nullptr;
    ImTextureID          m_RestoreIcon = nullptr;
    const float          m_TouchTime = 1.0f;
    std::map<imguiNodeEdit::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool                 m_ShowOrdinals = false;
private:
	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
	void ShowLeftPane(float paneWidth);
    void ShowStyleEditor(bool* show = nullptr);
    float GetTouchProgress(imguiNodeEdit::NodeId id);
public:
	virtual void init();
	virtual void tick();
	virtual void finaltick();
	virtual int render_update();
public:
public:
	AniControllerEditUI();
	virtual ~AniControllerEditUI();
};


