﻿#include "pch.h"
#include "MenuUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CTaskMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>

#include "CImGuiMgr.h"
#include "Outliner.h"
#include "Content.h"
#include "Inspector.h"
#include "LogUI.h"
#include "ModelUI.h"
#include "RTViewPort.h"
#include "SettingUI.h"

#include "TreeUI.h"
#include "CLevelSaveLoad.h"

#include <Engine\CDevice.h>
#include <Engine\CRenderMgr.h>

MenuUI::MenuUI()
	: UI("Menu", "##Menu")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::render()
{
	if (ImGui::BeginMainMenuBar())
	{
		render_update();

		ImGui::EndMainMenuBar();
	}
}

void MenuUI::render_update()
{
	File();

	Level();

	GameObject();

	Asset();

	View();

	ImVec2 Blank = ImGui::GetContentRegionAvail();
	ImGui::Dummy(ImVec2(Blank.x - 140.f, 0.f));

	ScreenControl();

	if (KEY_TAP_ANY(F5))
	{
		GameStart();
	}

	if (KEY_TAP_ANY(F6))
	{
		GameStop();
	}

	if (KEY_TAP_ANY(F7))
	{
		GamePause();
	}
}
// #include <Engine/CLevel.h>

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save Level", ""))
		{
			wchar_t szSelect[256] = {};

			OPENFILENAME ofn = {};

			ofn.lStructSize	   = sizeof(ofn);
			ofn.hwndOwner	   = nullptr;
			ofn.lpstrFile	   = szSelect;
			ofn.lpstrFile[0]   = '\0';
			ofn.nMaxFile	   = sizeof(szSelect);
			ofn.lpstrFilter	   = L"ALL\0*.*\0Level\0*.lv";
			ofn.lpstrDefExt	   = L"lv";
			ofn.nFilterIndex   = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle  = 0;

			// 탐색창 초기 위치 지정
			wstring strInitPath = CPathMgr::GetContentPath();
			strInitPath += L"level\\";
			ofn.lpstrInitialDir = strInitPath.c_str();

			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			if (GetSaveFileName(&ofn))
			{
				string name = ToString((wchar_t*)ofn.lpstrFile);
				name		= path(name).stem().string();
				pLevel->SetName(name);
				pLevel->SetRelativePath(name);
				CLevelSaveLoad::SaveLevel(pLevel, CPathMgr::GetRelativePath(szSelect));
			}
		}

		if (ImGui::MenuItem("Load Level", ""))
		{
			wchar_t szSelect[256] = {};

			OPENFILENAME ofn = {};

			ofn.lStructSize	   = sizeof(ofn);
			ofn.hwndOwner	   = nullptr;
			ofn.lpstrFile	   = szSelect;
			ofn.lpstrFile[0]   = '\0';
			ofn.nMaxFile	   = sizeof(szSelect);
			ofn.lpstrFilter	   = L"ALL\0*.*\0Level\0*.lv";
			ofn.nFilterIndex   = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle  = 0;

			// 탐색창 초기 위치 지정
			wstring strInitPath = CPathMgr::GetContentPath();
			strInitPath += L"level\\";
			ofn.lpstrInitialDir = strInitPath.c_str();

			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn))
			{
				CLevel* pLevel = CLevelSaveLoad::LoadLevel(CPathMgr::GetRelativePath(szSelect));
				GamePlayStatic::ChangeLevel(pLevel, LEVEL_STATE::STOP);

				// Inspector 의 타겟정보를 nullptr 로 되돌리기
				Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
				pInspector->SetTargetObject(nullptr);
			}
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	if (ImGui::BeginMenu("Level"))
	{
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

		LEVEL_STATE State = pCurLevel->GetState();

		bool PlayEnable	 = false;
		bool PauseEnable = false;
		bool StopEnable	 = false;

		if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
			PlayEnable = true;
		else
			PlayEnable = false;

		if (LEVEL_STATE::PLAY == State)
			PauseEnable = true;
		else
			PauseEnable = false;

		if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
			StopEnable = true;
		else
			StopEnable = false;

		if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
		{
			GameStart();
		}

		if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
		{
			CRenderMgr::GetInst()->RePositionDebugCam();

			CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
		}

		if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
		{
			GameStop();
		}

		ImGui::EndMenu();
	}
}

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object", ""))
		{
			CGameObject* pNewObj = new CGameObject;
			pNewObj->SetName(L"New GameObject");
			pNewObj->AddComponent(new CTransform);
			GamePlayStatic::SpawnGameObject(pNewObj, 0);

			// Inspector 의 타겟정보를 새로 만든걸로 갱신
			Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
			pInspector->SetTargetObject(pNewObj);
		}
		ImGui::Separator();

		if (ImGui::BeginMenu("Component", ""))
		{
			if (ImGui::MenuItem("Cut", "CTRL+X"))
			{
			}
			if (ImGui::MenuItem("Copy", "CTRL+C"))
			{
			}
			if (ImGui::MenuItem("Paste", "CTRL+V"))
			{
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Script", ""))
		{
			vector<wstring> vecScriptName;
			CScriptMgr::GetScriptInfo(vecScriptName);

			for (size_t i = 0; i < vecScriptName.size(); ++i)
			{
				if (ImGui::MenuItem(ToString(vecScriptName[i]).c_str()))
				{
					Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
					if (nullptr != inspector->GetTargetObject())
					{
						inspector->GetTargetObject()->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));

						inspector->SetTargetObject(inspector->GetTargetObject());
					}
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			wchar_t szPath[255] = {};
			wstring FilePath	= CPathMgr::GetContentPath();

			int num = 0;
			while (true)
			{
				swprintf_s(szPath, L"Material//New Material_%d.mtrl", num);
				if (!exists(FilePath + szPath))
					break;
				++num;
			}

			CMaterial* pMtrl = new CMaterial;
			pMtrl->SetName(szPath);
			pMtrl->Save(szPath);
			GamePlayStatic::AddAsset(pMtrl);
		}

		if (ImGui::MenuItem("Content Save All"))
		{
			ContentSaveAll();
		}

		if (ImGui::MenuItem("Create Default GraphicsShader"))
		{
			CAssetMgr::GetInst()->SaveDefaultGraphicsShader();
		}

		ImGui::EndMenu();
	}
}

