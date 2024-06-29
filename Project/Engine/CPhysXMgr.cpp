#include "pch.h"
#include "CPhysXMgr.h"
#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CPhysX.h"
#include "RoRCollisionCallback.h"

UINT CPhysXMgr::m_layerMasks[32] = { 0 };

CPhysXMgr::CPhysXMgr() {}

bool CPhysXMgr::PerfomRaycast(Vec3 _OriginPos, Vec3 _Dir, tRoRHitInfo& _HitInfo, UINT _LAYER)
{
    PxVec3 OriginPos = PxVec3(_OriginPos.x, _OriginPos.y, _OriginPos.z);
    PxVec3 Dir = PxVec3(_Dir.x, _Dir.y, _Dir.z);
    Dir.normalize();

    PxRaycastBuffer hit;
    static PxQueryFilterData filterData;

    filterData.data.word0 = m_layerMasks[_LAYER]; // 고정된 레이어 마스크 설정
    filterData.flags = PxQueryFlag::ePREFILTER | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC; // ePREFILTER 플래그 설정

    static CustomQueryCallback queryCallback;

    bool status = gScene->raycast(OriginPos, Dir, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData, &queryCallback);

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
        if (true == m_RayDebug)
        {
            GamePlayStatic::DrawDebugSphere(Vec3(_OriginPos.x, _OriginPos.y, _OriginPos.z), 50.f, Vec4(1.f, 0.8f, 0.f, 1.f), false);
            GamePlayStatic::DrawDebugSphere(_HitInfo.vHitPos, 50.f, Vec4(0.f, 0.8f, 0.f, 1.f), false);
            GamePlayStatic::DrawDebugCylinder(Vec3(_OriginPos.x, _OriginPos.y, _OriginPos.z), _HitInfo.vHitPos,5.f,Vec3(0.f,.8f,0.f),true);
        }
        return true;
    }
    else
    {
        return false;
    }
}

PxFilterFlags CustomFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    UINT layer0 = filterData0.word1;
    UINT layer1 = filterData1.word1;

    if ((CPhysXMgr::m_layerMasks[layer0] & (1 << layer1)) == 0 && (CPhysXMgr::m_layerMasks[layer1] & (1 << layer0)) == 0)
    {
        // 충돌을 무시합니다.
        return PxFilterFlag::eSUPPRESS;
    }

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

void CPhysXMgr::setFillterData(PxShape* _shape, UINT _Layer)
{
    // 객체에 필터 데이터 설정
    PxFilterData filterData;
    filterData.word0 = 1 << _Layer; // 레이어 번호 설정
    filterData.word1 = _Layer; // 레이어 번호 설정
    _shape->setSimulationFilterData(filterData);
    _shape->setQueryFilterData(filterData);
}

void CPhysXMgr::addGameObject(CGameObject* object, bool _bStatic, PhysShape _Shape)
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
    PxShape* shape;
    // Collider 추가 (여기서는 예시로 Box Collider를 사용)
    if (PhysShape::BOX == _Shape)
    {
        shape = gPhysics->createShape(PxBoxGeometry(scale.z / 2, scale.y / 2, scale.x / 2), *gMaterial);
    }
    else
    {
        shape = gPhysics->createShape(PxSphereGeometry(scale.x / 2), *gMaterial);
    }
    
    //필터정보 세팅
    setFillterData(shape, (UINT)object->GetLayerIdx());

    actor->attachShape(*shape);

    // Collider 추가 후, 씬에 배우 추가
    gScene->addActor(*actor);

    // 액터 추가
    object->PhysX()->m_Actor = actor;
    actor->userData = object;
}

void CPhysXMgr::init()
{
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    //static MyPhysXErrorCallback gErrorCallback;
    //gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    //// PVD 연결 설정
    //gPvd = PxCreatePvd(*gFoundation);
    //PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    //gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    // PhysX 초기화
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;

    //필터
    LayerCheck((UINT)LAYER::LAYER_MONSTER, (UINT)LAYER::LAYER_PLAYER);
    LayerCheck((UINT)LAYER::LAYER_DEFAULT, (UINT)LAYER::LAYER_RAYCAST);
    LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_RAYCAST);

    sceneDesc.filterShader = CustomFilterShader;

    gScene = gPhysics->createScene(sceneDesc);
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f); // (정지 마찰 계수, 동적 마찰 계수, 반발 계수)

    gCollisionCalback = new RoRCollisionCallback;
    gScene->setSimulationEventCallback(gCollisionCalback);
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
    if (gPvd)
    {
        PxPvdTransport* transport = gPvd->getTransport();
        if (transport)
        {
            transport->release();
        }
        gPvd->release();
        gPvd = nullptr;
    }
    gFoundation->release();

    //gMaterial->release();
    if (nullptr != gCollisionCalback)
    {
        delete gCollisionCalback;
        gCollisionCalback = nullptr;
    }
}

