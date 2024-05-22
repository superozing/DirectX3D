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

	Log.m_dTime = CTimeMgr::GetInst()->GetAccLevelTime();

	m_vecLog.push_back(Log);
}

void CLogMgr::AddLog(Log_Level Lv, wstring msg)
{
	string Msg = ToString(msg);
	AddLog(Lv, Msg);
}

void CLogMgr::AddLog(tLog Log)
{
	Log.m_dTime = CTimeMgr::GetInst()->GetAccLevelTime();
	m_vecLog.push_back(Log);
}

string CLogMgr::GetTimeMsg(double time)
{
	string strTime;

	double iTotalSec = time;;
	int iMinute = iTotalSec / (int)60;
	int iSecond = (int)(iTotalSec - (iMinute * 60)) % 60;
	double iMSecond = floor((iTotalSec -(iMinute * 60 + iSecond)) * 1000);


	strTime = "[M:" + std::to_string(iMinute) + " S:" + std::to_string(iSecond) + " MS:" + std::to_string((int)iMSecond)+ "]";

	return strTime;
}

void CLogMgr::CopyLog()
{
	wstring strLogSavepath =CPathMgr::GetLogPath();
	strLogSavepath += L"\\Log.txt";

	//Log 폴더가 없다면 폴더 생성
	if (!std::filesystem::exists(CPathMgr::GetLogPath()))
	{
		std::filesystem::create_directories(CPathMgr::GetLogPath());
	}

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

		Log += " " + GetTimeMsg(m_vecLog[i].m_dTime);

		Log += "  " + m_vecLog[i].m_strMsg;

		fout << Log << endl;
	}
}

void CLogMgr::ClearLog()
{
	m_vecLog.clear();
}
