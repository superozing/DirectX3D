#include "pch.h"
#include "CLogMgr.h"

#include "CPathMgr.h"

#define TagLogInfo		"[Log_Level::INFO  ]"
#define TagLogWarn		"[Log_Level::WARN]"
#define TagLogError		"[Log_Level::ERROR]"


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
