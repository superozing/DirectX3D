#include "pch.h"
#include "CLogMgr.h"

CLogMgr::CLogMgr()
{

}

CLogMgr::~CLogMgr()
{
}

void CLogMgr::init()
{
}

void CLogMgr::tick()
{
}

void CLogMgr::enter()
{
}

void CLogMgr::exit()
{
}

void CLogMgr::AddLog(Log_Level Lv, string msg)
{
	tLog Log;
	
	Log.m_LogLv = Lv;
	Log.m_strMsg = msg;


	m_vecLog.push_back(Log);
}

void CLogMgr::CopyLog()
{
}

void CLogMgr::ClearLog()
{
	vector<tLog> ChnageVectoer;

	m_vecLog.swap(ChnageVectoer);
}
