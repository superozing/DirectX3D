﻿#include "pch.h"
#include "CLevelMgr.h"

#include "CRenderMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CCollisionMgr.h"
#include "CTimeMgr.h"
#include "CGC.h"

#include "CTaskMgr.h"

#include "CEngine.h"

#include "CKeyMgr.h"

#include "CMemoryPoolMgr.h"

Restart_Level	CLevelMgr::LevelChangeFunc = nullptr;
LevelLoad		CLevelMgr::LevelLoadFunc   = nullptr;
Save_CheckPoint CLevelMgr::CheckPointFunc  = nullptr;

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
	, m_strPrevLevelPath("")
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void CLevelMgr::init()
{
}

void CLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	// 이전 프레임에 등록된 오브젝트들 clear
	m_CurLevel->clear();

	// 레벨이 Play 상태일 경우에만 tick() 호출
	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->tick();
	}

	m_CurLevel->finaltick();

	// 충돌 처리
	CCollisionMgr::GetInst()->tick();

	// Render
	CRenderMgr::GetInst()->tick();
}

void CLevelMgr::enter()
{
	CMemoryPoolMgr::GetInst()->enter();
}

void CLevelMgr::exit()
{
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	tTask task = {};

	task.Type	 = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_1 = (DWORD_PTR)m_CurLevel;
	task.Param_2 = (DWORD_PTR)_State;

	CTaskMgr::GetInst()->AddTask(task);
}

#include "CPhysXMgr.h"
void CLevelMgr::ChangeLevel_Task(CLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	assert(!(m_CurLevel == _NextLevel));

	CLevelMgr::GetInst()->exit();
	CRenderMgr::GetInst()->exit();
	CTimeMgr::GetInst()->exit();
	CAssetMgr::GetInst()->exit();
	CCollisionMgr::GetInst()->exit();
	CGC::GetInst()->exit();
	CPhysXMgr::GetInst()->exit();
	CMemoryPoolMgr::GetInst()->exit();

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	if (nullptr != m_CurLevel)
		m_CurLevel->ChangeState(_NextLevelState);

	CLevelMgr::GetInst()->enter();
	m_MapClientFunc[Client_Function_Type::CIMGUIMGR_ENTER]();
}

void CLevelMgr::RegisterClientFunction(Client_Function_Type _Type, std::function<void()> _MemberFunction)
{
	m_MapClientFunc[_Type] = _MemberFunction;
}
