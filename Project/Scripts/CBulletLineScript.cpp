#include "pch.h"
#include "CBulletLineScript.h"
#include <Engine/CRenderMgr.h>

CBulletLineScript::CBulletLineScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETLINESCRIPT)
	, m_Parent(nullptr)
	, m_vDir{}
	, m_Module{}

{
}

CBulletLineScript::~CBulletLineScript()
{
}

void CBulletLineScript::begin()
{
	// 파티클 시스템 모듈 가져오기
	m_Module = ParticleSystem()->GetParticleModule();

	ParticleSystem()->SetModule(m_Module);

	// 플레이어 세팅
	if (GetOwner()->GetParent())
		m_Parent = GetOwner()->GetParent();

	// 임시로 play 처리
	ParticleSystem()->Play();

	Transform()->SetRelativePos(Vec3(19.f, 24.f, -25.f));
}

void CBulletLineScript::tick()
{
	Matrix BoneFrameMat = m_Parent->Animator3D()->FindBoneMat(L"Bip001_Weapon");
	Transform()->SetFrameMat(BoneFrameMat);

	m_vDir = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	m_Module.FixedDirection = m_vDir;
	ParticleSystem()->SetModule(m_Module);
}

