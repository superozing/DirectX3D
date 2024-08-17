#include "pch.h"
#include "CPhysX.h"

#include "CTransform.h"
#include <DirectXMath.h>
#include "CPhysXMgr.h"
#include "CScript.h"

CPhysX::CPhysX()
	: CComponent(COMPONENT_TYPE::PHYSX)
{
}

CPhysX::~CPhysX()
{
	releaseActor();
}

void CPhysX::setLinearVelocity(const Vec3& _vLVel)
{
	if (nullptr == m_DActor)
		return;
	m_DActor->setLinearVelocity(PxVec3(_vLVel.x, _vLVel.y, _vLVel.z));
}

void CPhysX::setAngularVelocity(const Vec3& _vAVel)
{
	if (nullptr == m_DActor)
		return;
	m_DActor->setAngularVelocity(PxVec3(_vAVel.x, _vAVel.y, -_vAVel.z));
}

void CPhysX::applyBulletImpact(const PxVec3& bulletVelocity, float bulletMass, const PxVec3& hitPoint)
{
	if (nullptr == m_DActor)
		return;

	// 충격량 계산
	PxVec3 impulse = bulletMass * bulletVelocity;

	// 물체의 중심
	PxVec3 actorCenter = m_DActor->getGlobalPose().p * CPhysXMgr::GetInst()->m_PPM;

	// 충돌 지점과 중심 사이의 벡터
	PxVec3 r = hitPoint - actorCenter;

	// 충돌 지점이 중심에서 멀어질수록 충격량 감소 비율 계산
	float distance			= r.magnitude();
	float attenuationFactor = 1.0f / (1.0f + distance); // 거리 증가에 따른 충격량 감소 비율

	// 충격량 감소 적용
	PxVec3 adjustedImpulse = impulse * attenuationFactor;

	// 충격량의 방향 변화 고려
	PxVec3 impulseDirection = adjustedImpulse.getNormalized();
	adjustedImpulse			= impulseDirection * adjustedImpulse.magnitude();

	// 토크 계산 (외적)
	PxVec3 torque = r.cross(adjustedImpulse);

	// 토크 감소 비율 적용
	float torqueAttenuationFactor = 1.0f / (1.0f + distance * distance);
	torque *= torqueAttenuationFactor;

	// 물체에 충격량 적용
	m_DActor->addForce(adjustedImpulse, PxForceMode::eIMPULSE);

	// 물체에 토크 적용
	m_DActor->addTorque(torque, PxForceMode::eIMPULSE);
}

void CPhysX::releaseActor()
{
	if (nullptr == m_Actor)
		return;

	CPhysXMgr::GetInst()->ReleaseActor(m_Actor);
	// 자신과 충돌중이던 오브젝트들 endoverlap호출
	for (auto& e : m_vThisFrameContact)
	{
		if (e.Other->PhysX())
		{
			e.Other->PhysX()->EndOverlap(GetOwner());
		}
	}
}

Vec3 CPhysX::GetColliderPos()
{
	Vec3 ColliderPos;

	auto ObjPos	  = Transform()->GetWorldPos();
	auto worldrot = Transform()->GetWorldRot();

	auto RotatedOffset = RoRMath::RotateVectorByRotationVector(m_vOffsetPos, worldrot);
	ColliderPos		   = ObjPos;
	ColliderPos += RotatedOffset;

	return ColliderPos;
}

Vec3 CPhysX::ConvertToObjectPos(Vec3 _ColPos)
{
	Vec3 ObjectPos;

	auto worldrot = Transform()->GetWorldRot();

	auto RotatedOffset = RoRMath::RotateVectorByRotationVector(m_vOffsetPos, worldrot);
	ObjectPos		   = _ColPos;
	ObjectPos -= RotatedOffset;

	return ObjectPos;
}

void CPhysX::updateFromPhysics()
{
	if (nullptr == m_Actor)
		return;
}

void CPhysX::updateToPhysics()
{
	if (nullptr == m_Actor)
		return;

	Quat quaternion = Transform()->GetWorldQuaternion();

	Vec3 ColPos = GetColliderPos();
	ColPos /= CPhysXMgr::GetInst()->m_PPM;

	// 게임 오브젝트의 위치와 회전 정보
	PxTransform transform(PxVec3(ColPos.x, ColPos.y, ColPos.z),
						  PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));

	m_Actor->setGlobalPose(transform);
}

