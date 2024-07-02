#include "pch.h"
#include "CMonsterHP.h"
#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

CMonsterHP::CMonsterHP()
	: CProgressBar((UINT)SCRIPT_TYPE::MONSTERHP)
{
}

CMonsterHP::~CMonsterHP()
{
}

void CMonsterHP::begin()
{
	CProgressBar::begin();
}

void CMonsterHP::tick()
{
}

void CMonsterHP::MakeChildObjects()
{
	// transform 조정
	Transform()->SetRelativeScale(Vec3(380.f, 55.f, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Bg.png"));

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_0, true);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.9f);

	CGameObject* pObj = nullptr;

	// HPLine
	pObj		= new CGameObject;
	m_pHPLineUI = new CImageUIScript;
	pObj->SetName(L"HPLine");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pHPLineUI);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHpoint));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DynamicUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(361, 35.f, 1.f));

	GetOwner()->AddChild(pObj);
}
