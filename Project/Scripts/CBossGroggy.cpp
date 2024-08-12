#include "pch.h"
#include "CBossGroggy.h"
#include "CBossScript.h"
#include "CImageUIScript.h"

CBossGroggy::CBossGroggy()
	: CScript((UINT)SCRIPT_TYPE::BOSSGROGGY)
{
}

CBossGroggy::~CBossGroggy()
{
}

void CBossGroggy::begin()
{
	// Child Object Vector를 가져와 멤버로 세팅하기
	auto& vecChild = GetOwner()->GetChild();

	for (size_t i = 0; i < vecChild.size(); i++)
	{
		auto gauge = vecChild[i]->GetScript<CImageUIScript>();

		if (gauge)
		{
			m_pGroggyGaugeUI = gauge;
			m_pGroggyGaugeUI->MeshRender()->GetDynamicMaterial(0);
			m_pGroggyGaugeUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_1, true);
			continue;
		}
	}

}

void CBossGroggy::tick()
{
	float groggyGauge = m_pBossScript->GetGroggy();

	if (groggyGauge != 0.f)
		groggyGauge /= 100.f;

	m_pGroggyGaugeUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, groggyGauge);
}
