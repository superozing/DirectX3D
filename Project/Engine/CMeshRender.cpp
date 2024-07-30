#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CTransform.h"
#include "CMaterial.h"

#include "CRenderMgr.h"
#include "CScript.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
}

void CMeshRender::finaltick()
{
	// if (CRenderMgr::GetInst()->IsDebugPosition())
	//{
	//	// Depth Test 추가
	//	GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 100.f, Vec3(0.f, 1.f, 0.f), false);
	// }
}

void CMeshRender::render()
{
	if (nullptr == GetMesh())
		return;

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	else
	{
		Animator2D()->Clear();
	}

	// Animator3D 업데이트
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (UINT i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	Transform()->UpdateData();

	// 메쉬의 모든 부위를 렌더링한다.
	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		if (nullptr == GetMaterial(i))
			continue;

		GetMaterial(i)->UpdateData();
		GetMesh()->render(i);
	}

	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::render(UINT _Subset)
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(_Subset))
		return;

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D 업데이트
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		GetMaterial(_Subset)->SetAnim3D(true); // Animation Mesh 알리기
		GetMaterial(_Subset)->SetBoneCount(Animator3D()->GetBoneCount());
	}

	vector<CScript*> vecScript = GetOwner()->GetScripts();

	for (int i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->UpdateData();
	}

	// 사용할 재질 업데이트
	GetMaterial(_Subset)->UpdateData();

	// 사용할 메쉬 업데이트 및 렌더링
	GetMesh()->render(_Subset);

	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();

	for (int i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->Clear();
	}
}
