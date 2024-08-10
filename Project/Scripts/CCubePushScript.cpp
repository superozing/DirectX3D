#include "pch.h"
#include "CCubePushScript.h"
#include "Engine\CPhysX.h"
#include "Engine\CTransform.h"
#include "Engine\CLogMgr.h"

void CCubePushScript::resolveCollision(CGameObject* _Player)
{
	auto ThisColPos	  = PhysX()->GetColliderPos();
	auto PlayerColPos = _Player->PhysX()->GetColliderPos();

	auto ThisColScale = PhysX()->m_vScale;
	auto PlayerScale  = _Player->PhysX()->m_vScale;

	auto ThisColRot	  = Transform()->GetWorldRot();
	auto PlayerColRot = _Player->Transform()->GetWorldRot();

	// 밀어줄 Offset
	Vec3 offset;
	// ####################################################################################################
	//  TODO : 밀어줄 오프셋을 계산
	// 벽과 플레이어의 중심에서의 거리 계산
	Vec3 toWall = ThisColPos - PlayerColPos;

	// 간섭 거리 계산
	float overlapX = (PlayerScale.x / 2 + ThisColScale.x / 2) - std::fabs(toWall.x);
	float overlapY = (PlayerScale.y / 2 + ThisColScale.y / 2) - std::fabs(toWall.y);
	float overlapZ = (PlayerScale.z / 2 + ThisColScale.z / 2) - std::fabs(toWall.z);

	// 가장 큰 간섭 거리와 방향 선택
	float overlap = min(min(overlapX, overlapY), overlapZ);

	// 법선 방향 계산 (간섭이 가장 큰 방향)
	Vec3 pushDirection;
	if (std::fabs(overlapX) <= std::fabs(overlapY) && std::fabs(overlapX) <= std::fabs(overlapZ))
	{
		pushDirection = (toWall.x > 0) ? Vec3(-1, 0, 0) : Vec3(1, 0, 0); // X축 방향
	}
	else if (std::fabs(overlapY) <= std::fabs(overlapZ))
	{
		pushDirection = (toWall.y > 0) ? Vec3(0, -1, 0) : Vec3(0, 1, 0); // Y축 방향
	}
	else
	{
		pushDirection = (toWall.z > 0) ? Vec3(0, 0, -1) : Vec3(0, 0, 1); // Z축 방향
	}

	// 밀어줄 오프셋 계산 (간섭 거리와 방향을 고려)
	offset = pushDirection * overlap;
	CLogMgr::GetInst()->AddLog(Log_Level::INFO, offset.ToString());
	// ####################################################################################################

	auto PlayerPos = _Player->Transform()->GetRelativePos();
	_Player->Transform()->SetRelativePos(PlayerPos + offset);
}

void CCubePushScript::begin()
{
}

void CCubePushScript::tick()
{
}

void CCubePushScript::Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	bool bIsPlayer = false;

	auto vecScripts = _OtherObj->GetScripts();
	// 플레이어인지 확인
	for (auto& e : vecScripts)
	{
		if ((UINT)SCRIPT_TYPE::PLAYERSCRIPT == e->GetScriptType())
		{
			bIsPlayer = true;
		}
	}
	if (false == bIsPlayer)
		return;
	resolveCollision(_OtherObj);
}

void CCubePushScript::SaveToFile(ofstream& fout)
{
}

void CCubePushScript::LoadFromFile(ifstream& fin)
{
}

CCubePushScript::CCubePushScript()
	: CScript((UINT)SCRIPT_TYPE::CUBEPUSHSCRIPT)
{
}

CCubePushScript::~CCubePushScript()
{
}
