#include "pch.h"
#include "CUIMgr.h"

#include "CKeyMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CLogMgr.h"

#include "CUIScript.h"
#include "CGameObject.h"

void CUIMgr::tick()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	bool bLBtnTap = KEY_TAP(KEY::LBTN);
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
	auto iter = vecUI.rbegin();

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
			CLogMgr::GetInst()->AddLog(Log_Level::ERR, "UI Layer Contains Non - UI Objects");
			continue;
		}


		if (pUI->m_bMouseOn)
		{
			m_FocusedObject = *iter;
			m_FocusedUI = pUI;

			pUI = GetPriorityCheck(pUI);

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
}

CUIScript* CUIMgr::GetPriorityCheck(CUIScript* _ParentUI)
{
	CUIScript* pPriorityUI = nullptr;

	static list<CUIScript*> queue;
	queue.clear();

	queue.push_back(_ParentUI);

	while (!queue.empty())
	{
		CUIScript* pUI = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pUI->m_vecChildUI.size(); ++i)
		{
			queue.push_back(pUI->m_vecChildUI[i]);
		}

		if (pUI->m_bMouseOn)
		{
			pPriorityUI = pUI;
		}
	}

	return pPriorityUI;
}