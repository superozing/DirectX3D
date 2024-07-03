#include "pch.h"
#include "CCrosshair.h"

#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

namespace CROSSHAIR
{

enum idx
{
	IDX_UP,
	IDX_DOWN,
	IDX_LEFT,
	IDX_RIGHT
};

const int PosX[4] = {0, 0, -1, 1};
const int PosY[4] = {1, -1, 0, 0};

} // namespace CROSSHAIR

CCrosshair::CCrosshair()
	: CScript((UINT)SCRIPT_TYPE::CROSSHAIR)
{
}

CCrosshair::~CCrosshair()
{
}

void CCrosshair::begin()
{
	AppendScriptParam("Min Spread", SCRIPT_PARAM::FLOAT, &m_fMinSpread, 0.f, 1000.f);
	AppendScriptParam("Max Spread", SCRIPT_PARAM::FLOAT, &m_fMaxSpread, 0.f, 1000.f);
	AppendScriptParam("Spread Ratio", SCRIPT_PARAM::FLOAT, &m_fSpreadRatio, 0.f, 0.f);

	// 부모 오브젝트에 패널UI 추가
	SetParentPanelUI();

	using namespace CROSSHAIR;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pObj = new CGameObject;
		m_pCrossHair[i]	  = new CImageUIScript;

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(m_pCrossHair[i]);

		pObj->Transform()->SetRelativePos(Vec3(PosX[i], PosY[i], 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
		pObj->MeshRender()->GetDynamicMaterial(0);

		m_pCrossHair[i]->AllowBindTexPerFrame();

		GetOwner()->AddChild(pObj);
	}

	auto pTexture = CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Aim_Length.png");

	m_pCrossHair[IDX_UP]->GetOwner()->SetName("Crosshair_Up");
	m_pCrossHair[IDX_UP]->SetUIImg(pTexture);

	m_pCrossHair[IDX_DOWN]->GetOwner()->SetName("Crosshair_Down");
	m_pCrossHair[IDX_DOWN]->SetUIImg(pTexture);

	pTexture = CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Aim_Width.png");

	m_pCrossHair[IDX_LEFT]->GetOwner()->SetName("Crosshair_Left");
	m_pCrossHair[IDX_LEFT]->SetUIImg(pTexture);

	m_pCrossHair[IDX_RIGHT]->GetOwner()->SetName("Crosshair_Right");
	m_pCrossHair[IDX_RIGHT]->SetUIImg(pTexture);
}

void CCrosshair::tick()
{
	using namespace CROSSHAIR;

	float fSpreadVal = max((m_fMaxSpread * m_fSpreadRatio), m_fMinSpread);

	for (int i = 0; i < 4; ++i)
		m_pCrossHair[i]->Transform()->SetRelativePos(Vec3(PosX[i] * fSpreadVal, PosY[i] * fSpreadVal, 0.f));
}

void CCrosshair::SetParentPanelUI()
{
	auto pOwn = GetOwner();

	m_pPanelUI = pOwn->GetScript<CPanelUIScript>();

	if (!m_pPanelUI)
	{
		m_pPanelUI = new CPanelUIScript;
		pOwn->AddComponent(m_pPanelUI);
	}

	m_pPanelUI->DisableMouseInput();
	m_pPanelUI->DisallowDragAndDrop();
	m_pPanelUI->DisallowTexSet();

	auto meshrender = pOwn->MeshRender();

	if (!meshrender)
	{
		meshrender = new CMeshRender;
		pOwn->AddComponent(meshrender);
	}

	meshrender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	meshrender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	meshrender->GetDynamicMaterial(0);
}