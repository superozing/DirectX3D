#include "pch.h"
#include "CWeaponInfo.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

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

	// Weapon ui
	auto pObj = new CGameObject;
	m_pWeaponUI		  = new CImageUIScript;

	pObj->SetName("Weapon UI");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pWeaponUI);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	pObj->Transform()->SetRelativeScale(Vec3(479.f, 123.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pWeaponUI->AllowBindTexPerFrame();
	m_pWeaponUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Serika_Weapon.png"));

	GetOwner()->AddChild(pObj);

	// Font Info
	m_AmmoFontInfo.fFontSize = 50.f;
	m_AmmoFontInfo.FontType	 = FONT_TYPE::MAPLE;
	m_AmmoFontInfo.TextFlag	 = FW1_RIGHT;

	m_AmmoFontOffset.x = 200.f;
	m_AmmoFontOffset.y = 50.f;
}

void CWeaponInfo::tick()
{
	// 폰트의 색상 설정
	if (m_CurAmmo > 0)
		m_AmmoFontInfo.Color = FONT_RGBA(255, 255, 255, 255);
	else
		m_AmmoFontInfo.Color = FONT_RGBA(255, 60, 60, 255);

	// 폰트의 위치 설정

	Vec3 vWorldPos = Transform()->GetWorldPos();
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	m_AmmoFontInfo.vPos	 = Vec2(vWorldPos.x + (vResol.x / 2) + m_AmmoFontOffset.x,
								-vWorldPos.y + (vResol.y / 2) + m_AmmoFontOffset.y);
	
	m_AmmoFontInfo.WStr = to_wstring(m_CurAmmo) + L" /" + to_wstring(m_MaxAmmo);

	CFontMgr::GetInst()->RegisterFont(m_AmmoFontInfo);
}

void CWeaponInfo::Fire()
{
	if (!m_CurAmmo)
		return;

	--m_CurAmmo;
}
