#include "pch.h"
#include "CEventDetector.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>

CEventDetector::CEventDetector()
	: CScript((UINT)SCRIPT_TYPE::EVENTDETECTOR)
{
}

CEventDetector::~CEventDetector()
{
}

void CEventDetector::PushCallBack(CScript* _script, Delegate_S _delegate)
{
	m_vecListners.push_back({_script, _delegate});
}

bool CEventDetector::PushCallBack(const wstring& _detectorName, CScript* _script, Delegate_S _delegate)
{
	// 이벤트 디텍팅 예제
	CGameObject* pEventLisnter = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(_detectorName);

	if (pEventLisnter)
	{
		CEventDetector* pEventScript = pEventLisnter->GetScript<CEventDetector>();
		if (pEventScript)
			pEventScript->PushCallBack(_script, _delegate);
		else
			return false;
	}
	else
	{
		return false;
	}

	return true;
}

void CEventDetector::tick()
{
	GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 1.f), false);
}

void CEventDetector::begin()
{
	if (!ComponentCheck())
		return;

	// Example();
}

void CEventDetector::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	for (auto& data : m_vecListners)
	{
		if (data.m_pLisnter && data.Delegate)
			(data.m_pLisnter->*data.Delegate)();
	}
	m_vecListners.clear();
}

bool CEventDetector::ComponentCheck()
{
	wstring msg;
	msg += GetOwner()->GetName() + L"의 ";
	bool err = false;
	if (!Transform())
	{
		msg += L"Transform ";
		err = true;
	}
	if (!MeshRender())
	{
		msg += L"MeshRender ";
		err = true;
	}
	if (!PhysX())
	{
		msg += L"PhysX ";
		err = true;
	}
	if (err)
	{
		msg += L"컴포넌트가 없습니다.";
		MessageBox(nullptr, msg.c_str(), L"EventDetector 오류", 0);

		return false;
	}
	return true;
}

void CEventDetector::Example()
{
	// 이벤트 디텍팅 예제
	CGameObject* pEventLisnter = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"EventListner");

	if (pEventLisnter)
	{
		CEventDetector* pEventScript = pEventLisnter->GetScript<CEventDetector>();
		if (pEventScript)
			pEventScript->PushCallBack(this, (Delegate_S)&CEventDetector::eventTestFunc);
	}
}

void CEventDetector::eventTestFunc()
{
	CLogMgr::GetInst()->AddLog(Log_Level::WARN, L"Event Success");
}
