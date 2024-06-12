#include "pch.h"
#include "CPhysX.h"

#include "CTransform.h"

CPhysX::CPhysX()
	: CComponent(COMPONENT_TYPE::PHYSX)
{
}

CPhysX::~CPhysX()
{
}

void CPhysX::updateFromPhysics()
{
    PxTransform physTransform = getTransform();
    Transform()->SetRelativePos(Vec3(physTransform.p.x, physTransform.p.y, physTransform.p.z));
    Transform()->SetRelativeRotation(Vec3(physTransform.q.x, physTransform.q.y, physTransform.q.z));// , physTransform.q.w));
}

void CPhysX::updateToPhysics()
{

    PxTransform physTransform(PxVec3(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y, Transform()->GetWorldPos().z),
        PxQuat(Transform()->GetWorldRot().x, Transform()->GetWorldRot().y, Transform()->GetWorldRot().z, 1.f));
    setTransform(physTransform);
}

void CPhysX::begin()
{
}

void CPhysX::finaltick()
{
}

void CPhysX::SaveToFile(FILE* _File)
{
}

void CPhysX::LoadFromFile(FILE* _File)
{
}

void CPhysX::setTransform(const PxTransform& transform)
{
    mActor->setGlobalPose(transform);
}

PxTransform CPhysX::getTransform() const
{
    return mActor->getGlobalPose();
}