void MenuUI::View()
{
	if (ImGui::BeginMenu("View"))
	{
		auto pOutliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("##Outliner");
		if (ImGui::MenuItem("Outliner", "", false, !pOutliner->IsActivate()))
		{
			pOutliner->Activate();
		}

		auto pContent = (Content*)CImGuiMgr::GetInst()->FindUI("##Content");
		if (ImGui::MenuItem("Content", "", false, !pContent->IsActivate()))
		{
			pContent->Activate();
		}

		auto pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
		if (ImGui::MenuItem("Inspector", "", false, !pInspector->IsActivate()))
		{
			pInspector->Activate();
		}

		auto pLogUI = (LogUI*)CImGuiMgr::GetInst()->FindUI("##LogUI");
		if (ImGui::MenuItem("LogUI", "", false, !pLogUI->IsActivate()))
		{
			pLogUI->Activate();
		}

		auto pModelUI = (ModelUI*)CImGuiMgr::GetInst()->FindUI("##ModelUI");
		if (ImGui::MenuItem("ModelUI", "", false, !pModelUI->IsActivate()))
		{
			pModelUI->Activate();
		}

		auto pRTViewPort = (RTViewPort*)CImGuiMgr::GetInst()->FindUI("##Viewport");
		if (ImGui::MenuItem("RTViewPort", "", false, !pRTViewPort->IsActivate()))
		{
			pRTViewPort->Activate();
		}

		auto pSetting = (SettingUI*)CImGuiMgr::GetInst()->FindUI("##Setting");
		if (ImGui::MenuItem("Setting", "", false, !pSetting->IsActivate()))
		{
			pSetting->Activate();
		}

		ImGui::Separator();
		if (ImGui::MenuItem("ImguiDemo", "", false, !CImGuiMgr::GetInst()->m_bDemoUI))
		{
			CImGuiMgr::GetInst()->m_bDemoUI = true;
		}

		ImGui::EndMenu();
	}
}

#include <Engine/CAnim.h>

