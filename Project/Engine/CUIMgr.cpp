#include "pch.h"
#include "CUIMgr.h"

#include "CKeyMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CLogMgr.h"
#include "CDevice.h"

#include "CUIScript.h"
#include "CGameObject.h"

void CUIMgr::init()
{
	m_arrUIActive[(UINT)UI_TYPE::DEFAULT]	 = true;
	m_arrUIActive[(UINT)UI_TYPE::CROSSHAIR]	 = true;
	m_arrUIActive[(UINT)UI_TYPE::DAMAGEFONT] = true;
	m_arrUIActive[(UINT)UI_TYPE::PAUSEBTN]	 = true;
	m_arrUIActive[(UINT)UI_TYPE::PAUSEPANEL] = true;
	m_arrUIActive[(UINT)UI_TYPE::AMMO]		 = true;
	m_arrUIActive[(UINT)UI_TYPE::BOSSHP]	 = true;
	m_arrUIActive[(UINT)UI_TYPE::MONSTERHP]	 = true;
	m_arrUIActive[(UINT)UI_TYPE::PLAYERHP]	 = true;
	m_arrUIActive[(UINT)UI_TYPE::SKILLICON]	 = true;
}

void CUIMgr::tick()
{
	Vec2 vMousePos	   = CKeyMgr::GetInst()->GetMousePos();
	bool bLBtnTap	   = KEY_TAP(KEY::LBTN);
	bool bLbtnReleased = KEY_RELEASED(KEY::LBTN);

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pLevel)
		return;

	// UILayer 가져오기
	CLayer* pUILayer = pLevel->GetLayer((UINT)LAYER::LAYER_UI);

	// UILayer가 존재하지 않을 경우
	if (!pUILayer)
	{
		CLogMgr::GetInst()->AddLog(Log_Level::ERR, "UI Layer Does Not Exist");
		return;
	}

	// 상위 UI 오브젝트가 자식 UI 오브젝트를 들고 있는 구조에요.
	auto& vecUI = pUILayer->AccessParentObjects();
	auto  iter	= vecUI.rbegin();

	for (; iter != vecUI.rend(); ++iter)
	{
		// 오브젝트가 존재하는지 확인
		if (!(*iter))
		{
			CLogMgr::GetInst()->AddLog(Log_Level::ERR, "Invaild UI Iterator");
			continue;
		}

		// 오브젝트에게서 UI 스크립트 가져오기
		CUIScript* pUI = (*iter)->GetScript<CUIScript>();

		// 만약 UI 스크립트가 없는 오브젝트일 경우
		if (!pUI)
		{
			// CLogMgr::GetInst()->AddLog(Log_Level::ERR, "UI Layer Contains Non - UI Objects");
			continue;
		}

		// 활성화 되지 않은 UI 타입일 경우
		if (!CUIMgr::GetInst()->IsActiveUIType(pUI->GetUIType()))
			continue;

		// 만약 마우스 상태를 체크하지 않는 UI일 경우
		if (!pUI->IsMouseInputEnabled())
		{
			continue;
		}

		if (pUI->m_bMouseOn)
		{
			m_FocusedObject = *iter;
			m_FocusedUI		= pUI;

			pUI = GetPriorityCheck(pUI->GetOwner());

			if (pUI->m_bMouseOn_Prev != pUI->m_bMouseOn)
			{
				pUI->OnHovered();
			}
			else
			{
				pUI->MouseOn();
			}

			if (bLbtnReleased)
			{
				pUI->LBtnUp();

				if (pUI->m_bMouseLBtnDown)
				{
					pUI->LBtnClicked();
				}
			}

			if (bLBtnTap)
			{
				pUI->LBtnDown();
				pUI->m_bMouseLBtnDown = true;

				// reverse iterator 로 vector 내에서 erase 하기
				std::advance(iter, 1);
				vecUI.erase(iter.base());

				vecUI.push_back(m_FocusedObject);
			}

			if (bLbtnReleased)
			{
				pUI->m_bMouseLBtnDown = false;
			}

			break;
		}
		else
		{
			if (pUI->m_bMouseOn_Prev != pUI->m_bMouseOn)
			{
				pUI->OnUnHovered();
			}

			if (bLbtnReleased)
			{
				pUI->m_bMouseLBtnDown = false;
			}
		}
	}

	// UI의 m_bMouseOn 멤버 조절에 사용될 좌표 계산
	CalWorldMousePos();
}

CUIScript* CUIMgr::GetPriorityCheck(CGameObject* _Parent)
{
	// 현재 구조: UIScript가 다른 UIScript를 들고 있을 것을 가정한 구조
	// 수정이 완료될 경우의 구조: 상위 오브젝트를 인자로 받아와서 자식 오브젝트에게 PriorityCheck를 해주어야 하는 구조.
	// 인자로 받아오는 것이 스크립트여도 괜찮은가? -> 괜찮다. GetOwner()함수로 가져올 수는 있다.
	// 하지만 구조 상 올바른 행동인지는 고민을 해보아야 한다.

	// GetScript() 할 때 비용이 조금 큰 느낌이 있는데...

	// 반환할 우선 순위 UI
	CUIScript* pPriorityUI = nullptr;

	// BFS - queue 사용
	static list<CGameObject*> q; // static으로 비용 아끼기
	q.clear();

	// 1. 인자로 받아은  UIScript에게서 Owner Object 가져오기
	q.push_back(_Parent);

	// 2. 오브젝트의 자식 오브젝트 vector 가져오며 BFS로 자식UI 끝까지 탐색
	while (!q.empty())
	{
		CGameObject* pObj = q.front();
		q.pop_front();

		auto& vecChildUI = pObj->GetChild();

		for (size_t i = 0; i < vecChildUI.size(); ++i)
		{
			q.push_back(vecChildUI[i]);
		}

		CUIScript* pUIScript = pObj->GetScript<CUIScript>();

		if (!pUIScript)
		{
			continue;
		}

		if (pUIScript->m_bMouseOn)
		{
			pPriorityUI = pUIScript;
		}
	}

	return pPriorityUI;
}

void CUIMgr::CalWorldMousePos()
{
#ifndef _RELEASE_GAME

	Vec2 vMousePos = CCamera::ViewportConvertFunc();

#else

	// 마우스 좌표를 UI카메라 기준 월드로 변환
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
#endif

	Vec2 vResol		   = CDevice::GetInst()->GetRenderResolution();
	m_vWorldMousePos.x = vMousePos.x - vResol.x / 2.f;
	m_vWorldMousePos.y = -vMousePos.y + vResol.y / 2.f;

	// CLogMgr::GetInst()->AddLog(Log_Level::INFO, std::to_wstring(m_vWorldMousePos.x) +
	// std::to_wstring(m_vWorldMousePos.y));
}
