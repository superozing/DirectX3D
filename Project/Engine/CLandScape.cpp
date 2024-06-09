#include "pch.h"
#include "CLandScape.h"

#include "CKeyMgr.h"
#include "CTransform.h"
#include "CRenderMgr.h"

#include "CCamera.h"


CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceX(64)
	, m_FaceZ(64)
	, m_TessDivide(1.f)
	, m_matEdgeTessFactor{}
	, m_BrushScale(Vec2(0.5f, 0.5f))
	, m_CrossBuffer(nullptr)
	, m_bTessDir(true)
	, m_fBrushPow(0.2f)
	, m_Mode(LANDSCAPE_MODE::NONE)
{
	Init();

	XMFLOAT4 row1 = XMFLOAT4(1.f, 4.f, 1000.f, 4000.f);
	XMFLOAT4 row2 = XMFLOAT4(1.f, 4.f, 1000.f, 4000.f);
	XMFLOAT4 row3 = XMFLOAT4(1.f, 4.f, 1000.f, 4000.f);
	XMFLOAT4 row4 = XMFLOAT4(1.f, 4.f, 1000.f, 4000.f);
	m_matEdgeTessFactor = Matrix(row1, row2, row3, row4);

}

CLandScape::~CLandScape()
{
}

void CLandScape::finaltick()
{
	if(KEY_TAP(KEY::NUM0))
		m_Mode = LANDSCAPE_MODE::NONE;
	else if (KEY_TAP(KEY::NUM1))
		m_Mode = LANDSCAPE_MODE::HEIGHT_MAP;
	else if (KEY_TAP(KEY::NUM2))
		m_Mode = LANDSCAPE_MODE::SPLAT;
	else if (KEY_TAP(KEY::NUM3))
	{
		m_WeightIdx++;
		if (3 <= m_WeightIdx)
			m_WeightIdx = 0;
	}

	if (LANDSCAPE_MODE::NONE == m_Mode)
	{
		return;
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		if (LANDSCAPE_MODE::HEIGHT_MAP == m_Mode)
		{
			// 교점 위치정보를 토대로 높이를 수정 함
			m_CSHeightMap->SetInputBuffer(m_CrossBuffer);	// 픽킹 정보를 HeightMapShader 에 세팅

			m_CSHeightMap->SetBrushTex(m_BrushTex);			// 사용할 브러쉬 텍스쳐 세팅			
			m_CSHeightMap->SetBrushIndex(0);				// 브러쉬 인덱스 설정
			m_CSHeightMap->SetBrushScale(m_BrushScale);		// 브러쉬 크기
			m_CSHeightMap->SetHeightMap(m_HeightMapTex);
			m_CSHeightMap->SetBrushPow(m_fBrushPow);
			m_CSHeightMap->SetTesDir(m_bTessDir);
			m_CSHeightMap->Execute();
		}

		else if (LANDSCAPE_MODE::SPLAT == m_Mode)
		{
			// 피킹 위치정보를 토대로 가중치를 수정함	
			m_CSWeightMap->SetInputBuffer(m_CrossBuffer);  // 레이 캐스트 위치
			m_CSWeightMap->SetBrushArrTex(m_BrushTex);
			m_CSWeightMap->SetBrushIndex(0);
			m_CSWeightMap->SetBrushScale(m_BrushScale); // 브러쉬 크기
			m_CSWeightMap->SetBrushPow(m_fBrushPow);
			m_CSWeightMap->SetWeightMap(m_WeightMapBuffer, m_WeightWidth, m_WeightHeight); // 가중치맵, 가로 세로 개수
			m_CSWeightMap->SetWeightIdx(m_WeightIdx);
			m_CSWeightMap->Execute();
		}

	}
}

void CLandScape::render()
{
	UpdateData();

	GetMesh()->render(0);

	// 가중치 버퍼 클리어(컴퓨터 쉐이더에서도 써야해서)
	m_WeightMapBuffer->Clear(17);
}

