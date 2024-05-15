#include "pch.h"
#include "CLogMgr.h"

#include "CPathMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CTimeMgr.h"


#define TagLogInfo		"[Log_Level::INFO  ]"
#define TagLogWarn		"[Log_Level::WARN]"
#define TagLogError		"[Log_Level::ERROR]"

#define TagOpen			"["
#define TagTimedevide	":"
#define TagClose		"}"


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

void CLogMgr::AddLog(Log_Level Lv, wstring msg)
{
	string Msg = ToString(msg);
	AddLog(Lv, Msg);
}

void CLogMgr::AddLog(tLog Log)
{
	m_vecLog.push_back(Log);
}

void CLogMgr::AddTimeLog(Log_Level Lv, string msg)
{
	tLog Log;

	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
	{
		Log.m_fMin = 00.0f;
		Log.m_fsec = 00.0f;
	}
	else
	{
		float time = g_global.g_time;

		Log.m_fMin = (int)time / 60.f;
		Log.m_fsec = (int)time % 60;

	}

	string time = TagOpen + std::to_string(Log.m_fMin) + TagTimedevide + std::to_string(Log.m_fsec) + TagClose;

	Log.m_LogLv = Lv;
	Log.m_strMsg = time + msg;

	AddLog(Log);
}

void CLogMgr::AddTimeLog(Log_Level Lv, wstring msg)
{
	tLog Log;

	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
	{
		Log.m_fMin = 00.0f;
		Log.m_fsec = 00.0f;
	}
	else
	{
		float time = g_global.g_time;

		Log.m_fMin = (int)time / 60.f;
		Log.m_fsec = (int)time % 60;

	}

	string time = TagOpen + std::to_string(Log.m_fMin) + TagTimedevide + std::to_string(Log.m_fsec) + TagClose;
	string Msg = ToString(msg);

	Log.m_LogLv = Lv;
	Log.m_strMsg = time + Msg;

	AddLog(Log);
}

void CLogMgr::CopyLog()
{
	wstring strLogSavepath =CPathMgr::GetContentPath();
	strLogSavepath += L"\\Log.txt";

	ofstream fout(strLogSavepath, ofstream::out | ofstream::trunc);

	if (!fout.is_open()) {
		MessageBox(nullptr, L"Log copy fail!", L"Log copy fail!", 0);
	}

	fout << "[Log]" << endl;
	fout << endl;

	for (int i = 0; i < m_vecLog.size(); ++i)
	{
		string Log;

		switch (m_vecLog[i].m_LogLv)
		{
		case Log_Level::INFO:
			Log += TagLogInfo;
			break;
		case Log_Level::WARN:
			Log += TagLogWarn;
			break;
			case Log_Level::ERR:
			Log += TagLogError;
			break;
		default:
			break;
		}

		Log += "  " + m_vecLog[i].m_strMsg;

		fout << Log << endl;
	}
}

void CLogMgr::ClearLog()
{
	vector<tLog> ChnageVectoer;

	m_vecLog.swap(ChnageVectoer);
}
