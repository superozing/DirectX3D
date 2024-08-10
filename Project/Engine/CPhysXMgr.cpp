#include "pch.h"
#include "CPhysXMgr.h"
#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CPhysX.h"
#include "RoRCollisionCallback.h"

#include "CLevelMgr.h"
#include "CLevel.h"

UINT CPhysXMgr::m_layerMasks[32] = {0};

CPhysXMgr::CPhysXMgr()
{
}

bool CPhysXMgr::PerformRaycast(Vec3 _OriginPos, Vec3 _Dir, tRoRHitInfo& _HitInfo, UINT _LAYER, int _DebugFlagMask)
{
	PxVec3 OriginPos = PxVec3(_OriginPos.x, _OriginPos.y, _OriginPos.z) / m_PPM;
	PxVec3 Dir		 = PxVec3(_Dir.x, _Dir.y, _Dir.z);
	Dir.normalize();

	PxRaycastBuffer			 hit;
	static PxQueryFilterData filterData;

	filterData.data.word0 = m_layerMasks[_LAYER]; // 고정된 레이어 마스크 설정
	filterData.flags = PxQueryFlag::ePREFILTER | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC; // ePREFILTER 플래그 설정

	static CustomQueryCallback queryCallback;

	bool status = gScene->raycast(OriginPos, Dir, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData, &queryCallback);

	// 시작지점출력
	if (0 != (_DebugFlagMask & RayCastDebugFlag::StartPointVisible))
	{
		GamePlayStatic::DrawDebugSphere(_OriginPos, 50.f, Vec4(1.f, 0.8f, 0.f, 1.f), false, _LAYER);
	}

	if (true == status)
	{
		// 충돌한 경우

		PxRaycastHit  hitInfo  = hit.block;
		PxVec3		  hitPoint = hitInfo.position * m_PPM;
		PxRigidActor* hitActor = hitInfo.actor;

		auto pGO		   = hitActor->userData;
		_HitInfo.pOtherObj = static_cast<CGameObject*>(pGO);

		_HitInfo.vHitPos = Vec3(hitPoint.x, hitPoint.y, hitPoint.z);

		// 충돌 지점과 충돌한 물체에 대한 정보를 처리
		// 예: 충돌 지점 출력
		if (0 != (_DebugFlagMask & RayCastDebugFlag::RayLineVisible))
		{
			GamePlayStatic::DrawDebugCylinder(_OriginPos, _HitInfo.vHitPos, 5.f, Vec3(0.f, .8f, 0.f), false, _LAYER);
		}
		if (0 != (_DebugFlagMask & RayCastDebugFlag::EndPointVisible))
		{
			GamePlayStatic::DrawDebugSphere(_HitInfo.vHitPos, 50.f, Vec4(0.f, 0.8f, 0.f, 1.f), false, _LAYER);
		}
		return true;
	}
	else
	{
		if (0 != (_DebugFlagMask & RayCastDebugFlag::RayLineVisible))
		{
			GamePlayStatic::DrawDebugCylinder(_OriginPos, _OriginPos + _Dir * 1000.f, 5.f, Vec3(0.f, .8f, 0.f), false,
											  _LAYER);
		}
		return false;
	}
}

#include "CMRT.h"
#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CCamera.h"
bool CPhysXMgr::ViewPortRaycast(tRoRHitInfo& _HitInfo, UINT _LAYER, int _DebugFlagMask)
{
	CMRT*		   pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP	= pMRT->GetViewPort();

	//  현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	auto camera = CRenderMgr::GetInst()->GetMainCam();
	if (camera->ViewportConvertFunc != nullptr)
		vMousePos = camera->ViewportConvertFunc();

	// 카메라로부터 역투영 행렬과 역뷰 행렬을 가져옵니다
	Matrix ProjMat	  = camera->GetProjMat();
	Matrix ViewInvMat = camera->GetViewInvMat();

	Vec3 rayOrigin = camera->Transform()->GetWorldPos();
	Vec3 rayDir;

	// view space 에서의 방향
	rayDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - ProjMat._31) / ProjMat._11;
	rayDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - ProjMat._32) / ProjMat._22;
	rayDir.z = 1.f;

	// world space 에서의 방향
	rayDir = XMVector3TransformNormal(rayDir, ViewInvMat);
	rayDir.Normalize();

	// 레이케스트 수행
	bool hit =
		CPhysXMgr::GetInst()->PerformRaycast(Vec3(rayOrigin.x, rayOrigin.y, rayOrigin.z),
											 Vec3(rayDir.x, rayDir.y, rayDir.z), _HitInfo, _LAYER, _DebugFlagMask);
	return hit;
}

