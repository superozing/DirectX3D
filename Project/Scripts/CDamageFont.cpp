#include "pch.h"
#include "CDamageFont.h"
#include "CImageUIScript.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CDamageFont::CDamageFont()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEFONT)
{
	AppendScriptParam("Damage", SCRIPT_PARAM::INT, &m_Damage);
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
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetShader(
		CAssetMgr::GetInst()->Load<CGraphicsShader>(L"GraphicsShader/DamageFontShader.gs"));

	m_pImageUIScript->AllowBindTexPerFrame();
	m_pImageUIScript->DisableMouseInput();

	if (!m_FontImg.Get())
		m_pImageUIScript->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ImgFont/D_Font_Normal_crop.png"));
	else
		m_pImageUIScript->SetUIImg(m_FontImg);
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

void CDamageFont::SetFontImg(Ptr<CTexture> _FontImg)
{
	m_FontImg = _FontImg;

	// 만약 폰트 이미지가 플레이 중 설정되었을 경우
	if (m_pImageUIScript)
		m_pImageUIScript->SetUIImg(m_FontImg);
}

#define TagFontImg "[Font Img]"

void CDamageFont::SaveToFile(FILE* _File)
{
}

void CDamageFont::SaveToFile(ofstream& fout)
{
	fout << TagFontImg << endl;
	SaveAssetRef(m_FontImg, fout);
}

void CDamageFont::LoadFromFile(FILE* _File)
{
}

void CDamageFont::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagFontImg);
	LoadAssetRef(m_FontImg, fin);
}
