#include "pch.h"
#include "CFinishBalloon.h"

CFinishBalloon::CFinishBalloon()
	: CScript((UINT)SCRIPT_TYPE::FINISHBALLOON)
	, m_fUpTime(1.f)
	, m_fUpLength(100.f)
	, m_fWaveLength(100.f)
	, m_fWaveTime(1.f)
{
	AppendScriptParam("Up Length", SCRIPT_PARAM::FLOAT, &m_fUpLength);
	AppendScriptParam("Up Time", SCRIPT_PARAM::FLOAT, &m_fUpTime);
	AppendScriptParam("Wave Length", SCRIPT_PARAM::FLOAT, &m_fWaveLength);
	AppendScriptParam("Wave Time", SCRIPT_PARAM::FLOAT, &m_fWaveTime);
}

CFinishBalloon::~CFinishBalloon()
{
}

void CFinishBalloon::Up()
{
	m_bUp		   = true;
	m_fAccTime	   = 0.f;
	m_fWaveAccTime = m_fWaveTime;
	Vec3 vPos	   = Transform()->GetRelativePos();
	vPos.y += m_fUpLength;
	Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), m_fUpTime);
}

void CFinishBalloon::Wave()
{
	static int cnt = 0;

	m_fWaveAccTime += DT;
	if (m_fWaveAccTime >= m_fWaveTime)
	{
		Vec3 vPos = Transform()->GetRelativePos();

		if (cnt % 2 == 0)
		{
			vPos.y += m_fWaveLength;
		}
		else
		{
			vPos.y -= m_fWaveLength;
		}
		Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), m_fWaveTime);

		cnt++;
		m_fWaveAccTime = 0.f;
	}
}

void CFinishBalloon::begin()
{
	m_bUp = false;
}

void CFinishBalloon::tick()
{
	if (m_bUp)
	{
		m_fAccTime += DT;

		if (m_fAccTime >= m_fUpTime)
		{
			Wave();
		}
	}
}

#define TagUpLegnth "[UpLength]"
#define TagUpTime "[UpTime]"
#define TagWaveLength "[WaveLength]"

void CFinishBalloon::SaveToFile(ofstream& fout)
{
	fout << TagUpLegnth << endl;
	fout << m_fUpLength << endl;

	fout << TagUpTime << endl;
	fout << m_fUpTime << endl;

	fout << TagWaveLength << endl;
	fout << m_fWaveLength << endl;
}

void CFinishBalloon::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagUpLegnth);
	fin >> m_fUpLength;

	Utils::GetLineUntilString(fin, TagUpTime);
	fin >> m_fUpTime;

	Utils::GetLineUntilString(fin, TagWaveLength);
	fin >> m_fWaveLength;
}