PxShape* CPhysXMgr::createConeShape(PxRigidActor* actor, PxPhysics* gPhysics, PxMaterial* gMaterial, float radius,
									float height, int numSides)
{
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcubedebug);

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count	 = pMesh->GetVtxCount();
	convexDesc.points.stride = sizeof(Vtx);
	convexDesc.points.data	 = pMesh->GetVtxSysMem();

	// if (pMeshCol->m_bConvex)
	convexDesc.flags |= PxConvexFlag::eCOMPUTE_CONVEX;

	// PxConvexMesh 생성
	PxTolerancesScale				scale;
	PxCookingParams					params(scale);
	PxDefaultMemoryOutputStream		buf;
	PxConvexMeshCookingResult::Enum result;

	// 유효한 메쉬인 경우에만 생성
	if (PxCookConvexMesh(params, convexDesc, buf, &result))
	{
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxConvexMesh*			 convexMesh = gPhysics->createConvexMesh(input);

		PxShape* shape = PxRigidActorExt::createExclusiveShape(
			*actor, PxConvexMeshGeometry(convexMesh, PxMeshScale(PxVec3(height, radius, radius))), *gMaterial);
		return shape;
	}

	return nullptr;
	// static PxTriangleMesh* triangleMesh = nullptr;

	// if (nullptr == triangleMesh)
	//{
	//	// 원뿔의 꼭짓점과 바닥을 구성하는 정점 벡터
	//	std::vector<PxVec3> vertices;
	//	vertices.push_back(PxVec3(0.0f, height / 2, 0.0f)); // 원뿔의 꼭짓점

	//	// 바닥 원형의 정점들
	//	for (int i = 0; i < numSides; ++i)
	//	{
	//		float angle = i * PxPi * 2 / numSides;
	//		float x		= radius * cos(angle);
	//		float z		= radius * sin(angle);
	//		vertices.push_back(PxVec3(x, -height / 2, z));
	//	}

	//	// 삼각형 인덱스 벡터
	//	std::vector<PxU32> indices;
	//	for (int i = 1; i <= numSides; ++i)
	//	{
	//		indices.push_back(0); // 꼭짓점
	//		indices.push_back(i);
	//		indices.push_back(i % numSides + 1);
	//	}

	//	// 바닥 삼각형들
	//	for (int i = 1; i <= numSides; ++i)
	//	{
	//		indices.push_back(i);
	//		indices.push_back(numSides + 1);
	//		indices.push_back(i % numSides + 1);
	//	}

	//	// 삼각형 메시 생성
	//	PxTriangleMeshDesc meshDesc;
	//	meshDesc.points.count  = static_cast<PxU32>(vertices.size());
	//	meshDesc.points.stride = sizeof(PxVec3);
	//	meshDesc.points.data   = vertices.data();

	//	meshDesc.triangles.count  = static_cast<PxU32>(indices.size() / 3);
	//	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	//	meshDesc.triangles.data	  = indices.data();
	//	PxDefaultMemoryOutputStream		  writeBuffer;
	//	PxTriangleMeshCookingResult::Enum result;
	//	if (!gPhysics->getCooking()->cookTriangleMesh(meshDesc, writeBuffer, &result))
	//	{
	//		return nullptr;
	//	}
	//	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	//	triangleMesh = gPhysics->createTriangleMesh(readBuffer);
	//}

	// return gPhysics->createShape(PxTriangleMeshGeometry(triangleMesh), *gMaterial);
}

PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
								 PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags,
								 const void* constantBlock, PxU32 constantBlockSize)
{
	UINT layer0 = filterData0.word1;
	UINT layer1 = filterData1.word1;

	if ((CPhysXMgr::m_layerMasks[layer0] & (1 << layer1)) == 0 &&
		(CPhysXMgr::m_layerMasks[layer1] & (1 << layer0)) == 0)
	{
		// 충돌을 무시합니다.
		return PxFilterFlag::eSUPPRESS;
	}

	// 모든 충돌에 대해 충돌 보고 활성화
	// pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
	//			PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eSOLVE_CONTACT |
	//			PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eDETECT_CCD_CONTACT;
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
				PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_TOUCH_CCD;
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
	filterData.word1 = _Layer;		// 레이어 번호 설정
	_shape->setSimulationFilterData(filterData);
	_shape->setQueryFilterData(filterData);
}

