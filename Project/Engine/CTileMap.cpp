#include "pch.h"
#include "CTileMap.h"

#include "CStructuredBuffer.h"

#include "CAssetMgr.h"
#include "CMesh.h"

#include "CTransform.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_FaceX(2)
	, m_FaceY(2)
	, m_vTileRenderSize(Vec2(128.f, 128.f))	
	, m_TileInfoBuffer(nullptr)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));

	m_TileInfoBuffer = new CStructuredBuffer;

	SetFace(m_FaceX, m_FaceY);
}

CTileMap::CTileMap(const CTileMap& _OriginTileMap)
	: CRenderComponent(_OriginTileMap)
	, m_FaceX(_OriginTileMap.m_FaceX)
	, m_FaceY(_OriginTileMap.m_FaceY)
	, m_vTileRenderSize(_OriginTileMap.m_vTileRenderSize)	
	, m_TileAtlas(_OriginTileMap.m_TileAtlas)
	, m_vTilePixelSize(_OriginTileMap.m_vTilePixelSize)
	, m_vSliceSizeUV(_OriginTileMap.m_vSliceSizeUV)
	, m_MaxCol(_OriginTileMap.m_MaxCol)
	, m_MaxRow(_OriginTileMap.m_MaxRow)
	, m_vecTileInfo(_OriginTileMap.m_vecTileInfo)
	, m_TileInfoBuffer(nullptr)
{
	if (nullptr != _OriginTileMap.m_TileInfoBuffer)
	{
		m_TileInfoBuffer = _OriginTileMap.m_TileInfoBuffer->Clone();
	}	
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileInfoBuffer)
		delete m_TileInfoBuffer;
}

void CTileMap::finaltick()
{
	// (타일 개수 * 타일 사이즈) 로 사이즈를 변경처리한다.
	Vec3 vTileMapSize = Vec3(m_FaceX * m_vTileRenderSize.x, m_FaceY * m_vTileRenderSize.y, 1.f);
	Transform()->SetRelativeScale(vTileMapSize);
}

void CTileMap::render()
{	
	// 재질에 아틀라스 텍스쳐 전달.
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_TileAtlas);
	
	// 타일의 가로 세로 개수
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceY);
		
	// 아틀라스 이미지에서 타일 1개의 자르는 사이즈(UV 기준)
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, m_vSliceSizeUV);

	// 각 타일 정보를 구조화 버퍼로 이동
	m_TileInfoBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	// 타일 구조화 버퍼를 t20 에 바인딩
	m_TileInfoBuffer->UpdateData(20);

	// 재질 업데이트
	GetMaterial()->UpdateData();	

	Transform()->UpdateData();

	GetMesh()->render();
}


void CTileMap::UpdateData()
{

}


void CTileMap::SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize)
{
	m_TileAtlas = _Atlas;
	m_vTilePixelSize = _TilePixelSize;

	m_MaxCol = m_TileAtlas->GetWidth() / (UINT)m_vTilePixelSize.x;
	m_MaxRow = m_TileAtlas->GetHeight() / (UINT)m_vTilePixelSize.y;

	m_vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth()
		, m_vTilePixelSize.y / m_TileAtlas->GetHeight());
}

void CTileMap::SetFace(UINT _FaceX, UINT _FaceY)
{
	m_FaceX = _FaceX;
	m_FaceY = _FaceY;

	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_FaceX * _FaceY);

	m_TileInfoBuffer->Create(sizeof(tTileInfo), _FaceX * _FaceY, SB_TYPE::READ_ONLY, true);
}

