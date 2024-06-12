#include "pch.h"
#include "PhysXMgr.h"
#include "CTimeMgr.h"

void PhysXMgr::init()
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

void PhysXMgr::tick()
{
    gScene->simulate(DT);
    gScene->fetchResults(true);
}

PhysXMgr::~PhysXMgr()
{
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    gFoundation->release();
}

