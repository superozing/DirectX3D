﻿#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
	Vec3 vPos;	 // 정점의 좌표
	Vec4 vColor; // 정점의 색상 정보
	Vec2 vUV;	 // UV 좌표계 or Texture Coodinate

	Vec3 vTangent;	// 접선 벡터
	Vec3 vNormal;	// 법선 벡터
	Vec3 vBinormal; // 종법선 벡터

	Vec4 vWeights; // Bone 가중치
	Vec4 vIndices; // Bone 인덱스
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;

	Vec3   vWorldPos;
	Vec3   vWorldScale;
	Vec3   vWorldRot;
	Matrix matWorld;

	Vec3  vColor;
	float fLifeTime;
	float fDuration;
	bool  bDepthTest;
};

struct tLightInfo
{
	Vec4 vColor;	// 빛의 색상	- 광원의 순수 색상
	Vec4 vSpecular; // 반사광	- 광원이 물체 표변에 반사된 색상
	Vec4 vAmbient;	// 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛

	Vec3  vWorldPos; // 광원의 위치
	Vec3  vWorldDir; // 광원이 향하는 방향
	float fRadius;	 // 광원의 영향 반경, 거리 정보
	float fAngle;	 // 광원의 범위 각도

	int LightType; // 광원 타입

	int	 ToonShading;									   // 0:안함 1:함
	Vec4 vToonShadeRange = Vec4(0.06f, 0.15f, 0.93f, 1.f); // 노말 양자화 범위
	int	 padding[2];

	friend ofstream& operator<<(ofstream& fout, const tLightInfo& info);
	friend ifstream& operator>>(ifstream& fin, tLightInfo& info);
};

struct tPixel
{
	BYTE r, g, b, a;
};

// 광선 구조체
struct tBloom
{
	Vec4  vBloomColor;
	float fThreshold;
	int	  iPadding[3];
};

// 광선 구조체
struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// Raycast 결과를 받을 구조체
struct tRaycastOut
{
	Vec2  vUV;
	float fDist;
	int	  bSuccess;
};

struct tParticle
{
	Vec4  vLocalPos;	   // 로컬 위치
	Vec4  vWorldPos;	   // 윌드 위치
	Vec4  vWorldInitScale; // 초기 크기
	Vec4  vWorldScale;	   // 크기
	Vec4  vWorldRotation;  // 회전값
	Vec3  vVelocity;	   // 속도
	Vec4  vColor;		   // 색상
	Vec4  vForce;		   // 입자에 적용된 누적 힘 총량
	Vec3  vNoiseForce;	   // NoiseForce 모듈로 인한 랜덤 힘
	float NoiseForceTime;  // NoiseForce 를 세팅받은 시간

	float NormalizeAge; // Age 를 Life 기준으로 정규화한 값
	float Mass;			// 질량
	float Age;			// 현재 나이
	float Life;			// 수명
	int	  Active;		// 활성화, 비활성화 여부

	int	  CurFrame;	 // 현재 애니메이션 프레임
	float FrameTime; // 현재 프레임 시간

	int ParticlePadding[2];
};

struct tParticleModule
{
	// Sapwn 모듈
	Vec4 vSpawnColor;	 // 초기 컬러
	Vec4 vSpawnMinScale; // 초기 최소 크기
	Vec4 vSpawnMaxScale; // 초기 최대 크기

	int	  SpawnType;	  // 0 : Continuous, 1 : Burst
	float BurstInterval;  // Burst 간격
	float MinLife;		  // 최소 수명
	float MaxLife;		  // 최대 수명
	float MinMass;		  // 최소 질량
	float MaxMass;		  // 최대 질량
	int	  SpawnRate;	  // 초당 생성 개수
	int	  SpaceType;	  // 좌표계(0 : LocalSpace, 1 : WorldSpace)
	int	  SpawnShape;	  // 스폰 범위(0 : Sphere, 1 : Box)
	float Radius;		  // SpawnShape 가 Sphere 인 경우, 반지름 길이
	Vec4  vSpawnBoxScale; // SpawnShape 가 Box 인 경우, Box 의 크기

	// Add Velocity
	int	  AddVelocityType; // 0 : From Center, 1: To Center, 2: Fix Direction
	float MinSpeed;
	float MaxSpeed;
	float FixedAngle;	  // 해당 방향에서 랜덤범위 각도
	Vec4  FixedDirection; // 지정 방향

	// Scale
	Vec4 vScaleRatio;

	// Noise Force
	float NoiseForceScale;
	float NoiseForceTerm;

	// Drag
	float DragTime; // 감속시키는데 걸리는 시간

	// Render
	int	  VelocityAlignment; // 1 : On, 0 : Off
	int	  AlphaBasedLife;	 // 0 : off, 1 : NomrlizedAge, 2: Age
	float AlphaMaxAge;

