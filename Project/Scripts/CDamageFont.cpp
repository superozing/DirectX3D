#include "pch.h"
#include "CDamageFont.h"
#include "CImageUIScript.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CDamageFont::CDamageFont()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEFONT)
{
}

CDamageFont::~CDamageFont()
{
}

void CDamageFont::begin()
{
	auto pObj		 = GetOwner();
	m_pImageUIScript = new CImageUIScript;

	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pImageUIScript);

	m_pImageUIScript->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	m_pImageUIScript->SetUIType(UI_TYPE::DAMAGEFONT);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHpoint));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DynamicUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetShader(
		CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"DamageFontShader"));

	m_pImageUIScript->AllowBindTexPerFrame();
	m_pImageUIScript->DisableMouseInput();
	m_pImageUIScript->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ImgFont/Damage_Font_Number3.png"));
}

void CDamageFont::tick()
{
	// 1. 자릿수 계산
	int digit = to_string(m_Damage).size();

	// 2. 자릿수에 따라서 x scale 늘리기
	Transform()->SetRelativeScale(Vec3(32.f * digit, 32.f, 1.f));

	// 3. 자릿수를 텍스쳐 레지스터에 바인딩
	GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, digit);

	// 4. 데미지를 텍스쳐 레지스터에 바인딩
	GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, m_Damage);
}
