#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CMRT.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"

#include "CCameraShake.h"
#include "CLight3D.h"
#include "CAnimator3D.h"
#include "CInstancingBuffer.h"

#include "CLogMgr.h"
#include "CUIMgr.h"

#include "CUIScript.h"
ConvertCoord CCamera::ViewportConvertFunc = nullptr;

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_FOV(1)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
	, m_CameraPriority(-1)
{
	Vec2 vResol	  = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = vResol.x / vResol.y;
	m_Width		  = vResol.x;
	SetShake();

	m_Frustum.SetOwner(this);
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Frustum(_Other.m_Frustum)
	, m_ProjType(_Other.m_ProjType)
	, m_FOV(_Other.m_FOV)
	, m_Width(_Other.m_Width)
	, m_Scale(_Other.m_Scale)
	, m_AspectRatio(_Other.m_AspectRatio)
	, m_Far(_Other.m_Far)
	, m_LayerCheck(_Other.m_LayerCheck)
	, m_CameraPriority(-1)
{
	m_Frustum.SetOwner(this);
}

CCamera::~CCamera()
{
}

struct CmpAscending
{
	bool operator()(CGameObject* _First, CGameObject* _Second)
	{
		return _First->Transform()->GetWorldViewPos().z < _Second->Transform()->GetWorldViewPos().z;
	}
};

struct CmpDescending
{
	bool operator()(CGameObject* _First, CGameObject* _Second)
	{
		return _First->Transform()->GetWorldViewPos().z > _Second->Transform()->GetWorldViewPos().z;
	}
};

struct CmpUIDescending
{
	bool operator()(CGameObject* _First, CGameObject* _Second)
	{
		return _First->Transform()->GetWorldPos().z > _Second->Transform()->GetWorldPos().z;
	}
};

struct CmpDecalPriority
{
	bool operator()(CGameObject* _First, CGameObject* _Second)
	{
		return _First->Decal()->GetDecalPriority() < _Second->Decal()->GetDecalPriority();
	}
};

void CCamera::SetShake()
{
	m_pShake = make_shared<CCameraShake>(GetOwner());
}

void CCamera::SetShake(float _duration, Vec3 _posScale, Vec3 _rotScale, float _frequency, float _releaseTime)
{
	m_pShake = make_shared<CCameraShake>(GetOwner(), _duration, _posScale, _rotScale, _frequency, _releaseTime);
}

void CCamera::SetShake(shared_ptr<class CCameraShake> _shake)
{
	if (!_shake)
		return;

	m_pShake = _shake;
	m_pShake->RegistCamera(GetOwner());
}

void CCamera::Shake()
{
	m_pShake->Shake();
}

void CCamera::begin()
{
	// 카메라를 우선순위값에 맞게 RenderMgr 에 등록시킴
	CRenderMgr::GetInst()->RegisterCamera(this, m_CameraPriority);
	SetShake();
}

void CCamera::finaltick()
{
	if (m_pShake.get())
		m_pShake->finaltick();

	// 뷰 행렬을 계산한다.
	// 카메라를 원점으로 이동시키는 이동 행렬
	Vec3   vCamPos	= Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix matRotate = XMMatrixIdentity();
	matRotate._11	 = vRight.x;
	matRotate._12	 = vUp.x;
	matRotate._13	 = vFront.x;
	matRotate._21	 = vRight.y;
	matRotate._22	 = vUp.y;
	matRotate._23	 = vFront.y;
	matRotate._31	 = vRight.z;
	matRotate._32	 = vUp.z;
	matRotate._33	 = vFront.z;

	// 이동 x 회전 = view 행렬
	m_matView	 = matTrans * matRotate;
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);

	// 투영 방식에 따른 투영 행렬을 계산한다.
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교투영
		m_matProj = XMMatrixOrthographicLH(m_Width * m_Scale, (m_Width / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	else
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	// Frustum 계산
	m_Frustum.finaltick();

	// 마우스방향 직선 계산
	CalculateRay();
}

void CCamera::SetCameraPriority(int _Priority)
{
	m_CameraPriority = _Priority;
}

void CCamera::LayerCheck(UINT _LayerIdx, bool _bCheck)
{
	if (_bCheck)
	{
		m_LayerCheck |= (1 << _LayerIdx);
	}
	else
	{
		m_LayerCheck &= ~(1 << _LayerIdx);
	}
}

void CCamera::LayerCheck(const wstring& _strLayerName, bool _bCheck)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer	  = pCurLevel->GetLayer(_strLayerName);

	if (nullptr == pLayer)
		return;

	int idx = pLayer->GetLayerIdx();
	LayerCheck(idx, _bCheck);
}

void CCamera::SortObject()
{
	// 이전 프레임 분류정보 제거
	m_mapInstGroup_D.clear();
	m_mapInstGroup_F.clear();

	// 현재 레벨 가져와서 분류
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		// 카메라가 찍도록 설정된 Layer 가 아니면 무시
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		CLayer*						pLayer	   = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

			// 렌더링 기능이 없는 오브젝트는 제외
			if (nullptr == pRenderCom)
				continue;

			// Font일 경우 강제로 Transparent에 넣어주기
			if (pRenderCom->GetType() == COMPONENT_TYPE::FONTRENDER)
			{
				m_vecTransparent.push_back(vecObjects[j]);
				continue;
			}

			// UI일 경우 UI가 활성화 상태인지 체크
			auto pUI = vecObjects[j]->GetScript<CUIScript>();
			if (pUI)
			{
				if (!CUIMgr::GetInst()->IsActiveUIType(pUI->GetUIType()))
					continue;
			}

			// 렌더링 기능이 없는 오브젝트는 제외
			if (nullptr == pRenderCom->GetMesh())
				continue;

			// 절두체 안에 존재하지 않을 경우 continue
			/*if (!m_Frustum.FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()))
				continue;*/

			// 메테리얼 개수만큼 반복
			UINT iMtrlCount = pRenderCom->GetMtrlCount();
			for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
			{
				// 재질이 없거나, 재질의 쉐이더가 설정이 안된 경우
				if (nullptr == pRenderCom->GetMaterial(iMtrl) || nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
				{
					continue;
				}

				// 쉐이더 도메인에 따른 분류
				SHADER_DOMAIN		 eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
				Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
				case SHADER_DOMAIN::DOMAIN_MASKED: {
					// Shader 의 DOMAIN 에 따라서 인스턴싱 그룹을 분류한다.
					map<ULONG64, vector<tInstObj>>* pMap  = NULL;
					Ptr<CMaterial>					pMtrl = pRenderCom->GetMaterial(iMtrl);

					if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
					{
						pMap = &m_mapInstGroup_D;
					}
					else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE ||
							 pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASKED)
					{
						pMap = &m_mapInstGroup_F;
					}
					else
					{
						assert(nullptr);
						continue;
					}

					uInstID uID = {};
					uID.llID	= pRenderCom->GetInstID(iMtrl);

					// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
					if (0 == uID.llID)
						continue;

					map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
					if (iter == pMap->end())
					{
						pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{vecObjects[j], iMtrl}}));
					}
					else
					{
						iter->second.push_back(tInstObj{vecObjects[j], iMtrl});
					}
				}
				break;
				case SHADER_DOMAIN::DOMAIN_DECAL: {
					Vec3 CamPos	  = this->Transform()->GetWorldPos();
					Vec3 DecalPos = vecObjects[j]->Transform()->GetWorldPos();

					float fDist = Vec3::Distance(CamPos, DecalPos);

					if (fDist < vecObjects[j]->Decal()->GetRenderDistance())
						m_vecDecal.push_back(vecObjects[j]);
				}
				break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPostProcess.push_back(vecObjects[j]);
					break;
				}
			}
		}
	}

	// Depth Sorting
	if (this == CRenderMgr::GetInst()->GetMainCam())
	{
		std::sort(m_vecTransparent.begin(), m_vecTransparent.end(), CmpDescending());
	}
	else
	{
		std::sort(m_vecTransparent.begin(), m_vecTransparent.end(), CmpUIDescending());
	}
}