void CPhysX::begin()
{
	CPhysXMgr::GetInst()->addGameObject(GetOwner());
}

// #include "CLogMgr.h"
void CPhysX::finaltick()
{
	// imgui가 포커스되어있으면 ,현재프레임에 충돌한 오브젝트정보를 수집
	if (true == m_bImguiDirtyFlag)
	{
		m_bThisFrameImguiFocused = true;
	}
	else
	{
		m_bThisFrameImguiFocused = false;
	}
	m_bImguiDirtyFlag = false;

	if (PhysBodyType::DYNAMIC != m_bPhysBodyType)
	{
		updateToPhysics();
	}
	else
	{
		// updateFromPhysics();
	}

	auto Rot		 = Transform()->GetWorldQuaternion();
	auto ColPos		 = GetColliderPos();
	auto scale		 = m_vScale != Vec3() ? m_vScale : Transform()->GetWorldScale();
	auto vDebugColor = m_CollisionCount > 0 ? Vec4(1.f, 0.f, 0.f, 1.f) : Vec4(0.f, 1.f, 0.f, 1.f);

	if (m_bDrawing)
	{
		if (PhysShape::BOX == m_Shape)
		{
			GamePlayStatic::DrawDebugCube(ColPos, scale * 1.1, Vec4(Rot.x, Rot.y, Rot.z, Rot.w), vDebugColor, true);
		}
		else if (PhysShape::SPHERE == m_Shape)
		{
			GamePlayStatic::DrawDebugSphere(ColPos, scale.x / 1.8f, vDebugColor, true);
		}
		else if (PhysShape::CONE == m_Shape)
		{
			// GamePlayStatic::DrawDebugCone(ColPos, scale, Vec4(Rot.x, Rot.y, Rot.z, Rot.w), Vec3(0.3f, .3f, 0.3f),
			// false);
		}
	}
}

#define TagBodyType "[IsStatic]"
#define TagShape "[Shape]"
#define TagScale "[Scale]"
#define TagOffsetPos "[OffsetPos]"
void CPhysX::SaveToFile(FILE* _File)
{
}

void CPhysX::SaveToFile(ofstream& fout)
{
	fout << TagBodyType << endl;
	fout << magic_enum::enum_name<PhysBodyType>(m_bPhysBodyType) << endl;

	fout << TagShape << endl;
	auto shape = magic_enum::enum_name<PhysShape>(m_Shape);
	fout << ToString(shape) << endl;

	fout << TagScale << endl;
	fout << m_vScale << endl;

	fout << TagOffsetPos << endl;
	fout << m_vOffsetPos << endl;
}

void CPhysX::LoadFromFile(FILE* _File)
{
}

void CPhysX::LoadFromFile(ifstream& fin)
{
	string tag, str;
	Utils::GetLineUntilString(fin, TagBodyType);
	getline(fin, str);
	m_bPhysBodyType = magic_enum::enum_cast<PhysBodyType>(str).value();

	Utils::GetLineUntilString(fin, TagShape);
	getline(fin, str);
	m_Shape = magic_enum::enum_cast<PhysShape>(str).value();

	Utils::GetLineUntilString(fin, TagScale);
	fin >> m_vScale;

	Utils::GetLineUntilString(fin, TagOffsetPos);
	fin >> m_vOffsetPos;
}

void CPhysX::setTransform(const PxTransform& transform)
{
	m_Actor->setGlobalPose(transform);
}

PxTransform CPhysX::getTransform() const
{
	return m_Actor->getGlobalPose();
}

void CPhysX::BeginOverlap(CGameObject* other)
{
	auto iter = m_vThisFrameContact.begin();
	while (m_vThisFrameContact.end() != iter)
	{
		if (other == (*iter).Other)
		{
			return;
		}
		else
		{
			++iter;
		}
	}

	++m_CollisionCount;

	m_vThisFrameContact.push_back(tCollisionData{other, eColType::COL_ON});

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(this, other, other->PhysX());
	}
}

void CPhysX::Overlap(CGameObject* other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->Overlap(this, other, other->PhysX());
	}
}

void CPhysX::EndOverlap(CGameObject* other)
{
	for (auto iter = m_vThisFrameContact.begin(); iter != m_vThisFrameContact.end();)
	{
		if (iter->Other == other)
		{
			iter = m_vThisFrameContact.erase(iter);
			--m_CollisionCount;
			break;
		}
		else
		{
			++iter;
		}
	}

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(this, other, other->PhysX());
	}
}
