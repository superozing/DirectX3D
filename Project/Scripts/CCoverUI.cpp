#include "pch.h"
#include "CCoverUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CPlayerScript.h"
#include "CCoverArea.h"

CCoverUI::CCoverUI()
	: CEventListener((UINT)SCRIPT_TYPE::COVERUI)
{
}

CCoverUI::~CCoverUI()
{
}

void CCoverUI::DrawUI(bool _draw)
{
	// UI 띄우기
	for (auto& ui : m_vecUI)
		ui->PrintUI(_draw);
}

void CCoverUI::begin()
{
	CEventListener::begin();

	AddTarget(SCRIPT_TYPE::PLAYERSCRIPT);
	m_pPlayerScript = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();

	auto child = GetOwner()->GetParent()->GetChild();
	for (auto& obj : child)
	{
		auto script = obj->GetScript<CCoverArea>();
		if (script)
			m_vecUI.push_back(script);
	}

	Vec3 vOwnScale = Transform()->GetRelativeScale();
	Vec3 vScale	   = GetOwner()->GetParent()->Transform()->GetWorldScale();

	// Parent에 상대적이면서 절대적인 오프셋 조정이 필요
	vOwnScale.x = vScale.x + 100.f;
	vOwnScale.y = vScale.y;
	vOwnScale.z = vScale.z + 700.f;
	Transform()->SetRelativeScale(vOwnScale);
}

void CCoverUI::tick()
{
	CEventListener::tick();

	DrawUI(HasTargets());
}

void CCoverUI::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	CEventListener::BeginOverlap(_Collider, _OtherObj, _OtherCollider);

	DrawUI(HasTargets());
}

void CCoverUI::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	CEventListener::EndOverlap(_Collider, _OtherObj, _OtherCollider);

	DrawUI(HasTargets());
}
