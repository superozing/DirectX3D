#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CGC.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CFontMgr.h"
#include "CUIMgr.h"
#include "CInstancingBuffer.h"
#include "CSound.h"
#include "CPhysXMgr.h"
#include "CMemoryPoolMgr.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_vResolution()
{
}

CEngine::~CEngine()
{
	if (nullptr != CSound::g_pFMOD)
	{
		CSound::g_pFMOD->release();
		CSound::g_pFMOD = nullptr;
	}
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution, bool bWindowMode)
{
	m_hMainWnd	  = _hWnd;
	m_vResolution = _vResolution;

	RECT rt = {0, 0, (int)m_vResolution.x, (int)m_vResolution.y};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution, bWindowMode)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
		return E_FAIL;
	}

	// Manager 초기화
	CPathMgr::init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CAssetMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();
	CFontMgr::GetInst()->init();
	CInstancingBuffer::GetInst()->init();
	CPhysXMgr::GetInst()->init();
	CMemoryPoolMgr::GetInst()->init();
	CUIMgr::GetInst()->init();

	return S_OK;
}

void CEngine::progress()
{
	// Manager Update
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();

	// FMOD Update
	CSound::g_pFMOD->update();

	// UI Update
	CUIMgr::GetInst()->tick();

	// Level Update
	CLevelMgr::GetInst()->tick();

	// Font Render
	CFontMgr::GetInst()->render();
	CTimeMgr::GetInst()->render();
	CPhysXMgr::GetInst()->tick();

	// Task
	CTaskMgr::GetInst()->tick();

	// GC
	CGC::GetInst()->tick();

	DebugFunctionCheck();

	TestFunction();
}

void CEngine::ResizeScreenResolution(Vec2 NewResolution, bool bWindowMode)
{
	// 현재 창 스타일 가져오기
	LONG style = GetWindowLong(m_hMainWnd, GWL_STYLE);

	if (!bWindowMode)
	{
		// 전체화면 모드 설정
		if (style & WS_OVERLAPPEDWINDOW)
		{
			SetWindowLong(m_hMainWnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
			MONITORINFO monitorInfo = {sizeof(monitorInfo)};
			if (GetMonitorInfo(MonitorFromWindow(m_hMainWnd, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
			{
				SetWindowPos(m_hMainWnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
							 monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
							 monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
							 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
	}
	else
	{
		// 창 모드 설정
		if (!(style & WS_OVERLAPPEDWINDOW))
		{
			style |= WS_OVERLAPPEDWINDOW;
			SetWindowLong(m_hMainWnd, GWL_STYLE, style);
		}

		RECT rt = {0, 0, (int)NewResolution.x, (int)NewResolution.y};
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top,
					 SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	CDevice::GetInst()->SetScreenMode(bWindowMode);

	// 해상도 변경 시도
	if (FAILED(CDevice::GetInst()->RenewResolution(NewResolution, bWindowMode)))
	{
		MessageBox(nullptr, L"해상도 변경 실패", L"해상도 변경 실패", MB_OK);
		assert(true);
	}
}

void CEngine::DebugFunctionCheck()
{
	if (KEY_TAP(KEY::O))
	{
		CRenderMgr::GetInst()->IsDebugPosition() ? CRenderMgr::GetInst()->SetDebugPosition(false)
												 : CRenderMgr::GetInst()->SetDebugPosition(true);
	}
}

#include "CRandomMgr.h"

void CEngine::TestFunction()
{
	// if (KEY_TAP(KEY::Y)) {
	//	CRandomMgr::GetInst()->Test();
	// }
}
