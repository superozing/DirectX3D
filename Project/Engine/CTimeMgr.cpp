﻿#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_Time(0.f)
	, m_bLock(true)
	, m_szText{}
	, m_DTScale(1.f)
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	// 초당 빈도
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDeltaTime = m_DeltaTime = double(m_CurCount.QuadPart - m_PrevCount.QuadPart) / double(m_Frequency.QuadPart);

	if (m_bLock)
	{
		m_DeltaTime = 0.f;
	}
	else
	{
		m_DeltaTime *= m_DTScale;
	}

	m_PrevCount = m_CurCount;

	// DT 보정
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1. / 60.);

	// 시간 누적 ==> 1초마다 if 구문 실행
	m_Time += m_EngineDeltaTime;
	m_szText[70] = {};
	if (1.f <= m_Time)
	{
		swprintf_s(m_szText, 70, L"DeltaTime : %f[Scale: %f], FPS : %d", m_DeltaTime, m_DTScale, m_iCall);
		m_iCall = 0;
		m_Time	= 0.f;
	}

	++m_iCall;

	g_global.g_time += (float)m_DeltaTime;
	g_global.g_EngineDT = (float)m_EngineDeltaTime;
	g_global.g_EngineTime += (float)m_EngineDeltaTime;

	// 레벨 누적 시간 추가
	m_AccLevelTime += m_DeltaTime;
}

void CTimeMgr::exit()
{
	// 초당 빈도
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevCount);

	m_AccLevelTime = 0.;
}

void CTimeMgr::render()
{
	// 폰트 출력
	CFontMgr::GetInst()->DrawFont(m_szText, 10.f, 30.f, 16, FONT_RGBA(255, 30, 30, 255), FONT_TYPE::ARIAL);
}