#include "pch.h"
#include "CBossSmokeWaveScript.h"

#include <Engine\CGameObject.h>

CBossSmokeWaveScript::CBossSmokeWaveScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSMOKEWAVESCRIPT)
	, m_Parent(nullptr)
	, m_OriginScale{}
	, m_bSmokeWave(false)
{
}

CBossSmokeWaveScript::~CBossSmokeWaveScript()
{
}

void CBossSmokeWaveScript::begin()
{
	if (GetOwner()->GetParent())
		m_Parent = GetOwner()->GetParent();

	m_OriginScale = Transform()->GetRelativeScale();
	Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true, m_OriginScale,
					  0.2f);
}

void CBossSmokeWaveScript::tick()
{
	int idx = m_Parent->Animator3D()->GetCurFrameIdx();

	if (idx > 155 && !m_bSmokeWave)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(m_OriginScale.x * 0.01f, 0.f, m_OriginScale.z * 0.01f), 0.4f);

		m_bSmokeWave = true;
	}

	if (idx > 180)
		GamePlayStatic::DestroyGameObject(GetOwner());
}