#include "CLogMgr.h"
void CPhysXMgr::addGameObject(CGameObject* object)
{
	auto PhysX = object->PhysX();

	Quat WorldQuat = object->Transform()->GetWorldQuaternion();

	auto ColPos = PhysX->GetColliderPos();
	ColPos /= m_PPM;
	// 게임 오브젝트의 위치와 회전 정보
	PxTransform transform(PxVec3(ColPos.x, ColPos.y, ColPos.z),
						  PxQuat(WorldQuat.x, WorldQuat.y, -WorldQuat.z, -WorldQuat.w));

	PxRigidActor* actor = nullptr;

	if (PhysBodyType::STATIC == PhysX->m_bPhysBodyType)
	{
		// 고정된 물리 객체 생성
		actor = gPhysics->createRigidStatic(transform);
		// actor = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	}
	else if (PhysBodyType::TRIGGER == PhysX->m_bPhysBodyType)
	{
		// 운동학적 물리 객체 생성
		// PxRigidDynamic* dynamicActor = gPhysics->createRigidDynamic(transform);
		// dynamicActor->setKinematicTarget(transform);
		// dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		// actor = dynamicActor;

		// 동적 물리 객체 생성
		PxRigidDynamic* dynamicActor = gPhysics->createRigidDynamic(transform);
		PxRigidBodyExt::updateMassAndInertia(*dynamicActor, 10.0f);
		dynamicActor->setAngularDamping(0.5f);
		// dynamicActor->setMass(1.0f);
		PhysX->m_DActor = dynamicActor;
		actor			= dynamicActor;
		dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	}
	else
	{
		// 동적 물리 객체 생성
		PxRigidDynamic* dynamicActor = gPhysics->createRigidDynamic(transform);
		PxRigidBodyExt::updateMassAndInertia(*dynamicActor, 10.0f);
		dynamicActor->setAngularDamping(0.5f);
		// dynamicActor->setMass(1.0f);
		dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
		PhysX->m_DActor = dynamicActor;
		actor			= dynamicActor;
		// dynamicActor->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
		// dynamicActor->setAngularVelocity(PxVec3(0.f, 0.f, 0.f));
	}

	// 게임 오브젝트의 스케일 정보
	auto scale = PhysX->m_vScale;
	if (Vec3() == scale)
	{
		scale			= object->Transform()->GetWorldScale();
		PhysX->m_vScale = scale;
	}
	scale /= m_PPM;
	PxShape* shape;

	// Collider 추가 (여기서는 예시로 Box Collider를 사용)
	if (PhysShape::BOX == PhysX->m_Shape)
	{
		shape = gPhysics->createShape(PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2), *gMaterial);
	}
	else if (PhysShape::SPHERE == PhysX->m_Shape)
	{
		shape = gPhysics->createShape(PxSphereGeometry(scale.x / 2), *gMaterial);
	}
	else
	{
		float radius   = scale.x / 2; // 원뿔의 반지름
		float height   = scale.y;	  // 원뿔의 높이
		int	  numSides = 20;		  // 원뿔 바닥의 변 개수

		shape = createConeShape(actor, gPhysics, gMaterial, radius, height, numSides);
	}

	// 필터정보 세팅
	setFillterData(shape, (UINT)object->GetLayerIdx());

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);

	if (PhysBodyType::DYNAMIC == PhysX->m_bPhysBodyType)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	}
	if (PhysBodyType::TRIGGER != PhysX->m_bPhysBodyType)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		// shape->setContactOffset(m_fContactOffset);
		// shape->setRestOffset(m_fLestOffset);
	}
	if (PhysBodyType::TRIGGER == PhysX->m_bPhysBodyType)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	actor->attachShape(*shape);

	// Collider 추가 후, 씬에 배우 추가
	gScene->addActor(*actor);

	// 액터 추가
	object->PhysX()->m_Actor = actor;
	actor->userData			 = object;
}

