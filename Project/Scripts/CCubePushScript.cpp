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

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	Vec2 v2ColPos1 = Vec2(ThisColPos.x, ThisColPos.z);
	Vec2 v2ColPos2 = v2ColPos1 + ThisColScale.z / 2.f * Vec2(vFront.x, vFront.z);
	Vec2 v2ColPos3 = v2ColPos1 + ThisColScale.x / 2.f * Vec2(vRight.x, vRight.z);

	// float overlapAdvanceX = distancePointToSegment(Vec2(PlayerColPos.x, PlayerColPos.z), v2ColPos1, v2ColPos2);
	// float overlapAdvanceZ = distancePointToSegment(Vec2(PlayerColPos.x, PlayerColPos.z), v2ColPos1, v2ColPos3);
	Vec2  v2PlayerPos	  = Vec2(PlayerColPos.x, PlayerColPos.z);
	float wallX			  = distancePointToLineDirection(Vec2(vFront.x, vFront.z), v2PlayerPos - v2ColPos1);
	float wallZ			  = distancePointToLineDirection(Vec2(vRight.x, vRight.z), v2PlayerPos - v2ColPos1);
	float overlapAdvanceX = (PlayerScale.x / 2 + ThisColScale.x / 2) - std::fabs(wallX);
	float overlapAdvanceZ = (PlayerScale.x / 2 + ThisColScale.z / 2) - fabs(wallZ);
	// 간섭 거리 계산
	float overlapX = (PlayerScale.x / 2 + ThisColScale.x / 2) - std::fabs(toWall.x);
	float overlapZ = (PlayerScale.z / 2 + ThisColScale.z / 2) - std::fabs(toWall.z);

	// 가장 큰 간섭 거리와 방향 선택
	float overlap = min(overlapAdvanceX, overlapAdvanceZ);

	// 법선 방향 계산 (간섭이 가장 큰 방향)
	Vec3 pushDirection;
	if (std::fabs(overlapAdvanceX) <= std::fabs(overlapAdvanceZ))
	{
		pushDirection = (wallZ > 0) ? vRight : -vRight; // X축 방향
	}
	else
	{
		pushDirection = (wallX > 0) ? vFront : -vFront; // Z축 방향
	}

	// 밀어줄 오프셋 계산 (간섭 거리와 방향을 고려)
	offset = pushDirection * overlap;
	// CLogMgr::GetInst()->AddLog(Log_Level::INFO, offset.ToString());
	CLogMgr::GetInst()->AddLog(Log_Level::INFO, to_string(wallX) + " " + to_string(wallZ));
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

float CCubePushScript::distancePointToSegment(const Vec2& point, const Vec2& segmentStart, const Vec2& segmentEnd)
{
	{
		Vec2 segment = segmentEnd - segmentStart;
		Vec2 toPoint = point - segmentStart;

		float segmentLengthSquared = segment.Dot(segment);
		if (segmentLengthSquared == 0)
		{
			// 선분의 시작점과 끝점이 동일할 경우
			return toPoint.Length();
		}

		float t = toPoint.Dot(segment) / segmentLengthSquared;
		t		= max(0.0f, min(1.0f, t)); // t를 [0, 1] 범위로 클램핑

		Vec2 closestPoint	= {segmentStart.x + t * segment.x, segmentStart.y + t * segment.y};
		Vec2 distanceVector = point - closestPoint;

		return distanceVector.Length();
	}
}

float CCubePushScript::distancePointToLineDirection(Vec2 D, Vec2 P)
{
	// 방향 벡터 D의 길이 제곱
	float D_length_squared = D.x * D.x + D.y * D.y;

	// D가 0인 경우 (유효하지 않은 방향 벡터)
	if (D_length_squared == 0)
	{
		return std::sqrt(P.x * P.x + P.y * P.y); // 원점 (0, 0)에서의 거리
	}

	// P를 D 방향으로 투영하는 스칼라 t 계산
	float t = (P.x * D.x + P.y * D.y) / D_length_squared;

	// P와 D 방향으로의 가장 가까운 점 계산
	Vec2 closest_point = {t * D.x, t * D.y};

	// P와 가장 가까운 점 사이의 거리 계산
	float distance = std::sqrt((P.x - closest_point.x) * (P.x - closest_point.x) +
							   (P.y - closest_point.y) * (P.y - closest_point.y));

	return t > 0 ? distance : -distance;
}
