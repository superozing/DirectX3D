#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"
#include "ParamUI.h"


MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", "##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetComponentTitle("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("MeshRender")) return;

	CGameObject* pTarget = GetTargetObject();
	CMeshRender* pMeshRender = pTarget->MeshRender();

	vector<tMtrlSet> vMtrl = pMeshRender->GetVecMtrls();
	Ptr<CMesh> pMesh = pMeshRender->GetMesh();

	string meshname, mtrlname;

	if (nullptr != pMesh)
	{
		meshname = ToString(pMesh->GetKey()).c_str();
	}
		
	ImGui::Text("Mesh    ");
	ImGui::SameLine(); 
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);

	// Mesh Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::MESH == pAsset->GetType())
			{
				GetTargetObject()->MeshRender()->SetMesh((CMesh*)pAsset);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(20, 20)))
	{
		// 리스트 UI
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMeshName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		pListUI->AddString(vecMeshName);
		//pListUI->SetDbClickCallBack(MeshSelect);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MeshSelect);
		pListUI->Activate();
	}

	for (size_t i = 0; i < vMtrl.size(); ++i)
	{
		Ptr<CMaterial> pMtrl = vMtrl[i].pCurMtrl;

		if (nullptr != pMtrl)
		{
			mtrlname = ToString(pMtrl->GetKey()).c_str();
		}

		string label = "Material" + std::to_string(i + 1);

		if (ImGui::TreeNodeEx(label.c_str()))
		{
			ImGui::Text("Material");
			ImGui::SameLine();
			ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();

			// Material Drop 체크
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

				if (payload)
				{
					DWORD_PTR data = *((DWORD_PTR*)payload->Data);
					CAsset* pAsset = (CAsset*)data;
					if (ASSET_TYPE::MATERIAL == pAsset->GetType())
					{
						pMeshRender->SetMaterial((CMaterial*)pAsset, i);
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
			{
				// 리스트 UI
				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

				vector<string> vecMtrlName;
				CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

				pListUI->AddString(vecMtrlName);
				pListUI->SetDbClickDelegate(this, (Delegate_3)&MeshRenderUI::MaterialSelect, i);
				pListUI->Activate();
			}

			ImGui::Separator(); ImGui::Spacing();
			ImGui::Text("Material Parameter"); ImGui::Spacing();

			if (GetTargetObject()->MeshRender() && GetTargetObject()->MeshRender()->GetMaterial(i).Get())
			{
				Ptr<CGraphicsShader> pShader = GetTargetObject()->MeshRender()->GetMaterial(i)->GetShader();

				// Shader Parameter
				if (nullptr != pShader)
				{
					const auto& vecScalarParam = pShader->GetScalarParam();

					for (size_t j = 0; j < (UINT)SCALAR_PARAM::END; ++j)
					{
						if (vecScalarParam[j].IsUse)
						{
							switch (vecScalarParam[j].Type)
							{
							case SCALAR_PARAM::BOOL_0:
							case SCALAR_PARAM::BOOL_1:
							case SCALAR_PARAM::BOOL_2:
							case SCALAR_PARAM::BOOL_3:
								ParamUI::Param_BOOL((bool*)GetTargetObject()->MeshRender()->GetMaterial(i)->GetScalarParam(vecScalarParam[j].Type), vecScalarParam[j].Desc, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::INT_0:
							case SCALAR_PARAM::INT_1:
							case SCALAR_PARAM::INT_2:
							case SCALAR_PARAM::INT_3:
								ParamUI::Param_INT((int*)GetTargetObject()->MeshRender()->GetMaterial(i)->GetScalarParam(vecScalarParam[j].Type), vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::FLOAT_0:
							case SCALAR_PARAM::FLOAT_1:
							case SCALAR_PARAM::FLOAT_2:
							case SCALAR_PARAM::FLOAT_3:
								ParamUI::Param_FLOAT((float*)GetTargetObject()->MeshRender()->GetMaterial(i)->GetScalarParam(vecScalarParam[j].Type), vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::VEC2_0:
							case SCALAR_PARAM::VEC2_1:
							case SCALAR_PARAM::VEC2_2:
							case SCALAR_PARAM::VEC2_3:
								ParamUI::Param_VEC2((Vec2*)GetTargetObject()->MeshRender()->GetMaterial(i)->GetScalarParam(vecScalarParam[j].Type), vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::VEC4_0:
							case SCALAR_PARAM::VEC4_1:
							case SCALAR_PARAM::VEC4_2:
							case SCALAR_PARAM::VEC4_3:
								ParamUI::Param_VEC4((Vec4*)GetTargetObject()->MeshRender()->GetMaterial(i)->GetScalarParam(vecScalarParam[j].Type), vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::MAT_0:
							case SCALAR_PARAM::MAT_1:
							case SCALAR_PARAM::MAT_2:
							case SCALAR_PARAM::MAT_3:
								break;
							}
						}
					}
				}
			}

			ImGui::TreePop();
		}
	}


	//ImGui::Text("Material"); 
	//ImGui::SameLine(); 
	//ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);
	//ImGui::SameLine();

	//// Material Drop 체크
	//if (ImGui::BeginDragDropTarget())
	//{
	//	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

	//	if (payload)
	//	{
	//		DWORD_PTR data = *((DWORD_PTR*)payload->Data);
	//		CAsset* pAsset = (CAsset*)data;
	//		if (ASSET_TYPE::MATERIAL == pAsset->GetType())
	//		{
	//			GetTargetObject()->MeshRender()->SetMaterial((CMaterial*)pAsset, 0);
	//		}
	//	}
	//	ImGui::EndDragDropTarget();
	//}


	//if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
	//{
	//	// 리스트 UI
	//	ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

	//	vector<string> vecMtrlName;
	//	CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

	//	pListUI->AddString(vecMtrlName);
	//	pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MaterialSelect);
	//	pListUI->Activate();
	//}
}

void MeshRenderUI::MeshSelect(DWORD_PTR _ptr)
{
	string strMesh = (char*)_ptr;
	wstring strMeshName = ToWString(strMesh);

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshName);

	GetTargetObject()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::MaterialSelect(DWORD_PTR _ptr, UINT _idx)
{
	string strMtrl = (char*)_ptr;
	wstring strMtrlName = ToWString(strMtrl);

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMtrlName);

	GetTargetObject()->MeshRender()->SetMaterial(pMtrl, _idx);
}