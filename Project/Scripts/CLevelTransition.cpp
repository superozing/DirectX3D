#include "pch.h"
#include "CLevelTransition.h"

#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CTaskMgr.h>

CLevelTransition::CLevelTransition()
	: CScript((UINT)SCRIPT_TYPE::LEVELTRANSITION)
{
}

CLevelTransition::~CLevelTransition()
{
}

void CLevelTransition::begin()
{
}

#include <Engine\CLogMgr.h>

void CLevelTransition::tick()
{
	if (CLevelMgr::GetInst()->GetCurrentLevel()) //!= CLevelMgr::GetInst()->GetPrevLevel())
	{
		m_ChangeLevelTask.Type = TASK_TYPE::CHANGE_LEVEL;

		string strPrevLevel;

		wstring CheckPointPath = CPathMgr::GetContentPath();
		CheckPointPath += L"level\\CheckPoint.lv";

		ifstream CheckPointLv(CheckPointPath);

		if (CheckPointLv.is_open())
		{
			strPrevLevel = "level\\CheckPoint.lv";
		}
		else
		{
			strPrevLevel = CLevelMgr::GetInst()->GetstrPrevLevelPath();
		}

		auto	pLevel	  = CLevelMgr::GetInst()->LevelChangeFunc(strPrevLevel);
		wstring levelname = path(strPrevLevel).stem().wstring();
		pLevel->SetName(levelname);

		m_ChangeLevelTask.Param_1 = (UINT_PTR)pLevel;
		m_ChangeLevelTask.Param_2 = (UINT_PTR)LEVEL_STATE::PLAY;

		CTaskMgr::GetInst()->AddTask(m_ChangeLevelTask);
	}
}
