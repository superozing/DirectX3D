#include "pch.h"
#include "CPhysXMgr.h"
#include "CTimeMgr.h"

CPhysXMgr::CPhysXMgr() {}

void CPhysXMgr::init()
{
    // PhysX 초기화
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    gScene = gPhysics->createScene(sceneDesc);
}

void CPhysXMgr::tick()
{
    gScene->simulate(DT);
    gScene->fetchResults(true);
}

CPhysXMgr::~CPhysXMgr()
{
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    gFoundation->release();
}

