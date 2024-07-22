#include "pch.h"
#include "CDamagedDirection.h"
#include "CPanelUIScript.h"
#include "CImageUIScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CDamagedDirection::CDamagedDirection()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEDDIRECTION)
{
}

CDamagedDirection::~CDamagedDirection()
{
}

void CDamagedDirection::begin()
{
	AppendScriptParam("Enemy Pos", SCRIPT_PARAM::VEC3, &m_EnemyPos);
	AppendScriptParam("Max Radius", SCRIPT_PARAM::FLOAT, &m_fMaxDamageRadius);
	AppendScriptParam("Damage Ratio", SCRIPT_PARAM::FLOAT, &m_DamageRadiusRatio);
	AppendScriptParam("AlphaBlend", SCRIPT_PARAM::FLOAT, &m_Alpha);

	SetParentPanelUI();

	m_fMaxDamageRadius	= XM_PI;
	m_DamageRadiusRatio = 1.f;
	m_Alpha				= 0.7f;

	CGameObject* pObj = new CGameObject;
	m_pImageUI		  = new CImageUIScript;

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
	// 1. 입력받은 데미지 비율에 따라서 표시할 각도 바인딩
	m_pImageUI->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1,
																	m_fMaxDamageRadius * m_DamageRadiusRatio);

	// 2. 알파 값 바인딩
	m_pImageUI->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_2, m_Alpha);

	// 3. 회전 적용
	// 쉐이더 코드에서 위 쪽을 기준으로 렌더링 하기 때문에, 내적 결과가 -1 일 경우 0 만큼 회전, 1 일 경우 PI 만큼 z축
	// 회전.

	// 몬스터로부터 플레이어의 방향 벡터
	Vec3 MonToPlayer   = m_PlayerPos - m_EnemyPos;
	Vec2 MonToPlayerXZ = Vec2(MonToPlayer.x, MonToPlayer.z);

	// 플레이어의 front 벡터
	Vec2 PlayerDirXZ = Vec2(m_PlayerDir.x, m_PlayerDir.z);

	// 벡터 정규화
	MonToPlayerXZ.Normalize();
	PlayerDirXZ.Normalize();

	// 내적으로 회전 각도 계산
	float dotProduct = MonToPlayerXZ.Dot(PlayerDirXZ);
	float angle		 = acos(dotProduct);

	// 외적으로 회전 방향 계산
	float crossProduct = MonToPlayerXZ.x * PlayerDirXZ.y - MonToPlayerXZ.y * PlayerDirXZ.x;
	if (crossProduct > 0)
	{
		angle = -angle;
	}

	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angle));
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

void CDamagedDirection::SetEnemyPos(Vec3 _EnemyPos)
{
	m_EnemyPos = _EnemyPos;
	m_PlayerPos = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldPos();
	m_PlayerDir = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
}
