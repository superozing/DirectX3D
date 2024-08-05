#include "pch.h"
#include "CCoverArea.h"

#include "CPlayerController.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CImageUIScript.h"
#include "CCoverUI.h"

CCoverArea::CCoverArea()
	: CEventListener((UINT)SCRIPT_TYPE::COVERAREA)
	, m_bStand(true)
{
	AppendScriptParam("IsStand", SCRIPT_PARAM::BOOL, &m_bStand);
}

CCoverArea::~CCoverArea()
{
}

void CCoverArea::SetPlayerCover(bool _bRight)
{
	Vec3 vPos	= Transform()->GetWorldPos();
	Vec3 vDir	= Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vScale = Transform()->GetWorldScale();

	Vec3 vPlayerScale = m_pPlayer->Transform()->GetRelativeScale();

	vPos.y -= m_pPlayer->Transform()->GetRelativeScale().y / 2.f;

	bool bPlayerRight = m_bRight = _bRight;

	if (bPlayerRight)
	{
		vPos += vDir * (vScale.x / 2.f - abs(vPlayerScale.x) / 2.f);
	}
	else
	{
		vPos -= vDir * (vScale.x / 2.f - abs(vPlayerScale.x) / 2.f);
	}

	m_bStand ? m_pScript->SetCoverType(CoverType::Stand) : m_pScript->SetCoverType(CoverType::Kneel);

	m_pPlayer->Transform()->SetRelativePos(vPos);
	m_pPlayer->Transform()->SetDir(Transform()->GetWorldDir(DIR_TYPE::FRONT));
	m_pPlayer->Transform()->SetRelativeScale(vPlayerScale);

	m_bPlayerCover = true;

	// Area들 UI 제거
	m_UIMgr->DrawUI(false);
}

void CCoverArea::PrintUI(bool _print)
{
	if (m_pUI)
		m_pUI->Draw(_print);
}

void CCoverArea::begin()
{
	CEventListener::begin();

	m_pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	m_pScript = m_pPlayer->GetScript<CPlayerScript>();

	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
		const auto& script = child->GetScript<CImageUIScript>();
		if (script)
			m_pUI = script;
	}

	const auto& pChilds = GetOwner()->GetParent()->GetChild();
	for (const auto& pChild : pChilds)
	{
		const auto& script = pChild->GetScript<CCoverUI>();
		if (script)
			m_UIMgr = script;
	}

	if (m_pUI)
	{
		Vec3 vPos = Vec3();
		vPos.y += 50.f;
		vPos.z += m_pPlayer->Transform()->GetRelativeScale().z / 2.f;
		m_pUI->Transform()->SetRelativePos(vPos);
	}
}

void CCoverArea::tick()
{
	CEventListener::tick();

	if (KEY_TAP(CPlayerController::Flip) && m_bPlayerCover)
	{
		SetPlayerCover(!m_bRight);
	}

	if (KEY_TAP(CPlayerController::Cover) && m_bPlayerCover)
	{
		m_pScript->SetCoverType(CoverType::Normal);
		m_bPlayerCover = false;

		// Area들 UI 그리기
		m_UIMgr->DrawUI(true);
	}
	else if (HasTargets() && KEY_TAP(CPlayerController::Cover) && !m_bPlayerCover)
	{
		SetPlayerCover(m_pScript->IsRight());
	}
}

#define TagIsRight "[IsRight]"
#define TagIsStand "[IsStand]"

void CCoverArea::SaveToFile(ofstream& fout)
{
	CEventListener::SaveToFile(fout);

	fout << TagIsStand << endl;
	fout << m_bStand << endl;
}

void CCoverArea::LoadFromFile(ifstream& fin)
{
	CEventListener::LoadFromFile(fin);

	Utils::GetLineUntilString(fin, TagIsStand);
	fin >> m_bStand;
}
