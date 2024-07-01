#include "pch.h"
#include "CPlayerHP.h"
#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

CPlayerHP::CPlayerHP()
	: CProgressBar((UINT)SCRIPT_TYPE::PLAYERHP)
{
}

CPlayerHP::~CPlayerHP()
{
}

void CPlayerHP::begin()
{
	CProgressBar::begin();
}

void CPlayerHP::tick()
{
}

void CPlayerHP::MakeChildObjects()
{
	// transform 조정
	Transform()->SetRelativeScale(Vec3(361.f, 55.f, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Bg_Middle.png"));

	CGameObject* pObj = nullptr;

	// HPLine
	pObj		= new CGameObject;
	m_pHPLineUI = new CImageUIScript;
	pObj->SetName(L"HPLine");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pHPLineUI);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(361, 35.f, 1.f));

	GetOwner()->AddChild(pObj);

	// Bg_L
	pObj		= new CGameObject;
	m_pHPLineUI = new CImageUIScript;
	pObj->SetName(L"Bg_L");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pHPLineUI);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Bg_Left.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(-187, 0, -200.f));
	pObj->Transform()->SetRelativeScale(Vec3(15, 55.f, 1.f));

	GetOwner()->AddChild(pObj);

	// Bg_R
	pObj		= new CGameObject;
	m_pHPLineUI = new CImageUIScript;
	pObj->SetName(L"Bg_R");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pHPLineUI);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Bg_Right.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(187, 0, -200.f));
	pObj->Transform()->SetRelativeScale(Vec3(15, 55.f, 1.f));

	GetOwner()->AddChild(pObj);
}
