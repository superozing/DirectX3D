#include "pch.h"
#include "CRenderComponent.h"

#include "CLevelMgr.h"
#include "CLevel.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_DrawShadowMap(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _OriginRenderCom)
	: CComponent(_OriginRenderCom)
	, m_Mesh(_OriginRenderCom.m_Mesh)
{
	m_vecMtrls.resize(_OriginRenderCom.m_vecMtrls.size());

	for (size_t i = 0; i < _OriginRenderCom.m_vecMtrls.size(); ++i)
	{
		m_vecMtrls[i].pCurMtrl = _OriginRenderCom.m_vecMtrls[i].pCurMtrl;
		m_vecMtrls[i].pSharedMtrl = _OriginRenderCom.m_vecMtrls[i].pSharedMtrl;

		// 원본 오브젝트가 공유재질을 참조하고 있고, 현재 사용재질은 공유재질이 아닌경우
		if (_OriginRenderCom.m_vecMtrls[i].pSharedMtrl != _OriginRenderCom.m_vecMtrls[i].pCurMtrl)
		{
			assert(_OriginRenderCom.m_vecMtrls[i].pDynamicMtrl.Get());

			// 복사 렌더 컴포넌트도 별도의 동적재질을 생성한다.
			GetDynamicMaterial(i);

			// 원본 렌더컴포넌트의 동적재질 값을 현재 생성한 동적재질로 복사한다.
			*m_vecMtrls[i].pDynamicMtrl.Get() = *_OriginRenderCom.m_vecMtrls[i].pDynamicMtrl.Get();
		}
		else
		{
			m_vecMtrls[i].pCurMtrl = m_vecMtrls[i].pSharedMtrl;
		}
	}
}

CRenderComponent::~CRenderComponent()
{

}

void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
	m_Mesh = _Mesh;

	if (!m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}

	if (nullptr != m_Mesh)
		m_vecMtrls.resize(m_Mesh->GetSubsetCount());
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx)
{
	// 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
	m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
	m_vecMtrls[_idx].pCurMtrl = _Mtrl;
	m_vecMtrls[_idx].pDynamicMtrl = nullptr;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
	return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
	// 공유재질을 가져오는것으로 현재 사용재질을 동적재질에서 회복하도록 한다
	m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

	if (m_vecMtrls[_idx].pDynamicMtrl.Get())
	{
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
	}

	return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
	//CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	//if (pCurLevel->GetState() != LEVEL_STATE::PLAY)
	//	return nullptr;

	// 원본 재질이 없다 -> Nullptr 반환
	if (nullptr == m_vecMtrls[_idx].pSharedMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = nullptr;
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
		return m_vecMtrls[_idx].pCurMtrl;
	}

	if (nullptr == m_vecMtrls[_idx].pDynamicMtrl)
	{
		m_vecMtrls[_idx].pDynamicMtrl = m_vecMtrls[_idx].pSharedMtrl->Clone();
		m_vecMtrls[_idx].pDynamicMtrl->SetName(m_vecMtrls[_idx].pSharedMtrl->GetName() + L"_Clone");
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pDynamicMtrl;
    
		m_vecMtrls[_idx].pDynamicMtrl->SetKey(m_vecMtrls[_idx].pSharedMtrl->GetKey());
		m_vecMtrls[_idx].pDynamicMtrl->SetRelativePath(m_vecMtrls[_idx].pSharedMtrl->GetRelativePath());

		m_vecMtrls[_idx].pCurMtrl->SetKey(m_vecMtrls[_idx].pSharedMtrl->GetKey());
		m_vecMtrls[_idx].pCurMtrl->SetRelativePath(m_vecMtrls[_idx].pSharedMtrl->GetRelativePath());
	}

	return nullptr;
}

void CRenderComponent::SaveToFile(FILE* _File)
{
	// 메쉬 참조정보 저장
	SaveAssetRef(m_Mesh, _File);

	// 재질 참조정보 저장
	UINT iMtrlCount = GetMtrlCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
	}

	fwrite(&m_DrawShadowMap, 1, 1, _File);
}

#define TagMesh "[Mesh]"
#define TagMtrlCount "[MaterialCount]"

void CRenderComponent::SaveToFile(ofstream& fout)
{
	// 메쉬 참조정보 저장
	fout << TagMesh << endl;
	SaveAssetRef(m_Mesh, fout);
	
	// 재질 참조정보 저장
	fout << TagMtrlCount << endl;
	UINT iMtrlCount = GetMtrlCount();
	fout << iMtrlCount << endl;

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveAssetRef(m_vecMtrls[i].pSharedMtrl, fout);
	}

	fout << m_DrawShadowMap << endl;
}

void CRenderComponent::LoadFromFile(FILE* _File)
{
	// 메쉬 참조정보 불러오기
	LoadAssetRef(m_Mesh, _File);

	// 재질 참조정보 불러오기
	UINT iMtrlCount = GetMtrlCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		LoadAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
	}

	fread(&m_DrawShadowMap, 1, 1, _File);
}

void CRenderComponent::LoadFromFile(ifstream& fin)
{
	// 메쉬 참조정보 불러오기
	Utils::GetLineUntilString(fin, TagMesh);
	LoadAssetRef(m_Mesh, fin);
	
	// 재질 참조정보 불러오기
	Utils::GetLineUntilString(fin, TagMtrlCount);
	UINT iMtrlCount = GetMtrlCount();
	fin >> iMtrlCount;
	m_vecMtrls.resize(iMtrlCount);

	m_vecMtrls.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		LoadAssetRef(m_vecMtrls[i].pSharedMtrl, fin);
		SetMaterial(m_vecMtrls[i].pSharedMtrl.Get(), i);
	}

	fin >> m_DrawShadowMap;
}
