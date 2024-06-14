#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

void CAssetMgr::init()
{
	InitSound();

	CreateDefaultMesh();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CAssetMgr::CreateDefaultMesh()
{
	CMesh* pMesh = nullptr;

	Vtx vPoint;
	UINT Idx = 0;
	pMesh = new CMesh(true);
	pMesh->Create(&vPoint, 1, &Idx, 1);
	AddAsset(MESHpoint, pMesh);


	// 전역변수에 삼각형 위치 설정
	//   0(Red)-- 1(Blue)	     
	//    |   \   |	     
	//   3(G)---- 2(Magenta)  
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);	
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);	
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	

	// 인덱스
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(2);
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(MESHrect, pMesh);
		
	vecIdx.clear();

	// Topology LineStrip 용도
	//   0(Red)-- 1(Blue)	     
	//    |       |	     
	//   3(G)---- 2(Magenta)   
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);	

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(MESHrectdebug, pMesh);

	vecVtx.clear();
	vecIdx.clear();


	// =================
	// CircleMesh 만들기
	// =================
	
	

	// 중심 점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT iSlice = 40;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (UINT i = 0; i <= iSlice; ++i)
	{
		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHcircle, pMesh);


	// CircleMesh_Debug
	vecIdx.clear();
	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHcircledebug, pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// =================
	// CrossMesh 만들기
	// =================
	v.vPos = Vec3(0.f, 0.5f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.f, -0.5f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHcross, pMesh);
	vecVtx.clear();
	vecIdx.clear();


	// ==========
	// Cube Mesh
	// ==========
	// 6개의 각 면 마다 각자의 방향벡터(노말, 탄젠트, 바이노말) 이 세팅되어야 하기 때문에
	// 면 단위로 정점 4개씩 6면, 총 24개의 정점이 필요하다.	
	Vtx arrCube[24] = {};
	 
	// 윗면
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// 뒷 면
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// 앞 면
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHcube, pMesh);
	vecIdx.clear();

	// ========================
	// Cube Mesh
	// Topology Line Strip 용도
	// ========================
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	vecIdx.push_back(7);
	vecIdx.push_back(6);
	vecIdx.push_back(5);
	vecIdx.push_back(4);
	vecIdx.push_back(7);

	vecIdx.push_back(6);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(5);
	vecIdx.push_back(4);
	vecIdx.push_back(3);

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHcubedebug, pMesh);
	vecIdx.clear();


	// ===========
	// Sphere Mesh
	// ===========
	fRadius = 0.5f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vNormal.Cross(v.vTangent, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHsphere, pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Cone Mesh
	// ===========
	fRadius = 0.5f;
	float fHeight = 1.f;

	iSliceCount = 100; // 원뿔의 세로 분할 개수

	fSliceAngle = XM_2PI / iSliceCount;

	fUVXStep = 1.f / (float)iSliceCount;
	fUVYStep = 1.f;

	for (UINT i = 0; i <= iSliceCount; ++i)
	{
		// Top
		v.vPos = Vec3(0.f, 0.f, 0.f);
		v.vUV = Vec2(0.5f, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		// Body
		float theta = i * fSliceAngle;
		v.vPos = Vec3(fRadius * cosf(theta), fRadius * sinf(theta), fHeight);
		v.vUV = Vec2(fUVXStep * i, fUVYStep);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		// vecVtx 저장 순서 : {꼭대기1, 정점1, 꼭대기2, 정점2, ...}
		if (i > 0)
		{
			size_t lastVtxIdx = vecVtx.size() - 1;

			Vtx vtx1 = vecVtx[lastVtxIdx - 2];
			Vtx vtx2 = vecVtx[lastVtxIdx];

			vecVtx[lastVtxIdx - 3].vNormal = vtx1.vNormal = vtx2.vPos.Cross(vtx1.vPos).Normalize();
			vecVtx[lastVtxIdx - 3].vBinormal = vtx1.vBinormal = -vtx1.vPos.Normalize();
			vecVtx[lastVtxIdx - 3].vTangent = vtx1.vTangent = vtx1.vBinormal.Cross(vtx1.vNormal).Normalize();

			//vecVtx[lastVtxIdx].vNormal = vtx2.vNormal = vtx1.vPos.Cross(vtx2.vPos).Normalize();
			//vecVtx[lastVtxIdx].vBinormal = vtx2.vBinormal = -vtx2.vPos.Normalize();
			//vecVtx[lastVtxIdx].vTangent = vtx2.vTangent = vtx2.vBinormal.Cross(vtx2.vNormal).Normalize();
			
			//// Binormal : 아랫면 정점에서 원뿔 꼭대기를 향하는 방향 벡터
			//vecVtx[lastVtxIdx - 3] .vBinormal = vtx1.vBinormal = (vecVtx[lastVtxIdx - 3].vPos - vtx1.vPos).Normalize();
			//
			//// Tangent : 정점2에서 정점1로 향하는 방향 벡터
			//vecVtx[lastVtxIdx - 3].vTangent = vtx1.vTangent = (vtx2.vPos - vtx1.vPos).Normalize();

			//// Normal : Binormal과 Tangent의 외적
			//vecVtx[lastVtxIdx - 3].vNormal = vtx1.vNormal = (vtx1.vBinormal).Cross(vtx1.vTangent).Normalize();
		}
	}

	for (UINT i = 0; i <= iSliceCount; ++i)
	{
		// 인덱스
		if (i < iSliceCount)
		{
			vecIdx.push_back(2 * i);	
			vecIdx.push_back(2 * i + 3);
			vecIdx.push_back(2 * i + 1);
		}
	}

	for (UINT i = 0; i <= iSliceCount; ++i)
	{
		float theta = i * fSliceAngle;

		v.vPos = Vec3(fRadius * cosf(theta), fRadius * sinf(theta), fHeight);
		v.vUV = Vec2(fUVXStep * i, fUVYStep);
		v.vNormal = Vec3(0.f, 0.f, 1.f);
		v.vTangent = Vec3(1.f, 0.f, 0.f);
		v.vBinormal = Vec3(0.f, 1.f, 0.f);
		vecVtx.push_back(v);
	}

	// 아래면의 중심 정점 추가
	v.vPos = Vec3(0.f, 0, 1.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(MESHcone, pMesh);
	vecVtx.clear();
	vecIdx.clear();

	//// Top
	//v.vPos = Vec3(0.f, 0.f, 0.f);
	//v.vUV = Vec2(0.5f, 0.f);
	//v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	//v.vNormal = Vec3(0.f, 0.f, -1.f);
	//v.vTangent = Vec3(1.f, 0.f, 0.f);
	//v.vBinormal = Vec3(0.f, 1.f, 0.f);
	//vecVtx.push_back(v);

	//// Body
	//iSliceCount = 80; // 원뿔의 세로 분할 개수

	//fSliceAngle = XM_2PI / iSliceCount;

	//fUVXStep = 1.f / (float)iSliceCount;
	//fUVYStep = 1.f;

	//for (UINT i = 0; i <= iSliceCount; ++i)
	//{
	//	float theta = i * fSliceAngle;

	//	float x = fRadius * cosf(theta);
	//	float y = fRadius * sinf(theta);

	//	v.vPos = Vec3(x, y, fHeight);
	//	v.vUV = Vec2(fUVXStep * i, fUVYStep);
	//	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	//	v.vNormal = Vec3(0.f, 0.f, 1.f);
	//	v.vTangent = Vec3(1.f, 0.f, 0.f);
	//	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	//	vecVtx.push_back(v);

	//	// 인덱스
	//	if (i < iSliceCount)
	//	{
	//		vecIdx.push_back(0);
	//		vecIdx.push_back(i + 2);
	//		vecIdx.push_back(i + 1);
	//	}

	//	// 정점의 법선 벡터 다시 계산
	//	if (i > 0)
	//	{
	//		size_t lastVtxIdx = vecVtx.size() - 1;

	//		Vec3 vtx1 = vecVtx[lastVtxIdx - 1].vPos; // 가장 마지막 바로 전 VtxIdx
	//		Vec3 vtx2 = vecVtx[lastVtxIdx].vPos;     // 가장 마지막에 추가된 VtxIdx

	//		// 아랫면의 한 정점을 기준으로 연결된 두 벡터를 외적하여 노말 벡터를 재계산
	//		Vec3 vNormal = (vtx1 - vecVtx[0].vPos).Cross(vtx1 - vtx2).Normalize();

	//		vecVtx[lastVtxIdx - 1].vNormal = vNormal;
	//		vecVtx[lastVtxIdx].vNormal = vNormal;
	//	}
	//}
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
	// =================================
	// Std2DShader
	// =================================
	Ptr<CGraphicsShader> pShader = nullptr;

	pShader = new CGraphicsShader;	
	pShader->CreateVertexShader(FXstd2d, FN_VS_std2d);
	pShader->CreatePixelShader(FXstd2d, FN_PS_std2d);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		
	// Parameter	
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Test Param");
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Output Texture 1");
	
	AddAsset(SHADER_std2d, pShader.Get());



	// ===========
	// Std3DShader
	// ===========
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd3d, FN_VS_std3d);
	pShader->CreatePixelShader(FXstd3d, FN_PS_std3d);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Lighting Type", 0.f, 0.f, false, "0 : Gouraud, 1 : Phong");
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Output Texture");
	pShader->AddTexParam(TEX_PARAM::TEX_1, "Normal Texture");

	AddAsset(SHADER_std3d, pShader.Get());

	// ====================
	// Std3D_DeferredShader
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd3d_deferred, FN_VS_deferred);
	pShader->CreatePixelShader(FXstd3d_deferred, FN_PS_deferred);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "test", 0.f, 1.f);

	AddAsset(L"Std3D_DeferredShader", pShader.Get());

	// ==============
	// DirLightShader
	// ==============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXlight, FN_VS_dirlight);
	pShader->CreatePixelShader(FXlight, FN_PS_dirlight);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	AddAsset(SHADER_dirlight, pShader.Get());

	// =================
	// PointLight Shader
	// =================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXlight, FN_VS_pointlight);
	pShader->CreatePixelShader(FXlight, FN_PS_pointlight);

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	AddAsset(SHADER_pointlight, pShader.Get());

	// =================
	// SpotLight Shader
	// =================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXlight, FN_VS_spotlight);
	pShader->CreatePixelShader(FXlight, FN_PS_spotlight);

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	AddAsset(SHADER_spotlight, pShader.Get());

	// ============
	// Merge Shader
	// ============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXmerge, FN_VS_merge);
	pShader->CreatePixelShader(FXmerge, FN_PS_merge);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MERGE);

	AddAsset(SHADER_merge, pShader.Get());

	// ============
	// Decal Shader
	// ============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DECAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

	// Parameter
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Decal Texture");

	AddAsset(L"DecalShader", pShader.Get());

	// =================================
	// EffectShader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd2d, FN_VS_std2d);
	pShader->CreatePixelShader(FXstd2d, FN_PS_effect);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(SHADER_effect, pShader.Get());


	// =============
	// TileMapShader
	// =============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXtilemap, FN_VS_tile);
	pShader->CreatePixelShader(FXtilemap, FN_PS_tile);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(SHADER_tilemap, pShader.Get());

	// ====================
	// ParticleRenderShader
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXparticle, FN_VS_particle);
	pShader->CreateGeometryShader(FXparticle, FN_GS_particle);
	pShader->CreatePixelShader(FXparticle, FN_PS_particle);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	
	AddAsset(SHADER_particlerender, pShader.Get());

	// =================================
	// GrayFilter Shader
	// Mesh			: RectMesh
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXpostprocess, FN_VS_grayfilter);
	pShader->CreatePixelShader(FXpostprocess, FN_PS_grayfilter);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(SHADER_grayfilter, pShader.Get());

	// ============
	// SkyBoxShader
	// ============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXskybox, FN_VS_skybox);
	pShader->CreatePixelShader(FXskybox, FN_PS_skybox);

	pShader->SetRSType(RS_TYPE::CULL_FRONT);

	// SkyBox 는 최대깊이인 1 로 그려질 예정, 따라서 초기화 값인 Depth 1 이랑 같은 경우까지도 통과시켜줘야 한다.
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddAsset(SHADER_skybox, pShader.Get());

	// =================================
	// Distortion Shader	
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXpostprocess, FN_VS_distortion);
	pShader->CreatePixelShader(FXpostprocess, FN_PS_distortion);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(SHADER_distortion, pShader.Get());

	// =================================
	// DebugShape Shader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXdebug, FN_VS_debugshape);
	pShader->CreatePixelShader(FXdebug, FN_PS_debugshape);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	AddAsset(SHADER_debugshape, pShader.Get());

	// ================
	// ShadowMap Shader
	// ================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\shadowmap.fx", "VS_ShadowMap");
	pShader->CreatePixelShader(L"shader\\shadowmap.fx", "PS_ShadowMap");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_SHADOWMAP);

	AddAsset(L"ShadowMapShader", pShader.Get());


	// ===========
	// Tess Shader
	// ===========
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tess.fx", "VS_Tess");
	pShader->CreateHullShader(L"shader\\tess.fx", "HS_Tess");
	pShader->CreateDomainShader(L"shader\\tess.fx", "DS_Tess");
	pShader->CreatePixelShader(L"shader\\tess.fx", "PS_Tess");

	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Tess Factor");

	AddAsset(L"TessShader", pShader.Get());

	// ================
	// LandScape Shader
	// ================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\landscape.fx", "VS_LandScape");
	pShader->CreateHullShader(L"shader\\landscape.fx", "HS_LandScape");
	pShader->CreateDomainShader(L"shader\\landscape.fx", "DS_LandScape");
	pShader->CreatePixelShader(L"shader\\landscape.fx", "PS_LandScape");

	pShader->AddScalarParam(SCALAR_PARAM::VEC4_1, "Tess Divide");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "Tess Power");


	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	AddAsset(L"LandScapeShader", pShader.Get());
}



void CAssetMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	string strShaderPath;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_std2d));
	strShaderPath = "GraphicsShader\\Std2DShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_std2d, pMtrl);

	// Std3DMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_std3d));
	strShaderPath = "GraphicsShader\\Std3DShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_std3d, pMtrl);

	// Std3D_DeferredMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_deferred));
	strShaderPath = "GraphicsShader\\Std3D_DeferredShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_deferred, pMtrl);

	// DirLightMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_dirlight));
	strShaderPath = "GraphicsShader\\DirLightShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, FindAsset<CTexture>(L"NormalTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, FindAsset<CTexture>(L"ShadowDepthTargetTex"));
	AddAsset<CMaterial>(MTRL_dirlight, pMtrl);

	// PointLightMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_pointlight));
	strShaderPath = "GraphicsShader\\PointLightShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, FindAsset<CTexture>(L"NormalTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, FindAsset<CTexture>(L"ShadowDepthTargetTex"));
	AddAsset<CMaterial>(MTRL_pointlight, pMtrl);

	// SpotLightMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_spotlight));
	strShaderPath = "GraphicsShader\\SpotLightShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, FindAsset<CTexture>(L"NormalTargetTex"));
	AddAsset<CMaterial>(MTRL_spotlight, pMtrl);

	// MergeMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_merge));
	strShaderPath = "GraphicsShader\\MergeShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"ColorTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"DiffuseTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"SpecularTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_3, CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"));
	AddAsset<CMaterial>(MTRL_merge, pMtrl);

	// DecalMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DecalShader"));
	strShaderPath = "GraphicsShader\\DecalShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	AddAsset<CMaterial>(MTRL_decal, pMtrl);

	// BackgroundMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_std2d));
	strShaderPath = "GraphicsShader\\Std2DShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_background, pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_tilemap));
	strShaderPath = "GraphicsShader\\TileMapShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_tilemap, pMtrl);

	// ParticleMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_particlerender));
	strShaderPath = "GraphicsShader\\ParticleRenderShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_particle, pMtrl);

	// SkyBoxMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_skybox));
	strShaderPath = "GraphicsShader\\SkyBoxShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_skybox, pMtrl);

	// GrayFilterMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_grayfilter));
	strShaderPath = "GraphicsShader\\GrayFilterShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_grayfilter, pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_distortion));
	strShaderPath = "GraphicsShader\\DistortionShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_distortion, pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(SHADER_debugshape));
	strShaderPath = "GraphicsShader\\DebugShapeShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset<CMaterial>(MTRL_debugshape, pMtrl);

	// ShadowMapMtrl
	pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ShadowMapShader"));
	strShaderPath = "GraphicsShader\\ShadowMapShader.gs";
	pMtrl->SetShader(Load<CGraphicsShader>(strShaderPath));
	AddAsset(L"ShadowMapMtrl", pMtrl);

	// TessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"TessShader"));
	AddAsset(L"TessMtrl", pMtrl);

	// LandScapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"LandScapeShader"));
	AddAsset(L"LandScapeMtrl", pMtrl);
}




#include "CSetColorShader.h"
#include "CParticleUpdate.h"
#include "CAnimation3DShader.h"
void CAssetMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pShader = nullptr;

	// SetColorShader
	pShader = new CSetColorShader;	
	AddAsset(SHADER_setcolor, pShader.Get());

	// ParticleUpdateShader
	pShader = new CParticleUpdate;
	AddAsset(SHADER_particleupdate, pShader.Get());

	// Animation3DUpdateShader
	pShader = new CAnimation3DShader;
	AddAsset(L"Animation3DUpdateCS", pShader.Get());
}


#include "CSound.h"
void CAssetMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}
