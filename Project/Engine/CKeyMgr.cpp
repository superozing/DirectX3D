#include "pch.h"
#include "CKeyMgr.h"
#include "CEngine.h"

// clang-format off
int g_KeySync[KEY::KEY_END] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4,
	VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7,  VK_NUMPAD8, VK_NUMPAD9,	

	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', 

	VK_TAB, VK_LSHIFT, VK_LMENU, VK_LCONTROL, VK_SPACE,
	VK_ESCAPE, VK_RETURN, VK_BACK, VK_DELETE,
	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, 
	VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,

	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6,
	VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
};
// clang-format on

CKeyMgr::CKeyMgr()
{
	_inWheel = WHEEL_NONE;
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
	// Capaticy 를 미리 확보
	m_vecKeyData.reserve(KEY::KEY_END);

	// 데이터 개수를 늘림
	// m_vecKeyData.resize(KEY::KEY_END);

	for (UINT i = 0; i < KEY_END; ++i)
	{
		m_vecKeyData.push_back(FKeyData{(KEY)i, NONE, false});
	}
}

void CKeyMgr::tick()
{
	// GetFocus()는 현재 포커싱 되어있는 윈도우를 반환한다.
	// 메인 윈도우와 비교해서 메인 윈도우가 포커싱 되어있는지 판단한다.

	// if (GetFocus() == nullptr)
	//{
	//	m_FocusState = FOCUS_STATE::NONE;
	// }
	// else if (GetFocus() == CEngine::GetInst()->GetMainWind())
	//{
	//	m_FocusState = FOCUS_STATE::MAIN;
	// }
	// else
	//{
	//	m_FocusState = FOCUS_STATE::OTHER;
	// }

	// Wheel
	m_ThisFrameWheel = _inWheel;
	_inWheel		 = WHEEL_NONE;

	// FOCUS_STATE 가져오기
	m_FocusState = m_FocusCallback();

	// 1. 포커싱 되어있는 창이 없을 경우
	if (FOCUS_STATE::NONE == m_FocusState)
	{
		for (size_t i = 0; i < m_vecKeyData.size(); ++i)
		{
			switch (m_vecKeyData[i].eState)
			{
			case TAP:
				m_vecKeyData[i].eState = PRESSED;
				break;
			case PRESSED:
				m_vecKeyData[i].eState = RELEASED;
				break;
			case RELEASED:
				m_vecKeyData[i].eState = NONE;
				break;
			}
		}
	}
	// 2. 포커싱 되어있는 창이 있을 경우
	else
	{
		for (size_t i = 0; i < m_vecKeyData.size(); ++i)
		{
			if (GetAsyncKeyState(g_KeySync[m_vecKeyData[i].eKey]) & 0x8001)
			{
				// 이번 프레임에 해당 키가 눌려있다.
				if (m_vecKeyData[i].bPressed)
				{
					// 이전에도 눌려있었다 ==> 계속 눌림 상태
					m_vecKeyData[i].eState = PRESSED;
				}
				else
				{
					// 이전에는 눌려있지 않았다 ==> 막 눌림 상태
					m_vecKeyData[i].eState = TAP;
				}

				m_vecKeyData[i].bPressed = true;
			}
			else
			{
				// 이번 프레임에 해당키는 안 눌려 있다
				if (m_vecKeyData[i].bPressed)
				{
					// 이전 프레임에는 눌려 있었다 ==> 막 뗌
					m_vecKeyData[i].eState = RELEASED;
				}
				else
				{
					m_vecKeyData[i].eState = NONE;
				}

				m_vecKeyData[i].bPressed = false;
			}
		}

		// 마우스 중앙 고정 옵션 시 마우스 숨김.

		// 마우스 중앙 고정 옵션
		if (m_bHoldMouseCenter)
		{
			POINT pt = {};
			GetCursorPos(&pt);
			ScreenToClient(CEngine::GetInst()->GetMainWind(), &pt);
			m_vMousePos = Vec2((float)pt.x, (float)pt.y);

			// 현재 메인 윈도우 가져오기
			HWND MainWind = CEngine::GetInst()->GetMainWind();

			// RECT 구조체에 전체 화면 상에서 현재 윈도우가 있는 위치 가져오기
			RECT WindRect{};
			GetWindowRect(MainWind, &WindRect);

			// 현재 윈도우의 중앙 좌표 구하기
			POINT WindCenterPos{};
			WindCenterPos.x = WindRect.left + (WindRect.right - WindRect.left) / 2;
			WindCenterPos.y = WindRect.top + (WindRect.bottom - WindRect.top) / 2;

			// 윈도우의 중앙 좌표로 커서 위치 이동
			SetCursorPos(WindCenterPos.x, WindCenterPos.y);

			// 커서 위치를 게임 화면 기준으로 이동시키기
			ScreenToClient(CEngine::GetInst()->GetMainWind(), &WindCenterPos);

			m_vMousePrevPos = Vec2((float)WindCenterPos.x, (float)WindCenterPos.y);
		}
		else
		{
			// 이전 마우스 좌표 정보 저장하기
			m_vMousePrevPos = m_vMousePos;

			POINT pt = {};
			GetCursorPos(&pt);
			ScreenToClient(CEngine::GetInst()->GetMainWind(), &pt);
			m_vMousePos = Vec2((float)pt.x, (float)pt.y);
		}

		// 마우스 이동 방향 계산
		m_vMouseDrag = m_vMousePos - m_vMousePrevPos;

		// Temp Escape Key
		if (KEY_TAP(KEY::RBTN))
		{
			m_bHoldMouseCenter = !m_bHoldMouseCenter;
			ShowCursor(!m_bHoldMouseCenter);
		}
	}
}