void CTileMap::SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx)
{
	if (nullptr == m_TileAtlas)
		return;

	UINT idx = _Row* m_FaceX + _Col;

	// 렌더링할 타일 정보
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
								  , (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	m_vecTileInfo[idx].bRender = 1;
}

void CTileMap::SaveToFile(FILE* _File)
{
	// TileMap 정보 저장
	fwrite(&m_FaceX, sizeof(UINT), 1, _File);
	fwrite(&m_FaceY, sizeof(UINT), 1, _File);
	fwrite(&m_vTileRenderSize, sizeof(Vec2), 1, _File);
	fwrite(&m_vTileRenderSize, sizeof(Vec2), 1, _File);

	SaveAssetRef(m_TileAtlas, _File);

	fwrite(&m_vTilePixelSize, sizeof(Vec2), 1, _File);
	fwrite(&m_vSliceSizeUV, sizeof(Vec2), 1, _File);

	fwrite(&m_MaxCol, sizeof(UINT), 1, _File);
	fwrite(&m_MaxRow, sizeof(UINT), 1, _File);
	
	size_t InfoCount = m_vecTileInfo.size();
	fwrite(&InfoCount, sizeof(size_t), 1, _File);
	fwrite(m_vecTileInfo.data(), sizeof(tTileInfo), InfoCount, _File);
}

#define TagFaceX "[FaceX]"
#define TagFaceY "[FaceY]"
#define TagRenderSize "[RenderSize]"
#define TagAtlas "[Atlas]"
#define TagPixelSize "[PixelSize]"
#define TagSliceSize "[SliceSize]"
#define TagMaxCol "[MaxCol]"
#define TagMaxRow "[MaxRow]"
#define TagInfoCount "[InfoCount]"
#define TagTileInfo "[TileInfo]"

void CTileMap::SaveToFile(ofstream& fout)
{
	fout << TagFaceX << endl;
	fout << m_FaceX << endl;

	fout << TagFaceY << endl;
	fout << m_FaceY << endl;

	fout << TagRenderSize << endl;
	fout << m_vTileRenderSize << endl;

	fout << TagAtlas << endl;
	SaveAssetRef(m_TileAtlas, fout);

	fout << TagPixelSize << endl;
	fout << m_vTilePixelSize << endl;
	
	fout << TagSliceSize << endl;
	fout << m_vSliceSizeUV << endl;

	fout << TagMaxCol << endl;
	fout << m_MaxCol << endl;

	fout << TagMaxRow << endl;
	fout << m_MaxRow << endl;

	size_t InfoCount = m_vecTileInfo.size();
	fout << TagInfoCount << endl;
	fout << InfoCount << endl;

	if (InfoCount == 0) return;

	fout << TagTileInfo << endl;
	for (int i = 0; i < m_vecTileInfo.size(); i++) {
		fout << m_vecTileInfo[i] << endl;
	}
}

void CTileMap::LoadFromFile(FILE* _File)
{
	// TileMap 정보 저장
	fread(&m_FaceX, sizeof(UINT), 1, _File);
	fread(&m_FaceY, sizeof(UINT), 1, _File);
	fread(&m_vTileRenderSize, sizeof(Vec2), 1, _File);
	fread(&m_vTileRenderSize, sizeof(Vec2), 1, _File);

	LoadAssetRef(m_TileAtlas, _File);

	fread(&m_vTilePixelSize, sizeof(Vec2), 1, _File);
	fread(&m_vSliceSizeUV, sizeof(Vec2), 1, _File);

	fread(&m_MaxCol, sizeof(UINT), 1, _File);
	fread(&m_MaxRow, sizeof(UINT), 1, _File);

	size_t InfoCount = 0;
	fread(&InfoCount, sizeof(size_t), 1, _File);
	m_vecTileInfo.reserve(InfoCount);
	fread(m_vecTileInfo.data(), sizeof(tTileInfo), InfoCount, _File);
}

void CTileMap::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagFaceX);
	fin >> m_FaceX;

	Utils::GetLineUntilString(fin, TagFaceY);
	fin >> m_FaceY;

	Utils::GetLineUntilString(fin, TagRenderSize);
	fin >> m_vTileRenderSize;

	Utils::GetLineUntilString(fin, TagAtlas);
	LoadAssetRef(m_TileAtlas, fin);

	Utils::GetLineUntilString(fin, TagPixelSize);
	fin >> m_vTilePixelSize;

	Utils::GetLineUntilString(fin, TagSliceSize);
	fin >> m_vSliceSizeUV;

	Utils::GetLineUntilString(fin, TagMaxCol);
	fin >> m_MaxCol;

	Utils::GetLineUntilString(fin, TagMaxRow);
	fin >> m_MaxRow;

	size_t infoCnt;
	Utils::GetLineUntilString(fin, TagInfoCount);
	fin >> infoCnt;

	if (infoCnt == 0) return;

	Utils::GetLineUntilString(fin, TagTileInfo);
	for (int i = 0; i < m_vecTileInfo.size(); i++) {
		fin >> m_vecTileInfo[i];
	}
}

#define TagLeftTop "[LeftTop]"
#define TagRender "[IsRender]"

ofstream& operator<<(ofstream& fout, const tTileInfo& info)
{
	fout << TagLeftTop << endl;
	fout << info.vLeftTopUV << endl;

	fout << TagRender << endl;
	fout << info.bRender;

	return fout;
}

ifstream& operator>>(ifstream& fin, tTileInfo& info)
{
	Utils::GetLineUntilString(fin, TagLeftTop);
	fin >> info.vLeftTopUV;

	Utils::GetLineUntilString(fin, TagRender);
	fin >> info.bRender;
	
	return fin;
}
