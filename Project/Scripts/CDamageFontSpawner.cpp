#include "pch.h"
#include "CDamageFontSpawner.h"

#include <Engine/CMemoryPoolMgr.h>

#include "CMemoryPoolMgrScript.h"
#include "CDigitUI.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLogMgr.h>

#define DamageFontPath "prefab/ShootingSystem/DamageFont.pref"

CDamageFontSpawner::CDamageFontSpawner()
	: CScript((UINT)SCRIPT_TYPE::DAMAGEFONTSPAWNER)
{
}

CDamageFontSpawner::~CDamageFontSpawner()
{
}

void CDamageFontSpawner::begin()
{
	// 풀 매니저 스크립트 세팅
	auto pObj = CMemoryPoolMgr::GetInst()->GetEX();
	m_PoolMgr = pObj->GetScript<CMemoryPoolMgrScript>();
}

void CDamageFontSpawner::tick()
{

	for (list<tDamageFontNode>::iterator it = m_DamageFontList.begin(); it != m_DamageFontList.end();)
	{
		// 모든 List를 순회하면서 AccTime 누적
		it->ActiveTime -= DT;

		// pos를 위로 올리기 (알파 값 조절과 같이)
		auto pDF = it->DamageFont;
		auto pDigit = pDF->GetChild()[0];
		Vec3 pos = pDF->Transform()->GetWorldPos();
		pos.y += 200.f * DT;
		pDF->Transform()->SetRelativePos(pos);
		pDigit->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0,
															 Vec4(1.f, 1.f, 1.f, it->ActiveTime / it->MaxTime));
		pDF->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0,
															 Vec4(1.f, 0.2f, 0.2f, it->ActiveTime / it->MaxTime));

		if (it->ActiveTime < 0.f)
		{
			m_PoolMgr->PushObject(pDF);
			it = m_DamageFontList.erase(it);
		}
		else
			++it;
	}
}

void CDamageFontSpawner::SpawnDamageFont(const Vec3& _HitPos, int Damage, float _ActiveTime)
{
	auto pObj = m_PoolMgr->PopObject(DamageFontPath);

	pObj->MeshRender()->GetDynamicMaterial(0);
	auto PanalMtrl = pObj->MeshRender()->GetMaterial(0);
	pObj->Transform()->SetRelativePos(_HitPos);
	PanalMtrl->SetScalarParam(SCALAR_PARAM::BOOL_2, true);
	PanalMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 0.2f, 0.2f, 1.f));
	PanalMtrl->SetTexParam(TEX_PARAM::TEX_0, 
		CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Damage_Bg.png"));
	
	// 자식 오브젝트 가져오기
	auto pDigit	   = pObj->GetChild()[0];

	pDigit->MeshRender()->GetDynamicMaterial(0);
	pDigit->MeshRender()->GetDynamicMaterial(0);
	auto DigitMtrl = pDigit->MeshRender()->GetMaterial(0);
	DigitMtrl->SetScalarParam(SCALAR_PARAM::BOOL_2, true);
	DigitMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	auto pUIScript = pDigit->GetScript<CDigitUI>();
	pUIScript->SetValue(Damage);

	// 텍스쳐 세팅 해주기.
	pUIScript->SetUIImg(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ImgFont/D_Font_Normal_crop.png"));
		
	// 레벨에 폰트 오브젝트 스폰
	GamePlayStatic::SpawnGameObject(pObj, (UINT)LAYER::LAYER_DEFAULT);


	// 관리를 위해 리스트에 추가
	tDamageFontNode tNode{};
	tNode.DamageFont = pObj;
	tNode.ActiveTime = _ActiveTime;
	tNode.MaxTime = _ActiveTime;

	m_DamageFontList.push_back(tNode);

	// 알파 값은 구조체에 있는 Max와 ActiveTime 나눠서 구하면 되겠네요.
	// Pos는 매 틱 마다 계속 위로 올려주면 될 듯.

}

void CDamageFontSpawner::SaveToFile(FILE* _File)
{
}

void CDamageFontSpawner::SaveToFile(ofstream& fout)
{
}

void CDamageFontSpawner::LoadFromFile(FILE* _File)
{
}

void CDamageFontSpawner::LoadFromFile(ifstream& fin)
{
}
