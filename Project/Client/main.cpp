﻿#include "pch.h"
#include "framework.h"
#include "Client.h"

#include <crtdbg.h>

#include <Engine\global.h>
#include <Engine\CEngine.h>
#include <Engine\CDevice.h>
#include <Engine/CPrefab.h>
#include <Engine/CKeyMgr.h>
#include <Engine\CCamera.h>

#include "CLevelSaveLoad.h"

#include <Engine\CLogMgr.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_d.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Scripts\\Scripts_d.lib")
#else
#pragma comment(lib, "Scripts\\Scripts.lib")
#endif

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "CImGuiMgr.h"
#include "CEditorObjMgr.h"
#include "CCreateTempLevel.h"
#include "CUITempLevel.h"
#include "MapTestLevel.h"
#include "CCreatePlayerTestLevel.h"

#include <Engine/CRenderMgr.h>
#include "RTViewPort.h"

#include "CEnvMgr.h"
// #define _RELEASE_GAME

HINSTANCE hInst;
HWND	  hWnd;

static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

ATOM			 MyRegisterClass(HINSTANCE hInstance);
BOOL			 InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
					  _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(412);

	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
	MSG	   msg;

	CPathMgr::init();
	CEnvMgr::GetInst()->init();

	Vec2 res;
	res = CEnvMgr::GetInst()->GetResolutionData().res;
	if (res.x <= 200.f || res.x >= 4000.f)
	{
		res.x = 1920.f;
	}
	if (res.y <= 200.f || res.y >= 4000.f)
	{
		res.y = 1080.f;
	}
	// CEngine 초기화 실패 -> 프로그램 종료
	if (FAILED(CEngine::GetInst()->init(hWnd, res)))
	{
		MessageBox(nullptr, L"CEngine 초기화 실패", L"초기화 실패", MB_OK);
		return 0;
	}

	CPrefab::GAMEOBJECT_SAVE	 = &CLevelSaveLoad::SaveGameObject;
	CPrefab::GAMEOBJECT_LOAD	 = &CLevelSaveLoad::LoadGameObject;
	CRenderMgr::CameraChange	 = &RTViewPort::SetCamera;
	CCamera::ViewportConvertFunc = &RTViewPort::ConvertCoord;

#ifndef _RELEASE_GAME

	// 임시 레벨 생성
	// CCreateTempLevel::Init();
	// CCreateTempLevel::CreateTempLevel();

	// MapTestLevel::CreateMapTestLevel();
	string levelPath = CEnvMgr::GetInst()->GetLevelRelativePath();
	string abPath	 = ToString(CPathMgr::GetContentPath()) + levelPath;
	if (levelPath == "" || !exists(abPath))
	{
		CCreatePlayerTestLevel::CreateTempLevel();
	}
	else
	{
		auto pLevel = CLevelSaveLoad::LoadLevel(CEnvMgr::GetInst()->GetLevelRelativePath());
		GamePlayStatic::ChangeLevel(pLevel, LEVEL_STATE::STOP);
	}

	// ImGui 초기화
	CImGuiMgr::GetInst()->init(hWnd, DEVICE, CONTEXT);

	// EditorObjectManager 초기화
	CEditorObjMgr::GetInst()->init();

	CKeyMgr::GetInst()->m_FocusCallback = &CImGuiMgr::GetFocus_debug;
#else
	CKeyMgr::GetInst()->m_FocusCallback = &CImGuiMgr::GetFocus_release;
#endif

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{
			// Engine Update
			CEngine::GetInst()->progress();

#ifndef _RELEASE_GAME
			// EditorObj
			CEditorObjMgr::GetInst()->progress();

			// ImGui Update
			CImGuiMgr::GetInst()->progress();
#endif

			// Engine 및 ImGui 렌더링 최종 결과를 출력한다.
			CDevice::GetInst()->Present();
		}
	}

	CEnvMgr::GetInst()->exit();

	return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style		   = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra	   = 0;
	wcex.cbWndExtra	   = 0;
	wcex.hInstance	   = hInstance;
	wcex.hIcon		   = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor	   = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = nullptr; // MAKEINTRESOURCEW(IDC_CLIENT);
	wcex.lpszClassName = L"MyWindow";
	wcex.hIconSm	   = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	hWnd = CreateWindowW(L"MyWindow", L"Client", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
						 nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC			hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			// const int dpi = HIWORD(wParam);
			// printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top,
						   suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top,
						   SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	case WM_EXITSIZEMOVE: {
		RECT rect;

		if (GetClientRect(hWnd, &rect))
		{
			// rect 구조체에 윈도우의 크기와 위치 정보가 저장됨
			int	 width	= rect.right - rect.left;
			int	 height = rect.bottom - rect.top;
			Vec2 newRes(width, height);
			auto vRes = CDevice::GetInst()->GetRenderResolution();
			if (vRes == newRes)
				break;

			CEngine::GetInst()->ResizeScreenResolution(newRes, true);
		}
	}
	break;
#include <Engine\CKeyMgr.h>
	case WM_MOUSEWHEEL:
		if ((SHORT)HIWORD(wParam) > 0)
			CKeyMgr::GetInst()->SetWheel(WHEEL_STATE::WHEEL_UP);
		if ((SHORT)HIWORD(wParam) < 0)
			CKeyMgr::GetInst()->SetWheel(WHEEL_STATE::WHEEL_DOWN);

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