void CPhysXMgr::init()
{
	// gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	static MyPhysXErrorCallback gErrorCallback;
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	//// PVD 연결 설정
	gPvd					  = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	// PhysX 초기화
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity		= PxVec3(0.0f, -9.81f * m_fGravityMul, 0.0f);
	gDispatcher				= PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;

	// 필터
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_ETC_OBJECT);		  // ex 바닥과 탄피
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_PLAYER_SKILL);		  // ex 바닥과 수류탄
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_MONSTER);			  // ex 바닥과 몬스터 스탠딩
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_MONSTER_SKILL);		  // ex 바닥과 몬스터스킬
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_BOSS);				  // ex 바닥과 보스 스탠딩
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_BOSS_SKILL);		  // ex 바닥과 보스 스킬
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_PLAYER_CAMERA_RAY);	  // ex 바닥과 카메라스프링암
	LayerCheck((UINT)LAYER::LAYER_PLANE, (UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY); // ex 바닥과 사격데칼

	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY); // ex 벽과 사격
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_PLAYER);				 // ex 벽과 사격
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_ETC_OBJECT);			 // ex 벽과 탄피
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_PLAYER_SKILL);		 // ex 벽과 수류탄
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_MONSTER);			 // ex 벽과 몬스터 스탠딩
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_MONSTER_SKILL);		 // ex 벽과  몬스터스킬
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_BOSS);				 // ex 벽과 보스 스탠딩
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_BOSS_SKILL);			 // ex 벽과 보스 스킬
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_PLAYER_CAMERA_RAY);	 // ex 벽과 카메라스프링암
	LayerCheck((UINT)LAYER::LAYER_WALL, (UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY); // ex 벽과 사격데칼

	LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_MONSTER);
	LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_MONSTER_SKILL);
	LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_BOSS);
	LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_BOSS_SKILL);

	LayerCheck((UINT)LAYER::LAYER_PLAYER_SKILL, (UINT)LAYER::LAYER_MONSTER);
	LayerCheck((UINT)LAYER::LAYER_PLAYER_SKILL, (UINT)LAYER::LAYER_BOSS);

	LayerCheck((UINT)LAYER::LAYER_MONSTER, (UINT)LAYER::LAYER_MONSTER);
	LayerCheck((UINT)LAYER::LAYER_MONSTER, (UINT)LAYER::LAYER_BOSS);

	LayerCheck((UINT)LAYER::LAYER_EVENT, (UINT)LAYER::LAYER_PLAYER);

	LayerCheck((UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY, (UINT)LAYER::LAYER_MONSTER);
	LayerCheck((UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY, (UINT)LAYER::LAYER_BOSS);

	// ViewPort RayCast 디버그용
	LayerCheck((UINT)LAYER::LAYER_RAYCAST, (UINT)LAYER::LAYER_MONSTER);

	sceneDesc.filterShader			  = CustomFilterShader;
	sceneDesc.kineKineFilteringMode	  = PxPairFilteringMode::eKEEP;
	sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;
	// sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

	gScene = gPhysics->createScene(sceneDesc);
	gMaterial = gPhysics->createMaterial(7.5f, 7.5f, 0.2f); // (정지 마찰 계수, 동적 마찰 계수, 반발 계수)

	gCollisionCalback = new RoRCollisionCallback;
	gScene->setSimulationEventCallback(gCollisionCalback);
}

#include "imgui.h "
#include "ImGuizmo.h "
void CPhysXMgr::tick()
{
	RETURN_IF_NOT_PLAYING

	// static const float ThresholdTime = 1.f / 120.f;
	static float acctime = 0.f;
	acctime += DT;
	if (acctime < m_TimeStep)
		return;
	acctime -= int(acctime / m_TimeStep) * m_TimeStep;
	gScene->simulate(m_TimeStep);
	// gScene->simulate(DT);
	gScene->fetchResults(true);

	// Trigger Obj Overlap판정
	for (auto& e : m_vecTriggerColInfo)
	{
		if (PxPairFlag::eNOTIFY_TOUCH_PERSISTS == e.State)
		{
			RoRCollisionCallback::handleOverlap(e.Actor1, e.Actor2);
		}
		if (PxPairFlag::eNOTIFY_TOUCH_FOUND == e.State)
		{
			// 다음 프레임부터 Persist처리
			e.State = PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		}
	}

	// 시뮬레이션 결과로 트랜스폼 업데이트
	PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	std::vector<PxRigidActor*> actors(nbActors);
	if (nbActors == 0)
		return;
	gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC,
					  reinterpret_cast<PxActor**>(&actors[0]), nbActors);

	for (PxU32 i = 0; i < nbActors; i++)
	{
		if (!actors[i]->is<PxRigidDynamic>())
			continue;

		CGameObject* obj = (CGameObject*)actors[i]->userData;
		if (PhysBodyType::TRIGGER == obj->PhysX()->m_bPhysBodyType)
			continue;

		// RigidDynamic이 캐릭터 컨트롤러인 경우
		if (nullptr == actors[i]->userData)
			continue;

		CTransform* pTr = obj->Transform();

		const PxMat44 ActorPose(actors[i]->getGlobalPose());
		Matrix		  SimulatedMat = Matrix(ActorPose.front());

		// 시뮬레이션 Matrix SRT 분해
		Vec3 Translation, Rotation, Scale;
		ImGuizmo::DecomposeMatrixToComponents(*SimulatedMat.m, Translation, Rotation, Scale);

		// PPM 적용
		Rotation.ToRadian();
		// Rotation.z = -Rotation.z;

		SimulatedMat._41 *= m_PPM;
		SimulatedMat._42 *= m_PPM;
		SimulatedMat._43 *= m_PPM;

		auto rotatedoffset = RoRMath::RotateVectorByRotationVector(obj->PhysX()->m_vOffsetPos, Rotation);

		SimulatedMat._41 -= rotatedoffset.x;
		SimulatedMat._42 -= rotatedoffset.y;
		SimulatedMat._43 -= rotatedoffset.z;

		auto scale = pTr->Transform()->GetRelativeScale();
		pTr->Transform()->SetWorldMat(SimulatedMat);
		pTr->Transform()->SetRelativeScale(scale);
	}
}

