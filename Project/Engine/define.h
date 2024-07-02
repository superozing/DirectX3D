﻿#pragma once

#define SINGLE(classtype) \
private:                  \
	classtype();          \
	~classtype();         \
	friend class CSingleton<classtype>;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define DTd CTimeMgr::GetInst()->GetDeltaTime_d()

#define DT_ENGINE CTimeMgr::GetInst()->GetEngineDeltaTime()
#define DTd_ENGINE CTimeMgr::GetInst()->GetEngineDeltaTime_d()

#define KEY_CHECK(Key, State, Focus) \
	(CKeyMgr::GetInst()->GetKeyState(Key) == State) && (CKeyMgr::GetInst()->GetFocusState() == Focus)

#define KEY_TAP(Key) KEY_CHECK(Key, TAP, FOCUS_STATE::MAIN)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED, FOCUS_STATE::MAIN)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED, FOCUS_STATE::MAIN)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE, FOCUS_STATE::MAIN)
#define WHEEL_CHECK(Key) \
	(CKeyMgr::GetInst()->GetWheel() == Key && (CKeyMgr::GetInst()->GetFocusState() == FOCUS_STATE::MAIN))

#define KEY_TAP_EDITOR(Key) KEY_CHECK(Key, TAP, FOCUS_STATE::OTHER)
#define KEY_PRESSED_EDITOR(Key) KEY_CHECK(Key, PRESSED, FOCUS_STATE::OTHER)
#define KEY_RELEASED_EDITOR(Key) KEY_CHECK(Key, RELEASED, FOCUS_STATE::OTHER)
#define KEY_NONE_EDITOR(Key) KEY_CHECK(Key, NONE, FOCUS_STATE::OTHER)
#define WHEEL_CHECK_EDITOR(Key) \
	(CKeyMgr::GetInst()->GetWheel() == Key && (CKeyMgr::GetInst()->GetFocusState() == FOCUS_STATE::OTHER))

// #define LAYER_MAX 32
#define MAX_MIP 8

#define CLONE(TYPE)             \
	virtual TYPE* Clone()       \
	{                           \
		return new TYPE(*this); \
	}
#define CLONE_DISABLE(TYPE) \
	TYPE* Clone()           \
	{                       \
		return nullptr;     \
		assert(nullptr);    \
	}                       \
	TYPE(const TYPE& _OriginBuffer) = delete;

#define ExtensionAnim ".anim"
#define ExtensionLevel ".lv"
#define ExtensionPref ".pref"
#define ExtensionMtrl ".mtrl"
#define ExtensionFSM ".fsm"

enum class LAYER
{
	LAYER_DEFAULT,
	LAYER_BACKGROUND,
	LAYER_TILE,
	LAYER_PLAYER,
	LAYER_MONSTER,
	LAYER_LIGHT,
	LAYER_RAYCAST,
	LAYER_UI = 31,
	LAYER_MAX = 32,
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	PREFAB,
	TEXTURE,
	MATERIAL,
	SOUND,
	COMPUTE_SHADER,
	GRAPHICS_SHADER,
	FSM,
	END,
};

extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];

enum class COMPONENT_TYPE
{
	TRANSFORM, // 오브젝트의 위치,크기,회전

	COLLIDER2D, // 2차원 충돌체
	COLLIDER3D, // 3차원 충돌체

	ANIMATOR2D, // 스프라이트 Animation
	ANIMATOR3D, // Bone Skinning Animation

	LIGHT2D, // 2차원 광원
	LIGHT3D, // 3차원 광원

	CAMERA, // 카메라 기능

	STATEMACHINE, // 상태머신

	PHYSX,

	// Render Component
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,

	SCRIPT,
};

// 상수버퍼 종류
enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	ANIM2D_DATA,
	GLOBAL_DATA,
	ANIMATION,

	END,
};

enum class SB_TYPE
{
	LIGHT2D,
	LIGHT3D,
	CROSS,
	WEIGHTMAP,
	END,
};

// Rasterizer State Type
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,

	END,
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,		// 작은게 통과			깊이 기록 O
	LESS_EQUAL, // 작거나 같으면 통과		깊이 기록 O

	GRATER,		  // 더 멀면 통과			깊이 기록 O
	GRATER_EQUAL, // 더 멀거나 같으면 통과	깊이 기록 O

	NO_TEST,  // 깊이 테스트 X			깊이 기록 O
	NO_WRITE, // 깊이 테스트 O			깊이 기록 X

	NO_TEST_NO_WRITE, // 깊이 테스트 X			깊이 기록 X

	// Volume Mesh 용
	BACKFACE_CHECK,
	FRONTFACE_CHECK,
	STENCIL_CHECK,

	END,
};

// Blend State
enum class BS_TYPE
{
	DEFAULT,

	ALPHA_BLEND,

	ONE_ZERO,

	ONE_ONE,

	DECAL,

	END,
};

// ScalarParam
enum class SCALAR_PARAM
{
	BOOL_0,
	BOOL_1,
	BOOL_2,
	BOOL_3,

	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

	END,
};

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

enum class SCRIPT_PARAM
{
	BOOL,
	INT,
	FLOAT,
	STRING,
	VEC2,
	VEC3,
	VEC4,
	OBJECT,
	COLOR,
	FUNC_STATIC,
	FUNC_MEMBER,
	// PhysXMgr 리플렉션용
	MGR_PHYSX,
};

enum class FUNC_PARAM
{
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,

	END,
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	CROSS,
	CYLINDER,

	CUBE,
	SPHERE,
	CONE,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class SHADER_DOMAIN
{
	// Deferred
	DOMAIN_DEFERRED, // 자연 렌더링
	DOMAIN_DECAL,	 // 데칼 (문신)
	DOMAIN_LIGHTING, // 광원 연산

	// Merge
	DOMAIN_MERGE, // Deferred 정보를 SwapChain 타겟으로 이동

	// Foward
	DOMAIN_OPAQUE,		// 불투명
	DOMAIN_MASKED,		// 불투명 or 투명
	DOMAIN_TRANSPARENT, // 반투명
	DOMAIN_POSTPROCESS, // 후처리

	DOMAIN_SHADOWMAP, // ShadowMap

	DOMAIN_DEBUG,
};

enum class PARTICLE_MODULE
{
	SPAWN,		  // 파티클 생성 관련
	DRAG,		  // 감속 관련 모듈
	SCALE,		  // 크기 변화 모듈
	ADD_VELOCITY, // 초기 속도 지정 모듈
	NOISE_FORCE,
	CALCULATE_FORCE, // 주어진 힘을 계산하는 모듈

	RENDER, // 렌더링 때의 옵션 관련

	END,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
	NONE,
};

enum class FOCUS_STATE
{
	OTHER,
	MAIN,
	NONE,
};

enum class MRT_TYPE
{
	SWAPCHAIN,	  // RT 1, DS 1
	DEFERRED,	  // RT 5, DS 0
	DECAL,		  // RT 2, DS 0
	LIGHT,		  // RT 2, DS 0
	SHADOW_DEPTH, // RT 1, DS 1

	END,
};

struct tWeight_4
{
	float arrWeight[4];
};

enum class LANDSCAPE_MODE
{
	HEIGHT_MAP,
	SPLAT,
	NONE,
};

enum class Log_Level
{
	INFO = 1,
	WARN = 2,
	ERR = 4,
	END,
};
