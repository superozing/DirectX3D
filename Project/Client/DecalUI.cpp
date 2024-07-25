#include "pch.h"
#include "DecalUI.h"

#include <Engine\CDecal.h>
#include <Engine\CMaterial.h>

#include "ListUI.h"
#include "ParamUI.h"

DecalUI::DecalUI()
	: ComponentUI("Decal", "##Decal", COMPONENT_TYPE::DECAL)
	, strDecalMtrl(" ")
	, strEmissiveCondition(" ")
	, iDecalPriority(0)
	, fDecalRenderDistance(0)
	, strCustomAlpha(" ")
	, fCustomAlpha(1.f)
	, fDecalAnimationActionTime(0.f)
	, AnimationOutlinerColor(0.f, 0.f, 0.f, 1.f)
	, AnimationOutsideColor(0.f, 0.f, 0.f, 1.f)
	, AnimationInsideColor(0.f, 0.f, 0.f, 1.f)
{
	SetComponentTitle("Decal");
	strEmissiveCondition = "Emissive Off";
	strCustomAlpha		 = "Alpha Off";
}

DecalUI::~DecalUI()
{
}

#include <Engine\CLogMgr.h>
void DecalUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Decal"))
		return;

	CGameObject*	  pTarget  = GetTargetObject();
	CRenderComponent* pRenComp = pTarget->GetRenderComponent();

	string meshname, mtrlname;
	UIDecalShape			  = pTarget->Decal()->GetDecalShape();
	UIDecalType				  = pTarget->Decal()->GetDecalType();
	fDecalAnimationActionTime = pTarget->Decal()->GetAnimationActionTime();

	AnimationOutlinerColor = pTarget->Decal()->GetAnimationOutlinerColor();
	AnimationOutsideColor  = pTarget->Decal()->GetAnimationOutsideColor();
	AnimationInsideColor   = pTarget->Decal()->GetAnimationInsideColor();

	Ptr<CMaterial> pMtrl = pRenComp->GetMaterial(0);

	if (nullptr != pMtrl)
	{
		mtrlname = ToString(pMtrl->GetKey()).c_str();
	}

	string label = "Material" + std::to_string(0);

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
			GetTargetObject()->GetRenderComponent()->SetMaterial((CMaterial*)PayloadMaterial, 0);
		}

		if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
		{
			// 리스트 UI
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

			vector<string> vecMtrlName;
			CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

			pListUI->AddString(vecMtrlName);
			pListUI->SetDbClickDelegate(this, (Delegate_3)&DecalUI::MaterialSelect, 0);
			pListUI->Activate();
		}

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Material Parameter");
		ImGui::Spacing();

		if (GetTargetObject()->GetRenderComponent() && GetTargetObject()->GetRenderComponent()->GetMaterial(0).Get())
		{
			Ptr<CGraphicsShader> pShader = GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetShader();

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
								(bool*)GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetScalarParam(
									vecScalarParam[j].Type),
								vecScalarParam[j].Desc, vecScalarParam[j].View, vecScalarParam[j].Tooltip);
							break;
						case SCALAR_PARAM::INT_0:
						case SCALAR_PARAM::INT_1:
						case SCALAR_PARAM::INT_2:
						case SCALAR_PARAM::INT_3:
							ParamUI::Param_INT(
								(int*)GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetScalarParam(
									vecScalarParam[j].Type),
								vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
								vecScalarParam[j].View, vecScalarParam[j].Tooltip);
							break;
						case SCALAR_PARAM::FLOAT_0:
						case SCALAR_PARAM::FLOAT_1:
						case SCALAR_PARAM::FLOAT_2:
						case SCALAR_PARAM::FLOAT_3:
							ParamUI::Param_FLOAT(
								(float*)GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetScalarParam(
									vecScalarParam[j].Type),
								vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
								vecScalarParam[j].View, vecScalarParam[j].Tooltip);
							break;
						case SCALAR_PARAM::VEC2_0:
						case SCALAR_PARAM::VEC2_1:
						case SCALAR_PARAM::VEC2_2:
						case SCALAR_PARAM::VEC2_3:
							ParamUI::Param_VEC2(
								(Vec2*)GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetScalarParam(
									vecScalarParam[j].Type),
								vecScalarParam[j].Desc, vecScalarParam[j].Min, vecScalarParam[j].Max,
								vecScalarParam[j].View, vecScalarParam[j].Tooltip);
							break;
						case SCALAR_PARAM::VEC4_0:
						case SCALAR_PARAM::VEC4_1:
						case SCALAR_PARAM::VEC4_2:
						case SCALAR_PARAM::VEC4_3:
							ParamUI::Param_VEC4(
								(Vec4*)GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetScalarParam(
									vecScalarParam[j].Type),
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
							Ptr<CTexture> pTex = GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetTexParam(
								vecTexParam[j].Type);
							ParamUI::Param_TEXTURE(pTex, vecTexParam[j].Desc);
							GetTargetObject()->GetRenderComponent()->GetMaterial(0)->SetTexParam(vecTexParam[j].Type,
																								 pTex);

							// Texture payload 체크
							if (PayloadCheck(&PayloadTexture))
							{
								GetTargetObject()->GetRenderComponent()->GetMaterial(0)->SetTexParam((TEX_PARAM)j,
																									 PayloadTexture);
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

	ImGui::SeparatorText("Decal Option");

	ImGui::Text("Decal Shape : ");
	ImGui::SameLine();

	string strDecalShape = ToString(magic_enum::enum_name(UIDecalShape));

	if (ImGui::Button(strDecalShape.c_str()))
	{
		if (strDecalShape == "Rect")
		{
			GetTargetObject()->Decal()->SetDecalShape(DecalShape::Circle);
		}
		else
		{
			GetTargetObject()->Decal()->SetDecalShape(DecalShape::Rect);
		}
	}

	ImGui::Text("Decal Type : ");
	ImGui::SameLine();

	string strDecalType = ToString(magic_enum::enum_name(UIDecalType));

	if (ImGui::Button(strDecalType.c_str()))
	{
		if (strDecalType == "Texture")
		{
			GetTargetObject()->Decal()->SetDecalType(DecalType::Animation);
		}
		else
		{
			GetTargetObject()->Decal()->SetDecalType(DecalType::Texture);
		}
	}

	ImGui::Text("Decal Emissive : ");
	ImGui::SameLine();

	// Emissive 설정
	if (ImGui::Button(strEmissiveCondition.c_str(), ImVec2(120.f, 20.f)))
	{
		int iEmissive = GetTargetObject()->Decal()->GetDecalAsEmissive();

		if (iEmissive > 0)
		{
			strEmissiveCondition = "Emissive Off";
			GetTargetObject()->Decal()->SetDecalAsEmissive(0);
		}
		else
		{
			strEmissiveCondition = "Emissive On";
			GetTargetObject()->Decal()->SetDecalAsEmissive(1);
		}
	}

	ImGui::Text("Decal Priority");
	ImGui::SameLine();

	iDecalPriority = GetTargetObject()->Decal()->GetDecalPriority();
	if (ImGui::InputInt("##Decal Priority", &iDecalPriority))
	{
		if (iDecalPriority > DecalPriorityMax)
			iDecalPriority = DecalPriorityMax;

		if (iDecalPriority < 0)
			iDecalPriority = 0;

		GetTargetObject()->Decal()->SetDecalPriority(iDecalPriority);
	}

	ImGui::Text("Decal Render Distance");
	ImGui::SameLine();

	fDecalRenderDistance = GetTargetObject()->Decal()->GetRenderDistance();
	if (ImGui::DragFloat("##DecalRenderDistance", &fDecalRenderDistance))
	{
		GetTargetObject()->Decal()->SetRenderDistance(fDecalRenderDistance);
	}

	ImGui::Text("Decal Custom Alpha : ");
	ImGui::SameLine();

	// CustomAlpha 설정
	if (ImGui::Button(strCustomAlpha.c_str(), ImVec2(120.f, 20.f)))
	{
		int iCAlpha = GetTargetObject()->Decal()->GetUseCustomAlpha();

		if (iCAlpha > 0)
		{
			strCustomAlpha = "Alpha Off";
			GetTargetObject()->Decal()->SetUseCustomAlpha(0);
		}
		else
		{
			strCustomAlpha = "Alpha On";
			GetTargetObject()->Decal()->SetUseCustomAlpha(1);
		}
	}

	ImGui::Text("Decal Custom Alpha");
	ImGui::SameLine();

	fCustomAlpha = GetTargetObject()->Decal()->GetCustomAlpha();
	if (ImGui::DragFloat("##DecalCustomfAlpha", &fCustomAlpha, 0.01f, 0.f, 1.f))
	{
		GetTargetObject()->Decal()->SetCustomAlpha(fCustomAlpha);
	}

	// 데칼 에니메이션 옵션 설정

	if (UIDecalType == DecalType::Animation)
	{
		ImGui::SeparatorText("Animation Option");

		ImGui::Text("Decal Acction Time : ");
		ImGui::SameLine();

		if (ImGui::InputFloat("##Decal Animation", &fDecalAnimationActionTime))
		{
			pTarget->Decal()->SetAnimationActionOriginTime(fDecalAnimationActionTime);
			pTarget->Decal()->SyncTime();
		}

		ImGui::SameLine();

		if (ImGui::Button("Reset"))
		{
			pTarget->Decal()->SyncTime();
		}

		static Vec3 ColorPicker[3];

		ColorPicker[0] = pTarget->Decal()->GetAnimationOutlinerColor();
		ColorPicker[1] = pTarget->Decal()->GetAnimationInsideColor();
		ColorPicker[2] = pTarget->Decal()->GetAnimationOutsideColor();

		ImGui::Text("Outliner Color");
		ImGui::SameLine();
		ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##AnimationDecalOutlinerColor", ColorPicker[0],
							ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Inside Color");
		ImGui::SameLine();
		ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##AnimationDecalInsideColor", ColorPicker[1],
							ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Outside Color");
		ImGui::SameLine();
		ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##AninationDecalOusideColor", ColorPicker[2],
							ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		static float AnimDecalAlpha[3] = {1.f, 1.f, 1.f};

		AnimationOutlinerColor = Vec4(ColorPicker[0], AnimDecalAlpha[0]);
		AnimationInsideColor   = Vec4(ColorPicker[1], AnimDecalAlpha[1]);
		AnimationOutsideColor  = Vec4(ColorPicker[2], AnimDecalAlpha[2]);

		ImGui::Text("Outliner Color : ");
		ImGui::SameLine();
		ImGui::InputFloat4("##OutlinerColor", AnimationOutlinerColor);

		pTarget->Decal()->SetAnimationOutlinerColor(AnimationOutlinerColor);

		ImGui::Text("Inside Color : ");
		ImGui::SameLine();
		ImGui::InputFloat4("##InsideColor", AnimationInsideColor);

		pTarget->Decal()->SetAnimationInsideColor(AnimationInsideColor);

		ImGui::Text("Outside Color : ");
		ImGui::SameLine();

		ImGui::InputFloat4("##OutsideColor", AnimationOutsideColor);

		pTarget->Decal()->SetAnimationOutsideColor(AnimationOutsideColor);
	}
}

void DecalUI::MaterialSelect(DWORD_PTR _ptr)
{
	string	strMtrl		= (char*)_ptr;
	wstring strMtrlName = ToWString(strMtrl);

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMtrlName);

	GetTargetObject()->GetRenderComponent()->SetMaterial(pMtrl, 0);
}
