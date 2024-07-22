#include "pch.h"
#include "CDamagedDirectionMgr.h"
#include "CDamagedDirection.h"

CDamagedDirectionMgr::CDamagedDirectionMgr()
	: CScript(SCRIPT_TYPE::DAMAGEDDIRECTIONMGR)
{
}

CDamagedDirectionMgr::~CDamagedDirectionMgr()
{
}

void CDamagedDirectionMgr::begin()
{
	// 1. 배열에 집어넣기
	for (int i = 0; i < 3; ++i)
	{
		CGameObject* pObj		   = new CGameObject;
		m_arrDamagedDirectionUI[i] = new CDamagedDirection;
		m_arrDamagedDirectionUI[i]->SetName(L"DamagedDirection " + to_wstring(i));
		m_arrDamagedDirectionUI[i]->SetMaxDamageRadius(XM_2PI / 3);
		m_arrDamagedDirectionUI[i]->SetAlpha(0.f);
		m_arrDamagedDirectionUI[i]->SetDamageRatio(0.f);

		// 2. 오브젝트에 부착 후 레벨에 추가하기
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(m_arrDamagedDirectionUI[i]);
		pObj->SetName(m_arrDamagedDirectionUI[i]->GetName());

		GamePlayStatic::SpawnGameObject(pObj, (UINT)LAYER::LAYER_UI);
	}

	
}

void CDamagedDirectionMgr::tick()
{
	// 매 틱마다 알파 값 줄이기
	
}

void CDamagedDirectionMgr::SetParentPanelUI()
{
}

void CDamagedDirectionMgr::AddDamagedDirection(Vec3 _EnemyPos, float _RadiusRatio)
{
	m_arrDamagedDirectionUI[m_Idx]



	

}

void CDamagedDirectionMgr::SaveToFile(FILE* _File)
{
}

void CDamagedDirectionMgr::SaveToFile(ofstream& fout)
{
}

void CDamagedDirectionMgr::LoadFromFile(FILE* _File)
{
}

void CDamagedDirectionMgr::LoadFromFile(ifstream& fin)
{
}
