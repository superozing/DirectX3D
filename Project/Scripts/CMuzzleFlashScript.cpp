#include "pch.h"
#include "CMuzzleFlashScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CMuzzleFlashScript::CMuzzleFlashScript()
	: CScript((UINT)SCRIPT_TYPE::MUZZLEFLASHSCRIPT)
	, m_pTarget(nullptr)
{
}

CMuzzleFlashScript::~CMuzzleFlashScript()
{
}

void CMuzzleFlashScript::begin()
{
	m_pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");

	InitMuzzleFlash();
	Transform()->SetRelativePos(Vec3(19.f, 24.f, -25.f));
}

void CMuzzleFlashScript::tick()
{
	Matrix BoneFrameMat = m_pTarget->Animator3D()->FindBoneMat(L"Bip001_Weapon");
	Transform()->SetFrameMat(BoneFrameMat); // x : 19, y : 25, z : -24
}

void CMuzzleFlashScript::InitMuzzleFlash()
{
	tParticleModule module = {};

	// 초기 모듈 세팅
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	module.SpaceType	  = 0;
	module.SpawnType	  = 1; // 0 : Continuous, 1 : Burst
	module.BurstInterval  = 0.f;
	module.vSpawnColor	  = Vec4(1.f, 1.f, 1.f, 1.f);
	module.vSpawnMinScale = Vec4(10.f, 15.f, 1.f, 1.f);
	module.vSpawnMaxScale = Vec4(10.f, 15.f, 1.f, 1.f);
	module.MinLife		  = 1.f;
	module.MaxLife		  = 1.f;
	module.MinMass		  = 1.f;
	module.MaxMass		  = 1.f;
	module.SpawnShape	  = 1; // 0 : Sphere, 1 : Box
	module.vSpawnBoxScale = Vec4(1.f, 1.f, 1.f, 0.f);
	module.SpawnRate	  = 2;

	// Add Velocity Module
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	module.AddVelocityType									   = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	module.MinSpeed											   = 1;
	module.MaxSpeed											   = 1;
	module.FixedDirection;
	module.FixedAngle;

	// Scale
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;

	// Noise Force
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	module.NoiseForceScale									  = 10.f;
	module.NoiseForceTerm									  = 0.3f;

	// Drag Module
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
	module.DragTime									   = 0.1f;

	// Calculate Force
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;

	// Render
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
	module.VelocityAlignment							 = 1; // 속도에 따른 방향 정렬
	module.AlphaBasedLife								 = 1; // 0 : off, 1 : NomrlizedAge, 2: Age
	module.AlphaMaxAge									 = 2.f;
	
	// Animation
	module.arrModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION] = 0;

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\MuzzleFlash02.png");
	ParticleSystem()->SetModule(module);
	ParticleSystem()->SetParticleTex(pTex);
}