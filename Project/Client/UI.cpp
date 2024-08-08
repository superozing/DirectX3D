﻿#include "pch.h"
#include "UI.h"

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "ComponentUI.h"
#include "ScriptUI.h"

UI::UI(const string& _strName, const string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
	, m_bModal(false)
{
}

UI::~UI()
{
	Delete_Vec(m_vecChildUI);
}

void UI::StaticButton(const wstring& _content, STATIC_BTN_TYPE _type)
{
	StaticButton(ToString(_content).c_str(), _type);
}

void UI::StaticButton(const string& _content, STATIC_BTN_TYPE _type)
{
	StaticButton(_content.c_str(), _type);
}

void UI::StaticButton(const char* _content, STATIC_BTN_TYPE _type)
{
	auto& tColor = CImGuiMgr::GetInst()->GetStaticButtonColor(_type);

	ImGui::PushStyleColor(ImGuiCol_Button, tColor.ColBtnColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, tColor.ColBtnHoveredColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, tColor.ColBtnActiveColor);

	ImGui::Button(_content);

	ImGui::PopStyleColor(3);
}

bool UI::TitleCollapse(const char* _content)
{
	ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));

	Ptr<CTexture> BtnTex   = CAssetMgr::GetInst()->Load<CTexture>(TEXIconSetting);
	ImTextureID	  BtnTexID = (ImTextureID)(BtnTex->GetSRV().Get());

	float  FrameHeight = ImGui::GetFrameHeight();
	float  padding	   = ImGui::GetFrameHeightWithSpacing() - ImGui::GetFrameHeight();
	ImVec2 Btnsize	   = ImVec2(FrameHeight - padding, FrameHeight - padding);

	if (ImGui::CollapsingHeader(_content, ImGuiTreeNodeFlags_AllowItemOverlap))
	{

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f, 0.f));

		ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x - Btnsize.x);
		if (ImGui::ImageButton(BtnTexID, Btnsize))
		{
			ImGui::OpenPopup("HeaderSetting");
		}

		if (ImGui::BeginPopup("HeaderSetting"))
		{
			HeaderSetting(this);
			ImGui::EndPopup();
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleColor(3);
		return true;
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f, 0.f));

		ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x - Btnsize.x);
		if (ImGui::ImageButton(BtnTexID, Btnsize))
		{
			ImGui::OpenPopup("HeaderSetting");
		}

		if (ImGui::BeginPopup("HeaderSetting"))
		{
			HeaderSetting(this);
			ImGui::EndPopup();
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleColor(3);
		return false;
	}
}

void UI::tick()
{
}

void UI::render()
{
	if (!m_bActive)
		return;

	bool Active = m_bActive;

	if (nullptr == GetParentUI())
	{
		// Modal
		if (!m_bModal)
		{
			ImGui::Begin(string(m_strName + m_strID).c_str(), &Active, m_iImguiFlag);

			// 활성화, 비활성화 전환이 발생한 경우에는 Activate or Deactivate 를 호출시킨다.
			if (Active != m_bActive)
			{
				m_bActive = Active;

				if (m_bActive)
					Activate();
				else
					Deactivate();
			}

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();
			}

			ImGui::End();
		}

		// Modal
		else
		{
			ImGui::OpenPopup(string(m_strName + m_strID).c_str());
			if (ImGui::BeginPopupModal(string(m_strName + m_strID).c_str(), &Active))
			{
				render_update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->render();
				}

				ImGui::EndPopup();
			}

			else
			{
				// 활성화, 비활성화 전환이 발생한 경우에는 Activate or Deactivate 를 호출시킨다.
				if (Active != m_bActive)
				{
					m_bActive = Active;

					if (m_bActive)
						Activate();
					else
						Deactivate();
				}
			}
		}
	}

	// Child UI 인 경우
	else
	{
		ImGui::BeginChild(string(m_strName + m_strID).c_str(), ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}

bool UI::ColorSelector(const char* _label, Vec4* _col)
{
	auto	   items	   = GamePlayStatic::COLOR::GetColors();
	static int selectedidx = 0;

	if (0 != items.size())
	{
		if (!(selectedidx < items.size()))
		{
			selectedidx = 0;
		}
		ImGui::SetNextItemWidth(150);
		if (ImGui::BeginCombo("##comboColorPreset", items[selectedidx].first.c_str()))
		{
			for (int i = 0; i < items.size(); i++)
			{
				const bool isSelected = (selectedidx == i);
				if (ImGui::Selectable(items[i].first.c_str(), isSelected))
				{
					*_col		= items[i].second;
					selectedidx = i;
				}

				// 항목 선택 시 자동으로 스크롤
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	if (ImGui::ColorEdit4(_label, *_col))
	{
		return true;
	}
}

void UI::HeaderSetting(UI* _SelectedHeader)
{
	assert(nullptr != dynamic_cast<ComponentUI*>(_SelectedHeader));

	COMPONENT_TYPE Type = dynamic_cast<ComponentUI*>(_SelectedHeader)->GetType();

	if (COMPONENT_TYPE::SCRIPT != Type)
	{
		if (COMPONENT_TYPE::TRANSFORM == Type)
			ImGui::BeginDisabled();

		if (ImGui::MenuItem("Delete Component", ""))
		{
			DeleteHeaderComponent(Type);
		}

		if (COMPONENT_TYPE::TRANSFORM == Type)
			ImGui::EndDisabled();
	}
	else
	{
		ScriptUI* pScript = dynamic_cast<ScriptUI*>(_SelectedHeader);

		if (ImGui::MenuItem("Delete Script", ""))
		{
			DeleteHeaderScript(pScript);
		}
	}
}

void UI::DeleteHeaderComponent(COMPONENT_TYPE _Type)
{
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->DeleteTargetComponent(_Type);
}

void UI::DeleteHeaderScript(ScriptUI* _ScriptUI)
{
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->DeleteTargetScript(_ScriptUI);
}
