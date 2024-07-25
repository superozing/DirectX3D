#include "pch.h"
#include "CDamagedDirectionMgr.h"
#include "CDamagedDirection.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CImageUIScript.h"

CDamagedDirectionMgr::CDamagedDirectionMgr()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEDDIRECTIONMGR)
	, m_iDirectionCnt(3)
{
	AppendScriptParam("Spawn Cnt", SCRIPT_PARAM::INT, &m_iDirectionCnt);
}

CDamagedDirectionMgr::CDamagedDirectionMgr(const CDamagedDirectionMgr& _Origin)
	: CScript((UINT)SCRIPT_TYPE::DAMAGEDDIRECTIONMGR)
	, m_iDirectionCnt(_Origin.m_iDirectionCnt)
{
	AppendScriptParam("Spawn Cnt", SCRIPT_PARAM::INT, &m_iDirectionCnt);
}

CDamagedDirectionMgr::~CDamagedDirectionMgr()
{
}

void CDamagedDirectionMgr::begin()
{
	// 1. 배열에 집어넣기
	for (int i = 0; i < m_iDirectionCnt; ++i)
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"DamagedDirection " + to_wstring(i));

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		CDamagedDirection* pDmgScript = new CDamagedDirection;
		m_arrDamagedDirectionUI.push_back(pDmgScript);
		pObj->AddComponent(pDmgScript);
		pObj->AddComponent(new CImageUIScript);

		pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

		auto pMR = pObj->MeshRender();

		pMR->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
		pMR->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
		pMR->GetDynamicMaterial(0);
		pMR->GetMaterial(0)->SetShader(
			CAssetMgr::GetInst()->Load<CGraphicsShader>(L"GraphicsShader/DamagedDirection.gs"));

		GetOwner()->AddChild(pObj);
	}
}

void CDamagedDirectionMgr::tick()
{
	// 매 틱마다 알파 값 줄이기
	for (int i = 0; i < m_iDirectionCnt; ++i)
		m_arrDamagedDirectionUI[i]->SetAlpha(m_arrDamagedDirectionUI[i]->GetAlpha() - DT);

	// 임시로 키 입력 시 피격 효과 추가
	if (KEY_TAP(KEY::B))
	{
		AddDamagedDirection(Vec3(0.f, 0.f, 0.f), 0.1f);
	}
}

void CDamagedDirectionMgr::SetParentPanelUI()
{
}

void CDamagedDirectionMgr::AddDamagedDirection(Vec3 _EnemyPos, float _RadiusRatio)
{
	m_arrDamagedDirectionUI[m_Idx]->SetAlpha(0.5f);
	m_arrDamagedDirectionUI[m_Idx]->SetDamageRatio(_RadiusRatio);
	m_arrDamagedDirectionUI[m_Idx]->SetEnemyPos(_EnemyPos);

	AddIdx();
}

/////

void CDamagedDirectionMgr::SaveToFile(FILE* _File)
{
}

#define TagSpawnCnt "[SpawnCnt]"

void CDamagedDirectionMgr::SaveToFile(ofstream& fout)
{
	fout << TagSpawnCnt << endl;
	fout << m_iDirectionCnt << endl;
}

void CDamagedDirectionMgr::LoadFromFile(FILE* _File)
{
}

void CDamagedDirectionMgr::LoadFromFile(ifstream& fin)
{
	// Utils::GetLineUntilString(fin, TagSpawnCnt);
	// fin >> m_iDirectionCnt;
}
