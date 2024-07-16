#include "pch.h"
#include "MeshDataUI.h"

#include <Engine/CMeshData.h>
#include "ListUI.h"
#include "ParamUI.h"

MeshDataUI::MeshDataUI()
	: AssetUI("MeshData", "##MeshData", ASSET_TYPE::MESHDATA)
{
}

MeshDataUI::~MeshDataUI()
{
}

void MeshDataUI::render_update()
{
	AssetUI::render_update();

	Ptr<CMesh> pMesh = m_pTarget->m_pMesh;

	string meshname, mtrlname;

	if (nullptr != pMesh)
	{
		meshname = ToString(pMesh->GetKey()).c_str();
	}

	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);

	// Mesh payload 체크
	CMesh* PayloadMesh = nullptr;
	if (PayloadCheck(&PayloadMesh))
	{
		m_pTarget->m_pMesh = (CMesh*)PayloadMesh;
	}

	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(20, 20)))
	{
		// 리스트 UI
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMeshName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		pListUI->AddString(vecMeshName);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshDataUI::MeshSelect);
		pListUI->Activate();
	}

	m_pTarget->m_vecMtrl;

	for (size_t i = 0; i < m_pTarget->m_vecMtrl.size(); ++i)
	{
		Ptr<CMaterial> pMtrl = m_pTarget->m_vecMtrl[i];

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

			// Material payload 체크
			CMaterial* PayloadMaterial = nullptr;
			if (PayloadCheck(&PayloadMaterial))
			{
				m_pTarget->m_vecMtrl[i] = PayloadMaterial;
			}

			if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
			{
				// 리스트 UI
				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

				vector<string> vecMtrlName;
				CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

				pListUI->AddString(vecMtrlName);
				pListUI->SetDbClickDelegate(this, (Delegate_3)&MeshDataUI::MaterialSelect, i);
				pListUI->Activate();
			}

			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Material Parameter");
			ImGui::Spacing();

			if (m_pTarget->m_vecMtrl[i].Get())
			{
				Ptr<CGraphicsShader> pShader = m_pTarget->m_vecMtrl[i]->GetShader();

				// Shader Parameter
				if (nullptr != pShader)
				{
					const auto& vecScalarParam = pShader->GetScalarParam();
					const auto& vecTexParam	   = pShader->GetTexParam();

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
								ParamUI::Param_BOOL(
									(bool*)m_pTarget->m_vecMtrl[i]->GetScalarParam(vecScalarParam[j].Type),
									vecScalarParam[j].Desc, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::INT_0:
							case SCALAR_PARAM::INT_1:
							case SCALAR_PARAM::INT_2:
							case SCALAR_PARAM::INT_3:
								ParamUI::Param_INT(
									(int*)m_pTarget->m_vecMtrl[i]->GetScalarParam(vecScalarParam[j].Type),
									vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
									vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::FLOAT_0:
							case SCALAR_PARAM::FLOAT_1:
							case SCALAR_PARAM::FLOAT_2:
							case SCALAR_PARAM::FLOAT_3:
								ParamUI::Param_FLOAT(
									(float*)m_pTarget->m_vecMtrl[i]->GetScalarParam(vecScalarParam[j].Type),
									vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
									vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::VEC2_0:
							case SCALAR_PARAM::VEC2_1:
							case SCALAR_PARAM::VEC2_2:
							case SCALAR_PARAM::VEC2_3:
								ParamUI::Param_VEC2(
									(Vec2*)m_pTarget->m_vecMtrl[i]->GetScalarParam(vecScalarParam[j].Type),
									vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
									vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::VEC4_0:
							case SCALAR_PARAM::VEC4_1:
							case SCALAR_PARAM::VEC4_2:
							case SCALAR_PARAM::VEC4_3:
								ParamUI::Param_VEC4(
									(Vec4*)m_pTarget->m_vecMtrl[i]->GetScalarParam(vecScalarParam[j].Type),
									vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
									vecScalarParam[j].View, vecScalarParam[j].Tooltip);
								break;
							case SCALAR_PARAM::MAT_0:
							case SCALAR_PARAM::MAT_1:
							case SCALAR_PARAM::MAT_2:
							case SCALAR_PARAM::MAT_3:
								break;
							}
						}
					}

					for (size_t j = 0; j < (UINT)TEX_PARAM::END; j++)
					{
						CTexture* PayloadTexture = nullptr;

						if (vecTexParam[j].IsUse)
						{
							switch (vecTexParam[j].Type)
							{
							case TEX_PARAM::TEX_0:
							case TEX_PARAM::TEX_1:
							case TEX_PARAM::TEX_2:
							case TEX_PARAM::TEX_3:
							case TEX_PARAM::TEX_4:
							case TEX_PARAM::TEX_5: {
								Ptr<CTexture> pTex = m_pTarget->m_vecMtrl[i]->GetTexParam(vecTexParam[j].Type);
								ParamUI::Param_TEXTURE(pTex, vecTexParam[j].Desc);

								// Texture payload 체크
								if (PayloadCheck(&PayloadTexture))
								{
									m_pTarget->m_vecMtrl[i]->SetTexParam((TEX_PARAM)j, PayloadTexture);
								}
								break;
							}
							case TEX_PARAM::TEXCUBE_0:
								break;
							case TEX_PARAM::TEXCUBE_1:
								break;
							case TEX_PARAM::TEXARR_0:
								break;
							case TEX_PARAM::TEXARR_1:
								break;
							case TEX_PARAM::END:
								break;
							default:
								break;
							}
						}
					}
				}
			}

			ImGui::TreePop();
		}
	}
}

void MeshDataUI::Activate()
{
	AssetUI::Activate();
	m_pTarget = (CMeshData*)GetAsset().Get();
}

void MeshDataUI::MeshSelect(DWORD_PTR _ptr)
{
	string	strMesh		= (char*)_ptr;
	wstring strMeshName = ToWString(strMesh);

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshName);

	m_pTarget->m_pMesh = pMesh;
}

void MeshDataUI::MaterialSelect(DWORD_PTR _ptr, UINT _idx)
{
	string	strMtrl		= (char*)_ptr;
	wstring strMtrlName = ToWString(strMtrl);

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMtrlName);

	m_pTarget->m_vecMtrl[_idx] = pMtrl;
}
