#pragma once
#include "CManager.h"

struct tRoRHitInfo
{
	CGameObject* pOtherObj = nullptr;
	Vec3 vHitPos = Vec3();
};

using namespace physx;

class CPhysXMgr :
    public CManager<CPhysXMgr>
{
	SINGLE(CPhysXMgr);

private:
	bool m_RayDebug = true;
	PxDefaultAllocator      gAllocator;
	PxDefaultErrorCallback  gErrorCallback;
	PxFoundation* gFoundation = nullptr;
	PxPhysics* gPhysics = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;
	PxScene* gScene = nullptr;
	PxMaterial* gMaterial = nullptr;
	class RoRCollisionCallback* gCollisionCalback = nullptr;

	void LayerCheck(UINT _left, UINT _right);
	void LayerCheckToggle(UINT _left, UINT _right);

public:
	static UINT m_layerMasks[static_cast<UINT>(LAYER::LAYER_MAX)];
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}
	void addGameObject(CGameObject* object, bool _bStatic);
	bool PerfomRaycast(PxVec3 origin, PxVec3 direction , tRoRHitInfo& _HitInfo);

private:
	void Clear() {};

	friend class ParamUI;
	friend class CPhysXMgrScript;
};