void CCamera::render_deferred()
{
	render_Instance(m_mapInstGroup_D);
}

void CCamera::render_Instance(const map<ULONG64, vector<tInstObj>>& m_mapInstGroup)
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 2 || pair.second[0].pObj->Animator2D() ||
			pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() ==
				nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter = m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject*   pObj	 = pair.second[0].pObj;
		Ptr<CMesh>	   pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int	 iRowIdx	= 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV	   = tInstData.matWorld * m_matView;
			tInstData.matWVP   = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(
					pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();
		// auto BloomInfo = CRenderMgr::GetInst()->m_GlobalBloomInfo;
		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_decal()
{
	std::sort(m_vecDecal.begin(), m_vecDecal.end(), CmpDecalPriority());

	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
	}
	m_vecDecal.clear();
}

void CCamera::render_forward()
{
	render_Instance(m_mapInstGroup_F);

	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
	m_vecTransparent.clear();
}

void CCamera::render_postprocess()
{
	// Blur&Bloom처리
	if (true == CRenderMgr::GetInst()->m_GlobalBloomInfo.BloomActivate)
	{
		Blur();
		Bloom();
	}

	Cromatic_Aberration();

	// for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	//{
	//	// 최종 렌더링 이미지를 후처리 타겟에 복사
	//	CRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();

	//	// 복사받은 후처리 텍스쳐를 t13 레지스터에 바인딩
	//	Ptr<CTexture> pPostProcessTex = CRenderMgr::GetInst()->GetPostProcessTex();
	//	pPostProcessTex->UpdateData(13);

	//	// 후처리 오브젝트 렌더링
	//	m_vecPostProcess[i]->render();
	//}

	// m_vecPostProcess.clear();
}

