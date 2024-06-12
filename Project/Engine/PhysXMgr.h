#pragma once
#include "CManager.h"
#include "physx/PxPhysicsAPI.h"

using namespace physx;

class PhysXMgr :
    public CManager<PhysXMgr>
{
	SINGLE(PhysXMgr);

private:
	PxDefaultAllocator      gAllocator;
	PxDefaultErrorCallback  gErrorCallback;
	PxFoundation* gFoundation = nullptr;
	PxPhysics* gPhysics = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;
	PxScene* gScene = nullptr;

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}

private:
	void Clear() {};
};