void MenuUI::ContentSaveAll()
{
	CPathMgr::GetContentPath();

	vector<string> vecNames;
	vector<string> vecPaths;
	vector<string> vecRelativePaths;
	Utils::LoadAllFileNames(CPathMgr::GetContentPath(), vecNames);
	Utils::LoadAllFilePaths(CPathMgr::GetContentPath(), vecPaths);
	vecRelativePaths = vecPaths;
	Utils::SlicePath(CPathMgr::GetContentPath(), vecRelativePaths);

	for (int i = 0; i < vecNames.size(); i++)
	{
		string name			   = vecNames[i];
		string strPath		   = vecPaths[i];
		string strRelativePath = vecRelativePaths[i];
		auto   extension	   = filesystem::path(name).extension();

		if (extension == ExtensionAnim)
		{
			CAnim*	 pAnim = new CAnim;
			ifstream fin(strPath);
			pAnim->LoadFromFile(fin);

			ofstream fout(strPath);
			pAnim->SaveToFile(fout);
			delete pAnim;
		}
		else if (extension == ExtensionPref)
		{
			CPrefab* pPref = new CPrefab;
			pPref->Load(strPath);
			pPref->Save(strRelativePath);

			delete pPref;
		}
		else if (extension == ExtensionMtrl)
		{
			CMaterial* pMtrl = new CMaterial;

			pMtrl->Load(strPath);
			pMtrl->Save(strRelativePath);

			delete pMtrl;
		}
		else if (extension == ExtensionLevel)
		{
			CLevel* pLevel;
			pLevel = CLevelSaveLoad::LoadLevel(strRelativePath);
			CLevelSaveLoad::SaveLevel(pLevel, strRelativePath);
			delete pLevel;
		}
		else if (extension == ".gs")
		{
			CGraphicsShader* shader = new CGraphicsShader;
			shader->Load(strPath);
			shader->Save(strRelativePath);
			delete shader;
		}
	}
}

void MenuUI::ScreenResize(Vec2 _Resolution, bool IsWindowMode)
{
	tTask Task;
	Task.Type	 = TASK_TYPE::CHANGE_RESOLUTION;
	Task.Param_1 = (UINT_PTR)_Resolution.x;
	Task.Param_2 = (UINT_PTR)_Resolution.y;

	CDevice::GetInst()->SetScreenMode(IsWindowMode);

	CTaskMgr::GetInst()->AddTask(Task);
}

#include <Engine\CEngine.h>

void MenuUI::ScreenControl()
{
	int ResolutionX = int(CDevice::GetInst()->GetRenderResolution().x);
	int ResolutionY = int(CDevice::GetInst()->GetRenderResolution().y);

	string Resolution = to_string(ResolutionX) + " X " + to_string(ResolutionY);

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 1.0f)); // 메뉴 아이템의 배경색 변경
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));	// 메뉴 아이템의 글자색 변경

	if (ImGui::BeginMenu(Resolution.c_str()))
	{
		static bool IsWindowMode = true;
		ImGui::Checkbox("Window Mode", &IsWindowMode);

		// TODO : 동윤이 CreateMRT 정상화 되면 GPS로 변경해야 함
		if (ImGui::MenuItem("1366 X 720"))
		{
			ScreenResize(Vec2(1366.f, 720.f), IsWindowMode);
		}

		if (ImGui::MenuItem("1910 X 960"))
		{
			ScreenResize(Vec2(1910.f, 960.f), IsWindowMode);
		}

		if (ImGui::MenuItem("1920 X 1080"))
		{
			ScreenResize(Vec2(1920.f, 1080.f), IsWindowMode);
		}

		if (ImGui::MenuItem("2560 X 1440"))
		{
			ScreenResize(Vec2(2560.f, 1440.f), IsWindowMode);
		}

		static int Resolution[2] = {0, 0};

		ImGui::InputInt2("##Custom Resolution", Resolution);
		ImGui::SameLine();

		if (ImGui::Button("Change"))
		{
			Vec2 CustomRes = {Resolution[0], Resolution[1]};
			CEngine::GetInst()->ResizeScreenResolution(CustomRes, IsWindowMode);
		}

		ImGui::EndMenu();
	}

	ImGui::PopStyleColor(2); // Push한 스타일 변경을 원래대로 복원

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));

	if (ImGui::Button("Exit"))
	{
		PostQuitMessage(0);
	}

	ImGui::PopStyleColor(3);
}

void MenuUI::GameStart()
{
	auto pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (LEVEL_STATE::STOP == pCurLevel->GetState())
	{
		CLevelSaveLoad::SaveLevel(pCurLevel, L"Level//temp.lv");
	}

	Outliner* pOutliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("##Outliner");
	auto	  m_Tree	= pOutliner->GetTree()->GetSelectedNode();
	m_Tree				= nullptr;

	CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PLAY);
}

#include "Engine\CKeyMgr.h"
void MenuUI::GameStop()
{
	CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"Level//temp.lv");
	pLoadedLevel->SetRelativePath(CLevelMgr::GetInst()->GetCurrentLevel()->GetRelativePath());
	GamePlayStatic::ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

	Outliner* pOutliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("##Outliner");
	auto	  m_Tree	= pOutliner->GetTree()->GetSelectedNode();
	m_Tree				= nullptr;

	// Inspector 의 타겟정보를 nullptr 로 되돌리기
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(nullptr);

	CKeyMgr::GetInst()->RoRShowCursor(true);
}

void MenuUI::GamePause()
{
	auto pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
}
