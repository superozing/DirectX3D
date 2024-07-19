#include "pch.h"
#include "CEventListener.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>

static string strNotation = "This Is EvenetListen Script";
CEventListener::CEventListener()
	: CScript((UINT)SCRIPT_TYPE::EVENTLISTENER)
	, m_bDrawing(true)
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDrawing);
	AppendScriptParam("", SCRIPT_PARAM::STRING, &strNotation);
}

CEventListener::~CEventListener()
{
}

void CEventListener::AddTarget(SCRIPT_TYPE _type)
{
	auto iter = find(m_vecTargets.begin(), m_vecTargets.end(), _type);
	if (iter == m_vecTargets.end())
		m_vecTargets.push_back(_type);
}

void CEventListener::SubTarget(SCRIPT_TYPE _type)
{
	auto iter = find(m_vecTargets.begin(), m_vecTargets.end(), _type);
	if (iter == m_vecTargets.end())
		m_vecTargets.erase(iter);
}

void CEventListener::tick()
{
	if (m_bDrawing)
	{
		if (HasTargets())
			GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 1.f), false);
		else
			GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(1.f, 1.f, 0.f), false);
	}
}

void CEventListener::begin()
{
	if (!ComponentCheck())
		return;

	if (MeshRender())
		GetOwner()->DeleteComponent(COMPONENT_TYPE::MESHRENDER);
}

void CEventListener::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	for (size_t i = 0; i < m_vecTargets.size(); i++)
	{
		auto scripts = _OtherObj->GetScripts();
		for (size_t j = 0; j < scripts.size(); j++)
		{
			if (scripts[j]->GetScriptType() == (UINT)m_vecTargets[i])
			{
				m_iInternalTargetCnt++;
				return;
			}
		}
	}
}

void CEventListener::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	for (size_t i = 0; i < m_vecTargets.size(); i++)
	{
		auto scripts = _OtherObj->GetScripts();
		for (size_t j = 0; j < scripts.size(); j++)
		{
			if (scripts[j]->GetScriptType() == (UINT)m_vecTargets[i])
			{
				m_iInternalTargetCnt--;
				return;
			}
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
