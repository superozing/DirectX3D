﻿#include "pch.h"
#include "CPhysXMgr.h"
#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CPhysX.h"
#include "RoRCollisionCallback.h"

CPhysXMgr::CPhysXMgr() {}

PxFilterFlags CustomFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    // 모든 충돌에 대해 충돌 보고 활성화
    pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
    return PxFilterFlag::eDEFAULT;
}

void CPhysXMgr::init()
{
    // PhysX 초기화
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = CustomFilterShader;

    gScene = gPhysics->createScene(sceneDesc);
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.000f); // (정지 마찰 계수, 동적 마찰 계수, 반발 계수)

    gCollisionCalback = new RoRCollisionCallback;
    gScene->setSimulationEventCallback(gCollisionCalback);
}

void CPhysXMgr::tick()
{
    gScene->simulate(DT);
    gScene->fetchResults(true);
}

void CPhysXMgr::addGameObject(CGameObject* object, bool _bStatic)
{
    auto Rot = object->Transform()->GetWorldRot();
    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Rot.z, Rot.y, Rot.x);

    // 게임 오브젝트의 위치와 회전 정보
    PxTransform transform(PxVec3(object->Transform()->GetWorldPos().x, object->Transform()->GetWorldPos().y, object->Transform()->GetWorldPos().z),
        PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));

    PxRigidActor* actor = nullptr;

    if (_bStatic)
    {
        // 고정된 물리 객체 생성
        actor = gPhysics->createRigidStatic(transform);
    }
    else
    {
        // 동적 물리 객체 생성
        PxRigidDynamic* dynamicActor = gPhysics->createRigidDynamic(transform);
        dynamicActor->setMass(1.0f);
        actor = dynamicActor;
    }

    // 게임 오브젝트의 스케일 정보
    auto scale = object->Transform()->GetWorldScale();

    // Collider 추가 (여기서는 예시로 Box Collider를 사용)
    PxShape* shape = gPhysics->createShape(PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2), *gMaterial);
    actor->attachShape(*shape);

    // Collider 추가 후, 씬에 배우 추가
    gScene->addActor(*actor);

    // 액터 추가
    object->PhysX()->m_Actor = actor;
    actor->userData = object;
}

CPhysXMgr::~CPhysXMgr()
{
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    gFoundation->release();
    //gMaterial->release();
    if (nullptr != gCollisionCalback)
    {
        delete gCollisionCalback;
        gCollisionCalback = nullptr;
    }
}

