#include "pch.h"
#include "CWeaponInfo.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CFontMgr.h>

#include "CImageUIScript.h"

CWeaponInfo::CWeaponInfo()
	: CScript((UINT)SCRIPT_TYPE::WEAPONINFO)
{
}

CWeaponInfo::~CWeaponInfo()
{
}

void CWeaponInfo::begin()
{
	CGameObject* pObj = new CGameObject;
	m_pWeaponUI		  = new CImageUIScript;

	pObj->SetName("WeaponInfo");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pWeaponUI);

	pObj->Transform()->SetRelativeScale(Vec3(958.f, 246.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pWeaponUI->AllowBindTexPerFrame();
	m_pWeaponUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Serika_Weapon.png"));

	GetOwner()->AddChild(pObj);
}

void CWeaponInfo::tick()
{
	CFontMgr::GetInst()->DrawFont(to_wstring(m_CurAmmo).c_str(), 500, 500, 30, FONT_RGBA(255, 255, 255, 255));
}

void CWeaponInfo::Fire()
{
	if (!m_CurAmmo)
		return;

	--m_CurAmmo;
}
