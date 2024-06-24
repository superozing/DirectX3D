#include "pch.h"
#include "CPhysX.h"

#include "CTransform.h"
#include <DirectXMath.h>
#include "CPhysXMgr.h"

CPhysX::CPhysX()
	: CComponent(COMPONENT_TYPE::PHYSX)
{
}

CPhysX::~CPhysX()
{
}

void CPhysX::updateFromPhysics()
{
	if (nullptr == m_Actor)
		return;

    PxTransform physTransform = getTransform();

	auto tempmat = XMMatrixIdentity();

	auto ObScale = Transform()->GetRelativeScale();
	Matrix matScale = XMMatrixScaling(ObScale.x, ObScale.y, ObScale.z);

	auto q = XMFLOAT4(physTransform.q.x, physTransform.q.y, physTransform.q.z, physTransform.q.w);

	Vec3 AxisAngle = RoRMath::QuaternionToEulerAngles(q);

	Matrix matRotX = XMMatrixRotationX(-AxisAngle.z);
	Matrix matRotY = XMMatrixRotationY(AxisAngle.y);
	Matrix matRotZ = XMMatrixRotationZ(AxisAngle.x);

	Matrix matTranslation = XMMatrixTranslation(physTransform.p.x, physTransform.p.y, physTransform.p.z);

	Transform()->m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// 물체의 방향값을 다시 계산한다.
	static const Vec3 arrAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장
	// XMVector3TransformNormal - w 를 0으로 확장
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// 적용 받을 상태행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		Transform()->m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], Transform()->m_matWorld);
		Transform()->m_arrWorldDir[i] = Transform()->m_arrLocalDir[i].Normalize();
	}

	Transform()->SetWorldMat(Transform()->m_matWorld);
	Transform()->m_matWorldInv = XMMatrixInverse(nullptr, Transform()->m_matWorld);
}

void CPhysX::updateToPhysics()
{
	//TODO: 로직구현하기
    //PxTransform physTransform(PxVec3(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y, Transform()->GetWorldPos().z),
    //    PxQuat(Transform()->GetWorldRot().x, Transform()->GetWorldRot().y, Transform()->GetWorldRot().z, 1.f));
    //setTransform(physTransform);
}

void CPhysX::begin()
{
	if (true == m_bStaticActor)
	{
		CPhysXMgr::GetInst()->addStaticGameObject(GetOwner());
	}
	else
	{
		CPhysXMgr::GetInst()->addDynamicGameObject(GetOwner());
	}
}

void CPhysX::finaltick()
{
	if (m_bStaticActor)
	{

	}
	else
	{
		updateFromPhysics();
	}
}

void CPhysX::SaveToFile(FILE* _File)
{
}

void CPhysX::LoadFromFile(FILE* _File)
{
}

void CPhysX::setTransform(const PxTransform& transform)
{
    m_Actor->setGlobalPose(transform);
}

PxTransform CPhysX::getTransform() const
{
    return m_Actor->getGlobalPose();
}