void CCamera::Merge()
{
	// Deferred 정보를 SwapChain 으로 병합
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	static Ptr<CMesh>	  pRectMesh	 = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect);
	static Ptr<CMaterial> pMergeMtrl = CAssetMgr::GetInst()->Load<CMaterial>(MTRLOutLineMerge);

	pMergeMtrl->UpdateData();
	pRectMesh->render(0);
}

#include "CBlurX.h"
#include "CBlurY.h"
#include "CDownScale.h"
#include "CUpScale.h"

void CCamera::Blur()
{
	////////////////
	// CS쉐이더 구현
	////////////////
	auto BlurTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RelativeLuminanceTargetTex");

	// Bloom Level <= MAXBLURLEVEL
	int BlurLevel = min(MAXBLURLEVEL, CRenderMgr::GetInst()->m_GlobalBloomInfo.BlurLevel);

	// Blur Tex
	auto BloomOne = CRenderMgr::GetInst()->m_vecBlurOneTex;
	auto BloomTwo = CRenderMgr::GetInst()->m_vecBlurTwoTex;

	// Compute Shader
	Ptr<CBlurX>		BlurXShader = (CBlurX*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BlurXCS").Get();
	Ptr<CBlurY>		BlurYShader = (CBlurY*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BlurYCS").Get();
	Ptr<CDownScale> DownScaleShader =
		(CDownScale*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"DownScaleCS").Get();
	Ptr<CUpScale> UpScaleShader = (CUpScale*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"UpScaleCS").Get();

	// DownScale
	for (int i = 0; i < BlurLevel; ++i)
	{
		if (0 == i)
		{
			DownScaleShader->SetResourceTex(BlurTarget);
		}
		else
		{
			DownScaleShader->SetResourceTex(BloomOne[i - 1]);
		}
		DownScaleShader->SetTargetTexture(BloomOne[i]);
		DownScaleShader->Execute();
	}

	for (int i = BlurLevel - 1; i >= 0; --i)
	{
		// Blur X
		BlurXShader->SetResourceTex(BloomOne[i]);
		BlurXShader->SetTargetTexture(BloomTwo[i]);
		BlurXShader->Execute();

		// Blur Y
		BlurYShader->SetResourceTex(BloomTwo[i]);
		BlurYShader->SetTargetTexture(BloomOne[i]);
		BlurYShader->Execute();

		// Up Scaling
		UpScaleShader->SetResourceTex(BloomOne[i]);

		if (i == 0)
		{
			UpScaleShader->SetTargetTexture(BlurTarget);
		}
		else
		{
			UpScaleShader->SetTargetTexture(BloomOne[i - 1]);
		}
		UpScaleShader->Execute();
	}
}

void CCamera::Bloom()
{
	// 리소스,타겟 얻어오기
	auto BlurSource	  = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RelativeLuminanceTargetTex");
	auto ColorCopy	  = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
	auto RenderTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");

	// 매쉬,머터리얼 받아오기
	static Ptr<CMesh>	  pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect);
	static Ptr<CMaterial> pBloomMtrl;
	pBloomMtrl = CAssetMgr::GetInst()->Load<CMaterial>(MTRLBloom);

	// 블랜드비율
	auto  BlurInfo	 = CRenderMgr::GetInst()->m_GlobalBloomInfo;
	float Bloomratio = BlurInfo.Ratio;

	// 텍스쳐, 블랜드비율 바인딩
	pBloomMtrl->SetTexParam(TEX_PARAM::TEX_0, ColorCopy);
	pBloomMtrl->SetTexParam(TEX_PARAM::TEX_1, BlurSource);
	pBloomMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, Bloomratio);
	pBloomMtrl->UpdateData();

	// 타겟->리소스 복사
	CRenderMgr::GetInst()->CopyFromTextureToTexture(ColorCopy, RenderTarget);
	pRectMesh->render(0);
}

