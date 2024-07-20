#include "pch.h"
#include "CDamagedDirection.h"
#include "CPanelUIScript.h"
#include "CImageUIScript.h"

CDamagedDirection::CDamagedDirection()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEDDIRECTION)
{
}

CDamagedDirection::~CDamagedDirection()
{
}

void CDamagedDirection::begin()
{
	SetParentPanelUI();

	// begin에서 해야하는 일.
	// 1. Damaged Direction에 이미지와 쉐이더 세팅
	// 2. 상수 버퍼에 필요한 정보를 바인딩 - 이건 tick에서 해도 되겠네요.
}

void CDamagedDirection::tick()
{

	// 1. 입력받은 적의 Front 벡터와 메인 카메라의 Front 벡터를 내적을 바인딩 - x 축을 제외한 y축, z축.

	// 2. 입력받은 데미지 비율에 따라서 표시할 각도 바인딩
}

void CDamagedDirection::SetParentPanelUI()
{

	auto pOwn = GetOwner();

	m_pPanelUI = pOwn->GetScript<CPanelUIScript>();

	if (!m_pPanelUI)
	{
		m_pPanelUI = new CPanelUIScript;
		pOwn->AddComponent(m_pPanelUI);
	}

	m_pPanelUI->DisableMouseInput();
	m_pPanelUI->DisallowDragAndDrop();
	m_pPanelUI->DisallowTexSet();
	m_pPanelUI->SetUIType(UI_TYPE::CROSSHAIR);

	auto meshrender = pOwn->MeshRender();

	if (!meshrender)
	{
		meshrender = new CMeshRender;
		pOwn->AddComponent(meshrender);
	}

	meshrender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	meshrender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	meshrender->GetDynamicMaterial(0);
}

//

void CDamagedDirection::SaveToFile(FILE* _File)
{
}

void CDamagedDirection::SaveToFile(ofstream& fout)
{
}

void CDamagedDirection::LoadFromFile(FILE* _File)
{
}

void CDamagedDirection::LoadFromFile(ifstream& fin)
{
}
