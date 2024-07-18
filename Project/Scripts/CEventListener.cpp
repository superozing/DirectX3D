#include "pch.h"
#include "CEventListener.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>

CEventListener::CEventListener()
	: CScript((UINT)SCRIPT_TYPE::EVENTLISTENER)
	, m_bDrawing(true)
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDrawing);
}

CEventListener::~CEventListener()
{
}

void CEventListener::PushCallBack(CGameObject* _target, CScript* _receiver, Delegate_S _callback)
{
	m_vecEvent.push_back({_target, _receiver, _callback});
}

bool CEventListener::PushCallBack(const wstring& _detectorName, CGameObject* _target, CScript* _receiver,
								  Delegate_S _callback)
{
	// 이벤트 디텍팅 예제
	CGameObject* pEventLisnter = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(_detectorName);

	if (pEventLisnter)
	{
		CEventListener* pEventScript = pEventLisnter->GetScript<CEventListener>();
		if (pEventScript)
			pEventScript->PushCallBack(_target, _receiver, _callback);
		else
			return false;
	}
	else
	{
		return false;
	}

	return true;
}

void CEventListener::tick()
{
	if (m_bDrawing)
	{
		if (m_vecEvent.size() == 0)
			GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 1.f), false);
		else
			GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(1.f, 0.f, 1.f), false);
	}
}

void CEventListener::begin()
{
	if (!ComponentCheck())
		return;

	GetOwner()->DeleteComponent(COMPONENT_TYPE::MESHRENDER);
}

void CEventListener::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	for (auto iter = m_vecEvent.begin(); iter != m_vecEvent.end();)
	{
		if (iter->Target == _OtherObj)
		{
			auto data = *iter;

			if (data.Reciever && data.Callback)
				(data.Reciever->*data.Callback)();

			iter = m_vecEvent.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool CEventListener::ComponentCheck()
{
	wstring msg;
	msg += GetOwner()->GetName() + L"의 ";
	bool err = false;
	if (!Transform())
	{
		msg += L"Transform ";
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

void CEventListener::Example()
{
	// 이벤트 디텍팅 예제
	CGameObject* pEventLisnter = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"EventListner");
	CGameObject* pPlayer	   = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"azusa");
	if (pEventLisnter)
	{
		CEventListener* pEventScript = pEventLisnter->GetScript<CEventListener>();
		if (pEventScript)
			pEventScript->PushCallBack(pPlayer, this, (Delegate_S)&CEventListener::eventTestFunc);
	}
}

void CEventListener::eventTestFunc()
{
	CLogMgr::GetInst()->AddLog(Log_Level::WARN, L"Event Success");
}
