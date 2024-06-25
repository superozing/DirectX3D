#pragma once
#include "CManager.h"

using namespace physx;

class CPhysXMgr :
    public CManager<CPhysXMgr>
{
	SINGLE(CPhysXMgr);

private:
	PxDefaultAllocator      gAllocator;
	PxDefaultErrorCallback  gErrorCallback;
	PxFoundation* gFoundation = nullptr;
	PxPhysics* gPhysics = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;
	PxScene* gScene = nullptr;
	PxMaterial* gMaterial = nullptr;
	class RoRCollisionCallback* gCollisionCalback = nullptr;

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}
	void addGameObject(CGameObject* object, bool _bStatic);

private:
	void Clear() {};
};
