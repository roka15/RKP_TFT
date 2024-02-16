#pragma once
#include "UI.h"
#include "ImGui\imgui_node_editor.h"
namespace imguiNodeEdit = ax::NodeEditor;
class AniControllerEditUI
	:public UI
{
private:
	imguiNodeEdit::EditorContext* m_Context = nullptr;
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


