#pragma once

#include "imgui.h"
#include "CImGuiMgr.h"

class ScriptUI;

typedef std::function<void()> SetFunc;

class UI
{
private:
	string		 m_strName;
	const string m_strID;
	ImVec2		 m_vSize;
	bool		 m_bActive;
	bool		 m_bModal;

	UI*			m_Parent;
	vector<UI*> m_vecChildUI;

protected:
	static void StaticButton(const wstring& _content, STATIC_BTN_TYPE _type = STATIC_BTN_TYPE::TITLE);
	static void StaticButton(const string& _content, STATIC_BTN_TYPE _type = STATIC_BTN_TYPE::TITLE);

	static void StaticButton(const char* _content, STATIC_BTN_TYPE _type = STATIC_BTN_TYPE::TITLE);

	bool TitleCollapse(const char* _content);

	template <typename T> bool PayloadCheck(T** _Ptr);

	const vector<UI*>& GetChildUI() { return m_vecChildUI; }

public:
	void		  SetName(const string& _name) { m_strName = _name; }
	const string& GetName() { return m_strName; }
	const string& GetID() { return m_strID; }

	void   SetSize(ImVec2 _Size) { m_vSize = _Size; }
	ImVec2 GetSize() { return m_vSize; }

	void AddChildUI(UI* _ChildUI)
	{
		m_vecChildUI.push_back(_ChildUI);
		_ChildUI->m_Parent = this;
	}

	UI*			 GetParentUI() { return m_Parent; }
	virtual void Activate() { m_bActive = true; }
	virtual void Deactivate() { m_bActive = false; }
	bool		 IsActivate() { return m_bActive; }
	void		 SetModal(bool _Modal) { m_bModal = _Modal; }

	void HeaderSetting(UI* _SelectedHeader);
	void DeleteHeaderComponent(COMPONENT_TYPE _Type);
	void DeleteHeaderScript(ScriptUI* _ScriptUI);

public:
	virtual void enter() {}
	virtual void tick();
	virtual void render();
	virtual void render_update() = 0;

public:
	UI(const string& _strName, const string& _strID);
	virtual ~UI();

public:
	static bool ColorSelector(const char* _label, Vec4* _col);

	friend class ParamUI;
};

template <typename T> inline bool UI::PayloadCheck(T** _Ptr)
{
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data	  = *((DWORD_PTR*)payload->Data);
			CAsset*	  ptrData = (CAsset*)data;

			// 레벨 들어가면 터짐
			T* Res = dynamic_cast<T*>(ptrData);

			if (Res != nullptr)
			{
				*_Ptr = Res;
				ImGui::EndDragDropTarget();
				return true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	return false;
}
