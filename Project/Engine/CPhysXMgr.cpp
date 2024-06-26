#include "pch.h"
#include "CPhysXMgr.h"
#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CPhysX.h"
#include "RoRCollisionCallback.h"

UINT CPhysXMgr::m_layerMasks[32] = { 0 };

CPhysXMgr::CPhysXMgr() {}

PxFilterFlags CustomFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    UINT layer0 = filterData0.word0;
    UINT layer1 = filterData1.word0;

    //if ((CPhysXMgr::m_layerMasks[layer0] & (1 << layer1)) == 0 && (CPhysXMgr::m_layerMasks[layer1] & (1 << layer0)) == 0)
    //{
    //    // 충돌을 무시합니다.
    //    return PxFilterFlag::eSUPPRESS;
    //}

    // 모든 충돌에 대해 충돌 보고 활성화
    pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
    return PxFilterFlag::eDEFAULT;
}

void CPhysXMgr::LayerCheck(UINT _left, UINT _right)
{
    UINT iRow = _left;
    UINT iCol = _right;

    if (iRow == iCol)
    {
        m_layerMasks[iRow] |= (1 << iCol);
        return;
    }

    if (iRow > iCol)
    {
        std::swap(iRow, iCol);
    }

    m_layerMasks[iRow] |= (1 << iCol);
    m_layerMasks[iCol] |= (1 << iRow);
}


void CPhysXMgr::LayerCheckToggle(UINT _left, UINT _right)
{
    UINT iRow = _left;
    UINT iCol = _right;

    if (iRow == iCol)
    {
        m_layerMasks[_left] ^= (1 << _right);
        return;
    }

    if (iRow > iCol)
    {
        std::swap(iRow, iCol);
    }

    // iRow와 iCol 위치의 비트를 반전시킴
    m_layerMasks[iRow] ^= (1 << iCol);
    m_layerMasks[iCol] ^= (1 << iRow);
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

    //필터
    LayerCheck((UINT)LAYER::LAYER_DEFAULT, (UINT)LAYER::LAYER_PLAYER);

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
    PxShape* shape = gPhysics->createShape(PxBoxGeometry(scale.z / 2, scale.y / 2, scale.x / 2), *gMaterial);
    
    // 객체에 필터 데이터 설정
    PxFilterData filterData;
    filterData.word0 = (UINT)object->GetLayerIdx(); // 레이어 번호 설정
    shape->setSimulationFilterData(filterData);

    actor->attachShape(*shape);

    // Collider 추가 후, 씬에 배우 추가
    gScene->addActor(*actor);

    // 액터 추가
    object->PhysX()->m_Actor = actor;
    actor->userData = object;
}

bool CPhysXMgr::PerfomRaycast(PxVec3 origin, PxVec3 direction, tRoRHitInfo& _HitInfo)
{
    direction.normalize();
    // TODO(JINYOUNG) : DrawDebugLine 함수추가
    //if (true == m_Debug)
    //{
    //    GamePlayStatic::DrawDebugLine();
    //}


    PxRaycastBuffer hit;
    bool status = gScene->raycast(origin, direction, PX_MAX_F32, hit);

    if (true == status) 
    {
        // 충돌한 경우

        PxRaycastHit hitInfo = hit.block;
        PxVec3 hitPoint = hitInfo.position;
        PxRigidActor* hitActor = hitInfo.actor;

        auto pGO = hitActor->userData;
        _HitInfo.pOtherObj = static_cast<CGameObject*>(pGO);
        _HitInfo.vHitPos = Vec3(hitPoint.x, hitPoint.y, hitPoint.z);

        // 충돌 지점과 충돌한 물체에 대한 정보를 처리
        // 예: 충돌 지점 출력
        GamePlayStatic::DrawDebugSphere(_HitInfo.vHitPos, 20.f, Vec4(0.f,0.8f,0.f,1.f) ,false);
        return true;
    }
    else 
    {
        return false;
    }
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

