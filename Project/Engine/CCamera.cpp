#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include  "CMRT.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"

#include "CCameraShake.h"
#include "CLight3D.h"

#include "CLogMgr.h"

ConvertCoord CCamera::ViewportConvertFunc = nullptr;

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_FOV(XM_PI / 2.f)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
	, m_CameraPriority(-1)
{
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = vResol.x / vResol.y;
	m_Width = vResol.x;
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
	bool operator() (CGameObject* _First, CGameObject* _Second)
	{
		return _First->Transform()->GetWorldViewPos().z < _Second->Transform()->GetWorldViewPos().z;
	}
};

struct CmpDescending
{
	bool operator() (CGameObject* _First, CGameObject* _Second)
	{
		return _First->Transform()->GetWorldViewPos().z > _Second->Transform()->GetWorldViewPos().z;
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
	if (!_shake) return;

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
	if(m_pShake.get())
		m_pShake->finaltick();


	// 뷰 행렬을 계산한다.
	// 카메라를 원점으로 이동시키는 이동 행렬
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix matRotate = XMMatrixIdentity();
	matRotate._11 = vRight.x; matRotate._12 = vUp.x; matRotate._13 = vFront.x;
	matRotate._21 = vRight.y; matRotate._22 = vUp.y; matRotate._23 = vFront.y;
	matRotate._31 = vRight.z; matRotate._32 = vUp.z; matRotate._33 = vFront.z;

	// 이동 x 회전 = view 행렬
	m_matView = matTrans * matRotate;
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
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);

	if (nullptr == pLayer)
		return;

	int idx = pLayer->GetLayerIdx();
	LayerCheck(idx, _bCheck);
}

void CCamera::SortObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		// 카메라가 찍도록 설정된 Layer 가 아니면 무시
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 메쉬, 재질, 쉐이더 확인
			if (!( vecObjects[j]->GetRenderComponent()
				&& vecObjects[j]->GetRenderComponent()->GetMesh().Get()
				&& vecObjects[j]->GetRenderComponent()->GetMaterial(0).Get()
				&& vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader().Get()))
			{
				continue;
			}

			// 절두체 안에 존재하지 않을 경우 continue
			/*if (!m_Frustum.FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()))
				continue;*/

			SHADER_DOMAIN domain = vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader()->GetDomain();

			switch (domain)
			{
			case SHADER_DOMAIN::DOMAIN_DEFERRED:
				m_vecDeferred.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_DECAL:
				m_vecDecal.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_DEBUG:
				break;
			}
		}
	}

	// Depth Sort
	std::sort(m_vecOpaque.begin(), m_vecOpaque.end(), CmpAscending());
	std::sort(m_vecMasked.begin(), m_vecMasked.end(), CmpAscending());
	std::sort(m_vecTransparent.begin(), m_vecTransparent.end(), CmpDescending());

}

void CCamera::render_deferred()
{
	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		m_vecDeferred[i]->render();
	}
	m_vecDeferred.clear();
}

void CCamera::render_decal()
{
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
	}
	m_vecDecal.clear();
}

void CCamera::render_forward()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
	m_vecOpaque.clear();

	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->render();
	}
	m_vecMasked.clear();

	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
	m_vecTransparent.clear();
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		// 최종 렌더링 이미지를 후처리 타겟에 복사
		CRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();

		// 복사받은 후처리 텍스쳐를 t13 레지스터에 바인딩
		Ptr<CTexture> pPostProcessTex = CRenderMgr::GetInst()->GetPostProcessTex();
		pPostProcessTex->UpdateData(13);

		// 후처리 오브젝트 렌더링
		m_vecPostProcess[i]->render();
	}

	m_vecPostProcess.clear();
}

void CCamera::Merge()
{
	// Deferred 정보를 SwapChain 으로 병합
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	static Ptr<CMesh>	  pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect);
	static Ptr<CMaterial> pMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"MergeMtrl");

	pMergeMtrl->UpdateData();
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

		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 메쉬, 재질, 쉐이더 확인
			if (!(vecObjects[j]->Transform()
				&& vecObjects[j]->Transform()->IsDynamic()
				&& vecObjects[j]->GetRenderComponent()
				&& vecObjects[j]->GetRenderComponent()->IsDrawShadow()
				&& vecObjects[j]->GetRenderComponent()->GetMesh().Get()
				&& vecObjects[j]->GetRenderComponent()->GetMaterial(0).Get()
				&& vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader().Get()))
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

	g_Transform.matView = m_matView;
	g_Transform.matViewInv = m_matViewInv;
	g_Transform.matProj = m_matProj;
	g_Transform.matProjInv = m_matProjInv;

	for (size_t i = 0; i < m_vecShadow.size(); ++i)
	{
		m_vecShadow[i]->Transform()->UpdateData();
		pShadowMapMtrl->UpdateData();
		m_vecShadow[i]->GetRenderComponent()->GetMesh()->render(0);
	}

	m_vecShadow.clear();
}

#include "CKeyMgr.h"
void CCamera::CalculateRay()
{	
	// 마우스 방향을 향하는 Ray 구하기
	// SwapChain 타겟의 ViewPort 정보
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

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
	if (m_pShake.get()) {
		fout << 1 << endl;
		fout << *m_pShake.get() << endl;
	}
	else {
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
	if (exist) {
		fin >> *m_pShake.get();
		m_pShake->RegistCamera(GetOwner());
	}
}
