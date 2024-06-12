#include "pch.h"
#include "CPhysXMgr.h"
#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CPhysX.h"

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

void CPhysXMgr::addGameObject(CGameObject* object)
{
    // 게임 오브젝트의 물리 객체 생성 및 Scene에 추가
    PxTransform transform(PxVec3(object->Transform()->GetWorldPos().x, object->Transform()->GetWorldPos().y, object->Transform()->GetWorldPos().z),
        PxQuat(object->Transform()->GetWorldRot().x, object->Transform()->GetWorldRot().y, object->Transform()->GetWorldRot().z, 1.f));// object->transform->getRotation().w));
    PxRigidDynamic* actor = gPhysics->createRigidDynamic(transform);
    // Collider 추가 등
    gScene->addActor(*actor);
    object->AddComponent(new CPhysX);
}

CPhysXMgr::~CPhysXMgr()
{
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    gFoundation->release();
}