void CCamera::Cromatic_Aberration()
{
	auto& RefInfo = CRenderMgr::GetInst()->m_CAInfo;
	if (false == RefInfo.Activate)
		return;

	RefInfo.RemainTime -= DT;
	if (0.f >= RefInfo.RemainTime)
	{
		RefInfo.Activate   = false;
		RefInfo.RemainTime = 0.f;
	}
	float LifeRatio = RefInfo.RemainTime / RefInfo.Duration;

	auto ColorCopy	  = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
	auto RenderTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");

	// 매쉬,머터리얼 받아오기
	static Ptr<CMesh>	  pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect);
	static Ptr<CMaterial> pCAMat;
	pCAMat = CAssetMgr::GetInst()->Load<CMaterial>(MTRLCromatic_Aberration);

	// 텍스쳐, 오프셋바인딩
	pCAMat->SetTexParam(TEX_PARAM::TEX_0, ColorCopy);
	pCAMat->SetScalarParam(SCALAR_PARAM::VEC2_0, RefInfo.MaxRedOffSet * LifeRatio);
	pCAMat->SetScalarParam(SCALAR_PARAM::VEC2_1, RefInfo.MaxGreenOffset * LifeRatio);
	pCAMat->SetScalarParam(SCALAR_PARAM::VEC2_2, RefInfo.MaxBlueOffset * LifeRatio);
	pCAMat->SetScalarParam(SCALAR_PARAM::VEC2_3, RoRMath::Lerp(Vec2(1.f, 1.f), RefInfo.CropOffset, LifeRatio));
	pCAMat->UpdateData();

	// 타겟->리소스 복사
	CRenderMgr::GetInst()->CopyFromTextureToTexture(ColorCopy, RenderTarget);
	pRectMesh->render(0);
}

void CCamera::SortShadowMapObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		// 카메라가 찍도록 설정된 Layer 가 아니면 무시
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		CLayer*						pLayer	   = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 메쉬, 재질, 쉐이더 확인
			if (!(vecObjects[j]->Transform() && vecObjects[j]->Transform()->IsDynamic() &&
				  vecObjects[j]->GetRenderComponent() && vecObjects[j]->GetRenderComponent()->IsDrawShadow() &&
				  vecObjects[j]->GetRenderComponent()->GetMesh().Get() &&
				  vecObjects[j]->GetRenderComponent()->GetMaterial(0).Get() &&
				  vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader().Get()))
			{
				continue;
			}

			m_vecShadow.push_back(vecObjects[j]);
		}
	}
}

