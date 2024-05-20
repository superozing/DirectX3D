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
	CGameObject* pObj = pPrefab->Instantiate();
	//CGameObject* pObj = pPrefab->GetProtoObject();
	//ImGui::Text("Prefab");
	//ImGui::SameLine();
	//ImGui::InputText("##PrefabName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);


	//if (ImGui::Button("Spawn"))
	//{
	//	CGameObject* pNewObj = pPrefab->Instantiate();
	//	Vec3 vPos = pNewObj->Transform()->GetRelativePos();
	//	pNewObj->Transform()->SetRelativePos(vPos);
	//	GamePlayStatic::SpawnGameObject(pNewObj, 0);
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Edit"))
	//{
	//	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##PrefabInspector");
	//	
	//	if (nullptr == pInspector)
	//	{
	//		pInspector = new Inspector(pObj);
	//		CImGuiMgr::GetInst()->AddUI(pInspector->GetID(), pInspector);
	//	}
	//	else
	//	{
	//		pInspector->Activate();
	//	}

	//	pInspector->SetTargetObject(pObj);
	//}

	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pObj, true);
}