void CPhysXMgr::ClearAllActors()
{
	// 씬 잠금
	gScene->lockWrite();

	// 씬의 모든 액터를 가져오기
	physx::PxU32 actorCount =
		gScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	std::vector<physx::PxActor*> actors(actorCount);
	gScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, actors.data(),
					  actorCount);

	// 각 액터 제거
	for (physx::PxActor* actor : actors)
	{
		gScene->removeActor(*actor);
		auto obj			   = static_cast<CGameObject*>(actor->userData);
		obj->PhysX()->m_Actor  = nullptr;
		obj->PhysX()->m_DActor = nullptr;

		actor->userData = nullptr;
		actor->release();
		// auto e = static_cast<CGameObject*>(actor->userData);
		//  if (e && e->PhysX())
		//{
		//	e->PhysX()->m_Actor = nullptr;
		//  }
	}

	// 씬 잠금 해제
	gScene->unlockWrite();
}

void CPhysXMgr::ReleaseActor(PxRigidActor* actor)
{
	if (gScene == nullptr || actor->userData == nullptr)
		return;
	// 씬 잠금
	gScene->lockWrite();

	auto obj = static_cast<CGameObject*>(actor->userData);

	// trigger vec에 관리되던 요소제거
	auto iter = m_vecTriggerColInfo.begin();
	while (iter != m_vecTriggerColInfo.end())
	{
		auto GO1 = static_cast<CGameObject*>((*iter).Actor1->userData);
		auto GO2 = static_cast<CGameObject*>((*iter).Actor2->userData);

		if (obj == GO1 || obj == GO2)
		{
			iter = m_vecTriggerColInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	obj->PhysX()->m_Actor  = nullptr;
	obj->PhysX()->m_DActor = nullptr;
	actor->userData		   = nullptr;

	gScene->removeActor(*actor);
	actor->release();

	// 씬 잠금 해제
	gScene->unlockWrite();
}

void CPhysXMgr::exit()
{
	ClearAllActors();
	m_vecTriggerColInfo.clear();
}

CPhysXMgr::~CPhysXMgr()
{
	if (gScene == nullptr)
		return;

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

	// gMaterial->release();
	if (nullptr != gCollisionCalback)
	{
		delete gCollisionCalback;
		gCollisionCalback = nullptr;
	}
}
