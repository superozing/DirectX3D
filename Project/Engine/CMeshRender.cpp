#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CTransform.h"
#include "CMaterial.h"

#include "CRenderMgr.h"


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
	//if (CRenderMgr::GetInst()->IsDebugPosition())
	//{
	//	// Depth Test 추가
	//	GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 100.f, Vec3(0.f, 1.f, 0.f), false);
	//}
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
}