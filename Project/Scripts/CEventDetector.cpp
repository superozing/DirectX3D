#include "pch.h"
#include "CEventDetector.h"

CEventDetector::CEventDetector()
	: CScript((UINT)SCRIPT_TYPE::EVENTDETECTOR)
{
}

CEventDetector::~CEventDetector()
{
}

void CEventDetector::PushCallBack(CScript* _script, void (CScript::*_delegate)())
{
	m_vecListners.push_back({_script, _delegate});
}

void CEventDetector::tick()
{
	GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 1.f), false);
}

void CEventDetector::begin()
{
	if (!ComponentCheck())
		return;
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