	// Animation
	int TileX;
	int TileY;

	// Module On / Off
	int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
	int				 modulepadding[2];

	friend ofstream& operator<<(ofstream& fout, const tParticleModule& module);
	friend ifstream& operator>>(ifstream& fin, tParticleModule& module);
};

struct tSpawnCount
{
	int SpawnCount;
	int iPadding[3];
};

// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vec4 vTranslate;
	Vec4 vScale;
	Vec4 qRot;
};

struct tMTKeyFrame
{
	double dTime;
	int	   iFrame;
	Vec3   vTranslate;
	Vec3   vScale;
	Vec4   qRot;
};

struct tMTBone
{
	wstring						strBoneName;
	int							iDepth;
	int							iParentIndx;
	Matrix						matOffset; // Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix						matBone;   // 이거 안씀
	vector<vector<tMTKeyFrame>> vecKeyFrame;
};

struct tMTAnimClip
{
	wstring strAnimName;
	int		iStartFrame;
	int		iEndFrame;
	int		iFrameLength;

	double dStartTime;
	double dEndTime;
	double dTimeLength;
	float  fUpdateTime; // 이거 안씀

	FbxTime::EMode eMode;

	bool operator==(const wstring& other) const { return strAnimName == other; }
};

// ===========
// Instancing
// ===========
union uInstID {
	struct
	{
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	ULONG64 llID;
};

class CGameObject;
struct tInstObj
{
	CGameObject* pObj;
	UINT		 iMtrlIdx;
};

struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int	   iRowIdx;
};

// ==================
// 상수버퍼 대응 구조체
// ==================
struct tTransform
{
	Matrix matWorld;
	Matrix matWorldInv;

	Matrix matView;
	Matrix matViewInv;

	Matrix matProj;
	Matrix matProjInv;

	Matrix matWV;
	Matrix matWVP;
};
extern tTransform g_Transform;

// Material 계수
struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};

struct tMtrlConst
{
	tMtrlData mtrl;

	int	   bArr[4];
	int	   iArr[4];
	float  fArr[4];
	Vec2   v2Arr[4];
	Vec4   v4Arr[4];
	Matrix matArr[4];
	int	   bTex[(UINT)TEX_PARAM::END];

	// 3D Animation 정보
	int arrAnimData[2];

	// 블룸 정보
	int iBloomUse;
	int iPadding[3];

	friend ofstream& operator<<(ofstream& fout, const tMtrlConst& _mtrlConst);
	friend ifstream& operator>>(ifstream& fin, tMtrlConst& _mtrlConst);
};

struct tAnimData2D
{
	Vec2 vLeftTop;
	Vec2 vSlizeSize;
	Vec2 vBackGround;
	Vec2 vOffset;
	int	 UseAnim2D;
	Vec3 vPadding;
};

struct tGlobalData
{
	Vec2 g_RenderResolution; // 렌더링 해상도
	Vec2 g_NoiseTexResolution;

	float g_dt;	  // Delta Time
	float g_time; // 누적 시간

	float g_EngineDT;	// Engine Delta Time
	float g_EngineTime; // Engine 누적 시간

	int g_Light2DCount; // 2D 광원 개수
	int g_Light3DCount; // 3D 광원 개수

	Vec2 padding;
};
extern tGlobalData g_global;

struct tLog
{
	double	  m_dTime;
	Log_Level m_LogLv;
	string	  m_strMsg;
};

struct LayerMap
{
	int	   LayerIdx;
	string LayerName;
};

#include <FontEngine/FW1FontWrapper.h>
struct tFontInfo
{
	wstring WStr; // 출력할 문자열

	// X, Y - 윈도우 좌표 기준 x, y
	// Z - 깊이 판정을 위한 용도
	Vec3 vPos;

	float fFontSize; // 폰트의 크기

	UINT Color; // 폰트의 색상. FONT_RGBA 매크로 함수를 사용하세요.

	FONT_TYPE FontType = FONT_TYPE::ARIAL; // 폰트 타입. 어떤 폰트를 사용할 것인지

	// 텍스트 플래그.
	// FW1_RESTORESTATE - 폰트를 그린 후 디바이스, 컨텍스트를 원래 상태로 만듦
	// FW1_CENTER - 중앙 정렬
	// FW1_LEFT - 왼 쪽 정렬
	FW1_TEXT_FLAG TextFlag = FW1_RESTORESTATE;
};

struct AfterImageInfo
{
	int	   NodeCount; // max는 10개
	Matrix WorldTransform[10];
	float  TimeStep;
	Vec2   Padding;

	// 에니메이션 정보
	int	  AnimationClipIdx[10];
	float AnimationRatio[10];
};