void CCamera::render_shadowmap()
{
	Ptr<CMaterial> pShadowMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShadowMapMtrl");

	g_Transform.matView	   = m_matView;
	g_Transform.matViewInv = m_matViewInv;
	g_Transform.matProj	   = m_matProj;
	g_Transform.matProjInv = m_matProjInv;

	for (size_t i = 0; i < m_vecShadow.size(); ++i)
	{
		// Animator 업데이트
		if (m_vecShadow[i]->Animator3D())
		{
			m_vecShadow[i]->Animator3D()->UpdateData();

			for (UINT j = 0; j < m_vecShadow[i]->GetRenderComponent()->GetMtrlCount(); ++j)
			{
				if (nullptr == m_vecShadow[i]->GetRenderComponent()->GetMaterial(j))
					continue;

				pShadowMapMtrl->SetAnim3D(true); // Animation Mesh 알리기
				pShadowMapMtrl->SetBoneCount(m_vecShadow[i]->Animator3D()->GetBoneCount());
			}
		}

		m_vecShadow[i]->Transform()->UpdateData();
		pShadowMapMtrl->UpdateData();

		for (UINT k = 0; k < m_vecShadow[i]->GetRenderComponent()->GetMtrlCount(); ++k)
		{
			m_vecShadow[i]->GetRenderComponent()->GetMesh()->render(k);
		}

		if (m_vecShadow[i]->Animator3D())
		{
			UINT		   iMtrlCount = m_vecShadow[i]->GetRenderComponent()->GetMtrlCount();
			Ptr<CMaterial> pMtrl	  = nullptr;
			for (UINT i = 0; i < iMtrlCount; ++i)
			{
				pMtrl = pShadowMapMtrl;
				if (nullptr == pMtrl)
					continue;

				pMtrl->SetAnim3D(false); // Animation Mesh 알리기
				pMtrl->SetBoneCount(0);
			}
		}
	}

	m_vecShadow.clear();
}

#include "CKeyMgr.h"
void CCamera::CalculateRay()
{
	// 마우스 방향을 향하는 Ray 구하기
	// SwapChain 타겟의 ViewPort 정보
	CMRT*		   pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP	= pMRT->GetViewPort();

	//  현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	if (ViewportConvertFunc != nullptr)
		vMousePos = ViewportConvertFunc();

	// 직선은 카메라의 좌표를 반드시 지난다.
	m_ray.vStart = Transform()->GetWorldPos();

	// view space 에서의 방향
	m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	m_ray.vDir.z = 1.f;

	// world space 에서의 방향
	m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
	m_ray.vDir.Normalize();
}

void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_CameraPriority, sizeof(int), 1, _File);
}

#define TagProjType "[ProjType]"
#define TagFOV "[FOV]"
#define TagWidth "[Width]"
#define TagScale "[Scale]"
#define TagAspectRatio "[AspectRatio]"
#define TagFar "[Far]"
#define TagLayerCheck "[LayerCheck]"
#define TagPriority "[Priority]"
#define TagShake "[Shake]"

void CCamera::SaveToFile(ofstream& fout)
{
	fout << TagProjType << endl;
	auto str = magic_enum::enum_name((PROJ_TYPE)m_ProjType);
	fout << ToString(str) << endl;

	fout << TagFOV << endl;
	fout << m_FOV << endl;

	fout << TagWidth << endl;
	fout << m_Width << endl;

	fout << TagScale << endl;
	fout << m_Scale << endl;

	fout << TagAspectRatio << endl;
	fout << m_AspectRatio << endl;

	fout << TagFar << endl;
	fout << m_Far << endl;

	fout << TagLayerCheck << endl;
	fout << m_LayerCheck << endl;

	fout << TagPriority << endl;
	fout << m_CameraPriority << endl;

	fout << TagShake << endl;
	if (m_pShake.get())
	{
		fout << 1 << endl;
		fout << *m_pShake.get() << endl;
	}
	else
	{
		fout << (int)0 << endl;
	}
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_CameraPriority, sizeof(int), 1, _File);
}

void CCamera::LoadFromFile(ifstream& fin)
{
	string str;
	Utils::GetLineUntilString(fin, TagProjType);
	getline(fin, str);
	m_ProjType = magic_enum::enum_cast<PROJ_TYPE>(str).value();

	Utils::GetLineUntilString(fin, TagFOV);
	fin >> m_FOV;

	Utils::GetLineUntilString(fin, TagWidth);
	fin >> m_Width;

	Utils::GetLineUntilString(fin, TagScale);
	fin >> m_Scale;

	Utils::GetLineUntilString(fin, TagAspectRatio);
	fin >> m_AspectRatio;

	Utils::GetLineUntilString(fin, TagFar);
	fin >> m_Far;

	Utils::GetLineUntilString(fin, TagLayerCheck);
	fin >> m_LayerCheck;

	Utils::GetLineUntilString(fin, TagPriority);
	fin >> m_CameraPriority;

	int exist;
	Utils::GetLineUntilString(fin, TagShake);
	fin >> exist;
	if (exist)
	{
		fin >> *m_pShake.get();
		m_pShake->RegistCamera(GetOwner());
	}
}
