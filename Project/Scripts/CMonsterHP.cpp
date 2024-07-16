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
	int CurHP = GetCurHP();
	int MaxHP = GetMaxHP();

	if (MaxHP)
		m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, (float)CurHP / MaxHP);
}

void CMonsterHP::MakeChildObjects()
{
	// transform 조정
	Transform()->SetRelativeScale(Vec3(302.f, 22.f, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ColorTex/White.png"));

	GetPanelUI()->SetUIType(UI_TYPE::BOSSHP);

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_2, true);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 1, 1, 1));

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
	pObj->MeshRender()->GetMaterial(0)->SetShader(CAssetMgr::GetInst()->Load<CGraphicsShader>(L"GraphicsShader/DynamicHPShader.gs"));
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_0, false);
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_1, true);
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_2, true);
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_3, true);
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 0.2f, 0.2f, 1.f));
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, 10);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Div.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(300, 20.f, 1.f));

	GetOwner()->AddChild(pObj);
}

void CMonsterHP::SaveToFile(FILE* _File)
{
}

void CMonsterHP::SaveToFile(ofstream& fout)
{
}

void CMonsterHP::LoadFromFile(FILE* _File)
{
}

void CMonsterHP::LoadFromFile(ifstream& fin)
{
}
