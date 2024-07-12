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
	int CurHP = GetCurHP();
	int MaxHP = GetMaxHP();

	if (MaxHP)
		m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, (float)CurHP / MaxHP);
}

void CPlayerHP::SaveToFile(FILE* _File)
{
}

void CPlayerHP::SaveToFile(ofstream& fout)
{
}

void CPlayerHP::LoadFromFile(FILE* _File)
{
}

void CPlayerHP::LoadFromFile(ifstream& fin)
{
}

void CPlayerHP::MakeChildObjects()
{
	// transform 조정
	Transform()->SetRelativeScale(Vec3(380.f, 55.f, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Bg.png"));

	GetPanelUI()->SetUIType(UI_TYPE::BOSSHP);

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

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_1, true);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(361, 35.f, 1.f));

	GetOwner()->AddChild(pObj);
}
