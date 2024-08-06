#include "pch.h"
#include "CTutorialTarget.h"

#include <Engine\CRandomMgr.h>

CTutorialTarget::CTutorialTarget()
	: CScript((UINT)SCRIPT_TYPE::TUTORIALTARGET)
	, m_bMoveRight(true)
	, m_fMoveLength(1000.f)
	, m_fMoveTime(1.f)
{
	AppendScriptParam("GroundType", SCRIPT_PARAM::BOOL, &m_bGroundType);
}

CTutorialTarget::~CTutorialTarget()
{
}

void CTutorialTarget::begin()
{
	Hit(false);

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	vDir	  = m_bMoveRight ? vDir : -vDir;
	vPos += vDir * m_fMoveLength / 2.f;
	Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), m_fMoveTime);
	m_fAccTime = 0.f;
}

void CTutorialTarget::tick()
{
	if (m_bGroundType)
	{
		if (m_fAccTime >= m_fMoveTime)
		{
			m_bMoveRight = !m_bMoveRight;
			m_fMoveTime	 = CRandomMgr::GetInst()->GetRandomFloat(1.5f, 3.f);

			Vec3 vPos = Transform()->GetRelativePos();
			Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vDir	  = m_bMoveRight ? vDir : -vDir;

			vPos += vDir * m_fMoveLength;
			Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), m_fMoveTime);
			m_fAccTime = 0.f;
		}
		m_fAccTime += DT;
	}
}

#define TagType "[Type]"

void CTutorialTarget::SaveToFile(ofstream& fout)
{
	fout << TagType << endl;
	fout << m_bGroundType << endl;
}

void CTutorialTarget::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagType);
	fin >> m_bGroundType;
}
