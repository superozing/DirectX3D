#include "pch.h"
#include "PrefabUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>


#include "Inspector.h"

PrefabUI::PrefabUI()
	: AssetUI("Prefab", "##Prefab", ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::render_update()
{
	AssetUI::render_update();

	ImGui::Separator();

	Ptr<CPrefab> pPrefab = (CPrefab*)GetAsset().Get();
	string strPath = ToString(pPrefab->GetRelativePath());
	
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pPrefab->m_ProtoObj, true);
}