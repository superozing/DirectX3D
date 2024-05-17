#include "pch.h"
#include "PrefabUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

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

	ImGui::Text("Prefab");
	ImGui::SameLine();
	ImGui::InputText("##PrefabName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);


	if (ImGui::Button("Spawn"))
	{
		CGameObject* pNewObj = pPrefab->Instantiate();
		Vec3 vPos = pNewObj->Transform()->GetRelativePos();
		pNewObj->Transform()->SetRelativePos(vPos);
		GamePlayStatic::SpawnGameObject(pNewObj, 0);
	}

}