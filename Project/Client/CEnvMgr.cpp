﻿#include "pch.h"
#include "CEnvMgr.h"

#include <Engine/CPathMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CKeyMgr.h>
#include <Engine\CTaskMgr.h>
#include "CLevelSaveLoad.h"

#include <Engine\CMemoryPoolMgr.h>

#define TagResolution "[Resolution(x, y), FullScreen]"
#define TagLevel "[Level]"

CEnvMgr::CEnvMgr()
{
}

CEnvMgr::~CEnvMgr()
{
}

void CEnvMgr::init()
{
	wstring strEnvLoadPath = CPathMgr::GetLogPath();
	strEnvLoadPath += L"\\Env.txt";

	// env 파일이 없다면 예외
	if (!exists(strEnvLoadPath))
	{
		return;
	}

	ifstream fin(strEnvLoadPath);

	if (!fin.is_open())
		MessageBox(nullptr, L"Env Load Fail", L"Client 초기화 에러", 0);

	// 해상도 설정 로드
	Vec2 res;
	bool window;
	Utils::GetLineUntilString(fin, TagResolution);
	fin >> res.x >> res.y >> window;
	m_tRes.res	  = res;
	m_tRes.window = window;

	// 마지막 레벨 상대경로 로드
	string levelName;
	Utils::GetLineUntilString(fin, TagLevel);
	fin >> levelName;
	m_strLevelRelativePath = levelName;
}

void CEnvMgr::exit()
{
	wstring strEnvSavePath = CPathMgr::GetLogPath();
	strEnvSavePath += L"\\Env.txt";

	// Log 폴더가 없다면 폴더 생성
	if (!exists(CPathMgr::GetLogPath()))
	{
		create_directories(CPathMgr::GetLogPath());
	}

	ofstream fout(strEnvSavePath, ofstream::out | ofstream::trunc);

	if (!fout.is_open())
	{
		MessageBox(nullptr, L"Env Save fail!", L"Client 종료 에러", 0);
	}

	Vec2 res	= CDevice::GetInst()->GetRenderResolution();
	bool window = true;
	// bool full = CDevice::GetInst()->getfullscreen();
	fout << TagResolution << endl;
	fout << res.x << " " << res.y << " " << window << endl;

	fout << TagLevel << endl;
	fout << ToString(CLevelMgr::GetInst()->GetCurrentLevel()->GetRelativePath()) << endl;

	fout.close();

	CMemoryPoolMgr::GetInst()->SavePool();
}

void CEnvMgr::tick()
{
	if (KEY_PRESSED(KEY::LSHIFT) && KEY_TAP(KEY::F5) &&
		CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
	{
		tTask task;
		task.Type = TASK_TYPE::CHANGE_LEVEL;

		CLevel* TransitionLv = CLevelSaveLoad::LoadLevel(LEVELLevelTransition);
		task.Param_1		 = (UINT_PTR)TransitionLv;
		task.Param_2		 = (UINT_PTR)LEVEL_STATE::PLAY;

		CTaskMgr::GetInst()->AddTask(task);
	}

	if (KEY_PRESSED(KEY::LSHIFT) && KEY_TAP(KEY::F6))
	{
		CLevel* Curlevel = CLevelMgr::GetInst()->GetCurrentLevel();

		tTask task;
		task.Type	 = TASK_TYPE::SAVE_CHECKPOINT;
		task.Param_1 = (UINT_PTR)Curlevel;

		CTaskMgr::GetInst()->AddTask(task);
	}
}
