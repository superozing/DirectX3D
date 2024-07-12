#include "pch.h"
#include "CSkillIcon.h"
#include <Engine/CLogMgr.h>
#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

CSkillIcon::CSkillIcon()
	: CScript((UINT)SCRIPT_TYPE::SKILLICON)
{
}

CSkillIcon::~CSkillIcon()
{
}

void CSkillIcon::begin()
{
	AppendScriptParam("Cur Cooldown", SCRIPT_PARAM::FLOAT, &m_CurSkillCooldown);
	AppendScriptParam("Skill Cooldown", SCRIPT_PARAM::FLOAT, &m_SkillCooldown);

	MakeChildObjects();
}

void CSkillIcon::tick()
{
	// 쿨다운 돌리기
	m_CurSkillCooldown = RoRMath::ClampFloat(m_CurSkillCooldown -= DT, 0.f);

	// 스킬 사용 가능 여부 체크
	m_SkillUsable = m_CurSkillCooldown == 0.f ? true : false;

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_CurSkillCooldown / m_SkillCooldown);
}

void CSkillIcon::SaveToFile(FILE* _File)
{
}

void CSkillIcon::SaveToFile(ofstream& fout)
{
}

void CSkillIcon::LoadFromFile(FILE* _File)
{
}

void CSkillIcon::LoadFromFile(ifstream& fin)
{
}

void CSkillIcon::SetSkillIconTex(Ptr<CTexture> _SkillIconTex)
{
	m_pSkillIconTex = _SkillIconTex;

	if (m_pSkillIconUI)
	{
		m_pSkillIconUI->SetUIImg(_SkillIconTex);
	}
}

void CSkillIcon::UseSkill()
{
	if (m_SkillUsable)
		m_CurSkillCooldown = m_SkillCooldown;
}

void CSkillIcon::MakeChildObjects()
{
	// m_pSkillIconUI
	m_pSkillIconUI = new CImageUIScript;

	GetOwner()->AddComponent(new CTransform);
	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->AddComponent(m_pSkillIconUI);

	m_pSkillIconUI->SetUIType(UI_TYPE::SKILLICON);

	Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	Transform()->SetRelativeScale(Vec3(152.f, 152.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);
	MeshRender()->GetMaterial(0)->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"SkillIconShader"));

	m_pSkillIconUI->SetUIImg(m_pSkillIconTex);
	m_pSkillIconUI->AllowBindTexPerFrame();
}
