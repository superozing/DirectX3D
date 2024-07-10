#include "pch.h"
#include "CAmmoInfo.h"
#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

CAmmoInfo::CAmmoInfo()
	: CProgressBar((UINT)SCRIPT_TYPE::AMMOINFO)
{
}

CAmmoInfo::~CAmmoInfo()
{
}

void CAmmoInfo::begin()
{
	CProgressBar::begin();
}

void CAmmoInfo::tick()
{
	m_pAmmoLine->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ratio);
}

void CAmmoInfo::MakeChildObjects()
{
	// transform 조정
	Transform()->SetRelativeScale(Vec3(16.f, 54.f, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Color/White.png"));
	GetPanelUI()->SetUIType(UI_TYPE::AMMO);
	CGameObject* pObj = nullptr;

	// HPLine
	pObj		= new CGameObject;
	m_pAmmoLine = new CImageUIScript;
	pObj->SetName(L"Ammo Line");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pAmmoLine);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	pObj->MeshRender()->GetMaterial(0)->SetShader(
		CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"AmmoProgressBarShader"));

	m_pAmmoLine->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine.png"));
	m_pAmmoLine->AllowBindTexPerFrame();
	m_pAmmoLine->SetUIType(UI_TYPE::AMMO);

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(12, 50.f, 1.f));

	GetOwner()->AddChild(pObj);
}
