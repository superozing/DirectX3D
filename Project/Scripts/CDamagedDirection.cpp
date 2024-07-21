#include "pch.h"
#include "CDamagedDirection.h"
#include "CPanelUIScript.h"
#include "CImageUIScript.h"

#include <Engine/CRenderMgr.h>

CDamagedDirection::CDamagedDirection()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEDDIRECTION)
{
}

CDamagedDirection::~CDamagedDirection()
{
}

void CDamagedDirection::begin()
{
	AppendScriptParam("Enemy Front Vector", SCRIPT_PARAM::VEC3, &m_DamagedDirection);
	AppendScriptParam("Max Radius", SCRIPT_PARAM::FLOAT, &m_fMaxDamageRadius);
	AppendScriptParam("Damage Ratio", SCRIPT_PARAM::FLOAT, &m_DamageRadiusRatio);
	AppendScriptParam("AlphaBlend", SCRIPT_PARAM::FLOAT, &m_Alpha);

	SetParentPanelUI();

	m_fMaxDamageRadius = XM_PI;
	m_DamageRadiusRatio = 1.f;
	m_Alpha			   = 0.7f;

	CGameObject* pObj = new CGameObject;
	m_pImageUI = new CImageUIScript;

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pImageUI);

	m_pImageUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ShootingDirection.png"));
	m_pImageUI->AllowBindTexPerFrame();
	m_pImageUI->DisableMouseInput();

	m_pImageUI->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	auto pMR = pObj->MeshRender();
	
	pMR->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pMR->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pMR->GetDynamicMaterial(0);
	pMR->GetMaterial(0)->SetShader(CAssetMgr::GetInst()->Load<CGraphicsShader>(L"GraphicsShader/DamagedDirection.gs"));
	
	GetOwner()->AddChild(pObj);
}

void CDamagedDirection::tick()
{

	// 상수 버퍼에 필요한 정보를 바인딩
	// 1. 입력받은 적의 Front 벡터와 메인 카메라의 Front 벡터를 내적을 바인딩 - x 축을 제외한 y축, z축.
	Vec3 camDir = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Vec2 playerFront = Vec2(camDir.y, camDir.z);
	Vec2 enemyFront	 = Vec2(m_DamagedDirection.y, m_DamagedDirection.z);
	//playerFront.Dot(enemyFront)
	m_pImageUI->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0);

	// 2. 입력받은 데미지 비율에 따라서 표시할 각도 바인딩
	m_pImageUI->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1,
														m_fMaxDamageRadius * m_DamageRadiusRatio);

	// 3. 알파 값 바인딩
	m_pImageUI->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_2, m_Alpha);
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
