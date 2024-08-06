#pragma once
#include "CManager.h"
#include <iostream>
#include <windows.h>
#include <sstream>
#include "CPhysX.h"

struct tRoRHitInfo
{
	CGameObject* pOtherObj = nullptr;
	Vec3		 vHitPos   = Vec3();
};

enum RayCastDebugFlag
{
	AllInvisible	  = (0),
	StartPointVisible = (1 << 0),
	EndPointVisible	  = (1 << 1),
	RayLineVisible	  = (1 << 2),
	StartEndVisible	  = (StartPointVisible | EndPointVisible),
	AllVisible		  = (StartPointVisible | EndPointVisible | RayLineVisible),
};

struct FlagActorColInfo
{
	PxActor*		 Actor1;
	PxActor*		 Actor2;
	PxPairFlag::Enum State;
};

using namespace physx;

class CPhysXMgr : public CManager<CPhysXMgr>
{
	SINGLE(CPhysXMgr);

private:
	PxDefaultAllocator			gAllocator;
	PxDefaultErrorCallback		gErrorCallback;
	PxFoundation*				gFoundation		  = nullptr;
	PxPvd*						gPvd			  = nullptr;
	PxPhysics*					gPhysics		  = nullptr;
	PxDefaultCpuDispatcher*		gDispatcher		  = nullptr;
	PxScene*					gScene			  = nullptr;
	PxMaterial*					gMaterial		  = nullptr;
	class RoRCollisionCallback* gCollisionCalback = nullptr;
	vector<FlagActorColInfo>	m_vecTriggerColInfo;
	float						m_fGravityMul	 = 5.f;
	float						m_fContactOffset = 0.1f;
	float						m_fLestOffset	 = 0.5f;

	void LayerCheck(UINT _left, UINT _right);
	void LayerCheckToggle(UINT _left, UINT _right);
	class MyPhysXErrorCallback : public PxErrorCallback
	{
	public:
		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			// std::ostringstream oss;
			// oss << "PhysX Error (" << code << "): " << message << " in " << file << " at line " << line << std::endl;
			// OutputDebugStringA(oss.str().c_str());
		}
	};
	void setFillterData(PxShape* _shape, UINT _Layer);

public:
#pragma region RayCast CallBack
	// 사용자 정의 필터 콜백
	class CustomQueryCallback : public PxQueryFilterCallback
	{
	public:
		virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape,
											   const PxRigidActor* actor, PxHitFlags& queryFlags)
		{
			// 레이어 필터링 로직
			UINT shapeLayer = shape->getSimulationFilterData().word0;
			// UINT rayLayer = (UINT)LAYER::LAYER_LIGHT;//filterData.word0;
			UINT rayLayer = filterData.word0;

			// if ((CPhysXMgr::m_layerMasks[rayLayer] & (1 << shapeLayer)) == 0 && (CPhysXMgr::m_layerMasks[shapeLayer]
			// & (1 << rayLayer)) == 0) if ((rayLayer & (1 << shapeLayer)) == 0 && (CPhysXMgr::m_layerMasks[shapeLayer]
			// & (1 << rayLayer)) == 0)
			if ((rayLayer & shapeLayer) == 0)
			{
				return PxQueryHitType::eNONE; // 충돌 무시
			}

			return PxQueryHitType::eBLOCK; // 충돌 허용
		}
		virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit,
												const PxShape* shape, const PxRigidActor* actor) override
		{
			// 기본적으로 아무 작업도 수행하지 않음
			return PxQueryHitType::eBLOCK;
		}
	};
#pragma endregion
	static UINT	 m_layerMasks[static_cast<UINT>(LAYER::LAYER_MAX)];
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}
	virtual void exit() override;
	void		 addGameObject(CGameObject* object);

	bool PerfomRaycast(Vec3 _OriginPos, Vec3 _Dir, tRoRHitInfo& _HitInfo, UINT _LAYER = (UINT)LAYER::LAYER_RAYCAST,
					   int _DebugFlagMask = RayCastDebugFlag::StartEndVisible);
	bool ViewPortRaycast(tRoRHitInfo& _HitInfo, UINT _LAYER = (UINT)LAYER::LAYER_RAYCAST,
						 int _DebugFlagMask = RayCastDebugFlag::StartEndVisible);

	PxShape* createConeShape(PxRigidActor* actor, PxPhysics* gPhysics, PxMaterial* gMaterial, float radius,
							 float height, int numSides);

private:
	void ClearAllActors();
	void ReleaseActor(PxRigidActor* _actor);

	friend class ParamUI;
	friend class CPhysXMgrScript;
	friend class RoRCollisionCallback;
	friend class CPhysX;
};