void CLandScape::UpdateData()
{
	Transform()->UpdateData();

	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceZ);
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_HeightMapTex);
	GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
  
  GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_1, m_TessDivide);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::MAT_0, m_matEdgeTessFactor);


	// 가중치 버퍼 전달
	m_WeightMapBuffer->UpdateData(17);

	// 가중치 버퍼 해상도 전달
	Vec2 vWeightMapResolution = Vec2((float)m_WeightWidth, (float)m_WeightHeight);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, vWeightMapResolution);

	// 타일 텍스쳐 전달
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEXARR_0, m_TileArrTex);

	//타일 배열 개수 전달
	float m_fTileCount = float(m_TileArrTex->GetArraySize() / 2); // 색상, 노말 합쳐져 있어서 2를 나눈다.
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fTileCount);

	// Camera World Pos 전달
	Vec3 vCamWorldPos = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldPos();
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, vCamWorldPos);

	// 재질정보 바인딩
	GetMaterial(0)->UpdateData();
}

void CLandScape::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_CrossBuffer->SetData(&out, 1);

	m_CSRaycast->SetHeightMap(m_HeightMapTex);
	m_CSRaycast->SetFaceCount(m_FaceX, m_FaceZ);
	m_CSRaycast->SetCameraRay(CamRay);
	m_CSRaycast->SetOuputBuffer(m_CrossBuffer);

	m_CSRaycast->Execute();

	m_CrossBuffer->GetData(&out);

}


#define TagFaceX "[FaceX]"
#define TagFaceZ "[FaceZ]"
#define TagTessDivede "[TessDivide]"
#define TagMatMatrix "[TessFactor]"
#define TagHeightMapTex "[HeighMapTex]"

void CLandScape::SaveToFile(ofstream& fout)
{
	fout << TagFaceX << endl;
	fout << m_FaceX << endl;

	fout << TagFaceZ << endl;
	fout << m_FaceZ << endl;

	fout << TagTessDivede << endl;
	fout << m_TessDivide.x << " " << m_TessDivide.y << " " << m_TessDivide.z << " " << m_TessDivide.w << endl;

	fout << TagMatMatrix << endl;
	fout << m_matEdgeTessFactor._11 << " " << m_matEdgeTessFactor._12 << " " << m_matEdgeTessFactor._13 << " " << m_matEdgeTessFactor._14 << endl;
	fout << m_matEdgeTessFactor._21 << " " << m_matEdgeTessFactor._22 << " " << m_matEdgeTessFactor._23 << " " << m_matEdgeTessFactor._24 << endl;
	fout << m_matEdgeTessFactor._31 << " " << m_matEdgeTessFactor._32 << " " << m_matEdgeTessFactor._33 << " " << m_matEdgeTessFactor._34 << endl;
	fout << m_matEdgeTessFactor._41 << " " << m_matEdgeTessFactor._42 << " " << m_matEdgeTessFactor._43 << " " << m_matEdgeTessFactor._44 << endl;

	fout << TagHeightMapTex << endl;
	SaveAssetRef(m_HeightMapTex, fout);


}

void CLandScape::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagFaceX);
	fin >> m_FaceX;

	Utils::GetLineUntilString(fin, TagFaceZ);
	fin >> m_FaceZ;

	Utils::GetLineUntilString(fin, TagTessDivede);
	fin >> m_TessDivide.x >> m_TessDivide.y >> m_TessDivide.z >> m_TessDivide.w;

	Utils::GetLineUntilString(fin, TagMatMatrix);
	fin >> m_matEdgeTessFactor._11 >> m_matEdgeTessFactor._12 >> m_matEdgeTessFactor._13 >> m_matEdgeTessFactor._14;
	fin >> m_matEdgeTessFactor._21 >> m_matEdgeTessFactor._22 >> m_matEdgeTessFactor._23 >> m_matEdgeTessFactor._24;
	fin >> m_matEdgeTessFactor._31 >> m_matEdgeTessFactor._32 >> m_matEdgeTessFactor._33 >> m_matEdgeTessFactor._34;
	fin >> m_matEdgeTessFactor._41 >> m_matEdgeTessFactor._42 >> m_matEdgeTessFactor._43 >> m_matEdgeTessFactor._44;

	Utils::GetLineUntilString(fin, TagHeightMapTex);
	LoadAssetRef(m_HeightMapTex, fin);
}

void CLandScape::SetLandScapeFace(UINT face, bool bXaxis)
{
	if (bXaxis)
	{
		m_FaceX = face;
	}
	else
	{
		m_FaceZ = face;
	}
}