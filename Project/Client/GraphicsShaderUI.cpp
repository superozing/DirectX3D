#include "pch.h"
#include "GraphicsShaderUI.h"
#include "CImGuiMgr.h"

GraphicsShader::GraphicsShader()
	: AssetUI("GraphicsShader", "##GraphicsShader", ASSET_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShader::~GraphicsShader()
{
}


void GraphicsShader::render_update()
{
	AssetUI::render_update();
	
	Ptr<CGraphicsShader> pShader = (CGraphicsShader*)GetAsset().Get();

	ImGui::SameLine();

	if (ImGui::Button("Save to file"))
	{
		pShader->Save(pShader->GetKey());
	}

	string strShaderName = ToString(pShader->GetKey());

	ImGui::Separator();

	ImGui::Text("Graphics Shader ");
	ImGui::SameLine();
	
	using namespace std::filesystem;

	path filePath = strShaderName;

	// 확장자 제거 - stem() 함수를 사용하면 파일 명만 가져올 수 있어요.
	string strFileName = filePath.stem().string();

	ImGui::InputText("##GraphicsShaderName", (char*)strFileName.c_str(), strFileName.length(), ImGuiInputTextFlags_ReadOnly);


	ImGui::SeparatorText("Shader Info");

	string strPath;
	string strFuncName;

	// VertexShader
	pShader->GetVSInfo(strPath, strFuncName);
	if (!strPath.empty())
	{
		ImGui::Text("VS File Path    ");
		ImGui::SameLine();
		ImGui::InputText("##VS_Path", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("VS Func Name    ");
		ImGui::SameLine();
		ImGui::InputText("##VS_Func", (char*)strFuncName.c_str(), strFuncName.length(), ImGuiInputTextFlags_ReadOnly);
	}

	// HullShader
	pShader->GetHSInfo(strPath, strFuncName);
	if (!strPath.empty())
	{
		ImGui::Text("HS File Path    ");
		ImGui::SameLine();
		ImGui::InputText("##HS_Path", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("HS Func Name    ");
		ImGui::SameLine();
		ImGui::InputText("##HS_Func", (char*)strFuncName.c_str(), strFuncName.length(), ImGuiInputTextFlags_ReadOnly);
	}

	// DomainShader
	pShader->GetDSInfo(strPath, strFuncName);
	if (!strPath.empty())
	{
		ImGui::Text("DS File Path    ");
		ImGui::SameLine();
		ImGui::InputText("##DS_Path", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("DS Func Name    ");
		ImGui::SameLine();
		ImGui::InputText("##DS_Func", (char*)strFuncName.c_str(), strFuncName.length(), ImGuiInputTextFlags_ReadOnly);
	}

	// GeometryShader
	pShader->GetGSInfo(strPath, strFuncName);
	if (!strPath.empty())
	{
		ImGui::Text("GS File Path    ");
		ImGui::SameLine();
		ImGui::InputText("##GS_Path", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("GS Func Name    ");
		ImGui::SameLine();
		ImGui::InputText("##GS_Func", (char*)strFuncName.c_str(), strFuncName.length(), ImGuiInputTextFlags_ReadOnly);
	}

	// PixelShader
	pShader->GetPSInfo(strPath, strFuncName);
	if (!strPath.empty())
	{
		ImGui::Text("PS File Path    ");
		ImGui::SameLine();
		ImGui::InputText("##PS_Path", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("PS Func Name    ");
		ImGui::SameLine();
		ImGui::InputText("##PS_Func", (char*)strFuncName.c_str(), strFuncName.length(), ImGuiInputTextFlags_ReadOnly);
	}



	ImGui::SeparatorText("Shader State ");



	string strEnum = ToString(magic_enum::enum_name(pShader->GetTopology()));
	if (!strEnum.empty())
	{
		ImGui::Text("Topology           ");
		ImGui::SameLine();

		auto SelectedTopology = pShader->GetTopology();

		const vector<string>& vecTopologyNames = CImGuiMgr::GetInst()->GetVecEnumTopology();

		if (ImGui::BeginCombo("##ShaderTopology", (char*)strEnum.c_str()))
		{
			for (int i = 0; i < (int)vecTopologyNames.size(); ++i)
			{
				auto topologyBuffer = (D3D11_PRIMITIVE_TOPOLOGY)i;

				bool isSelected = (topologyBuffer == SelectedTopology);

				if (vecTopologyNames[(UINT)topologyBuffer] != "" 
					&& ImGui::Selectable(vecTopologyNames[(UINT)topologyBuffer].c_str(), isSelected))
				{
					SelectedTopology = topologyBuffer;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();

			if (pShader->GetTopology() != SelectedTopology)
			{
				pShader->SetTopology(SelectedTopology);
			}
		}
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetRSType()));
	if (!strEnum.empty())
	{
		ImGui::Text("Resterizer State   ");
		ImGui::SameLine();

		RS_TYPE SelectedType = pShader->GetRSType();

		const vector<string>& vecTypeNames = CImGuiMgr::GetInst()->GetVecEnumRS();

		if (ImGui::BeginCombo("##ResterizerState", (char*)strEnum.c_str()))
		{
			for (int i = 0; i < (int)vecTypeNames.size(); ++i)
			{
				RS_TYPE TypeBuffer = (RS_TYPE)i;

				bool isSelected = (TypeBuffer == SelectedType);

				if (vecTypeNames[(UINT)TypeBuffer] != ""
					&& ImGui::Selectable(vecTypeNames[(UINT)TypeBuffer].c_str(), isSelected))
				{
					SelectedType = TypeBuffer;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();

			if (pShader->GetRSType() != SelectedType)
			{
				pShader->SetRSType(SelectedType);
			}
		}
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetDSType()));
	if (!strEnum.empty())
	{
		ImGui::Text("Depth Stancil State");
		ImGui::SameLine();
	
		DS_TYPE SelectedType = pShader->GetDSType();

		const vector<string>& vecTypeNames = CImGuiMgr::GetInst()->GetVecEnumDS();

		if (ImGui::BeginCombo("##DepthStancilState", (char*)strEnum.c_str()))
		{
			for (int i = 0; i < (int)vecTypeNames.size(); ++i)
			{
				DS_TYPE TypeBuffer = (DS_TYPE)i;

				bool isSelected = (TypeBuffer == SelectedType);

				if (vecTypeNames[(UINT)TypeBuffer] != ""
					&& ImGui::Selectable(vecTypeNames[(UINT)TypeBuffer].c_str(), isSelected))
				{
					SelectedType = TypeBuffer;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();

			if (pShader->GetDSType() != SelectedType)
			{
				pShader->SetDSType(SelectedType);
			}
		}
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetBSType()));
	if (!strEnum.empty())
	{
		ImGui::Text("Blend State        ");
		ImGui::SameLine();

		BS_TYPE SelectedType = pShader->GetBSType();

		const vector<string>& vecTypeNames = CImGuiMgr::GetInst()->GetVecEnumBS();

		if (ImGui::BeginCombo("##BlendState", (char*)strEnum.c_str()))
		{
			for (int i = 0; i < (int)vecTypeNames.size(); ++i)
			{
				BS_TYPE TypeBuffer = (BS_TYPE)i;

				bool isSelected = (TypeBuffer == SelectedType);

				if (vecTypeNames[(UINT)TypeBuffer] != ""
					&& ImGui::Selectable(vecTypeNames[(UINT)TypeBuffer].c_str(), isSelected))
				{
					SelectedType = TypeBuffer;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();

			if (pShader->GetBSType() != SelectedType)
			{
				pShader->SetBSType(SelectedType);
			}
		}
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetDomain()));
	if (!strEnum.empty())
	{
		ImGui::Text("Shader Domain      ");
		ImGui::SameLine();
		SHADER_DOMAIN SelectedType = pShader->GetDomain();

		const vector<string>& vecTypeNames = CImGuiMgr::GetInst()->GetVecEnumShaderDomain();

		if (ImGui::BeginCombo("##ShaderDomain", (char*)strEnum.c_str()))
		{
			for (int i = 0; i < (int)vecTypeNames.size(); ++i)
			{
				SHADER_DOMAIN TypeBuffer = (SHADER_DOMAIN)i;

				bool isSelected = (TypeBuffer == SelectedType);

				if (vecTypeNames[(UINT)TypeBuffer] != ""
					&& ImGui::Selectable(vecTypeNames[(UINT)TypeBuffer].c_str(), isSelected))
				{
					SelectedType = TypeBuffer;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();

			if (pShader->GetDomain() != SelectedType)
			{
				pShader->SetDomain(SelectedType);
			}
		}
	}


	ImGui::SeparatorText("");

	if (TitleCollapse("Scalar Param"))
	{
		auto pScalarParam = pShader->GetScalarParam();

		for (UINT i = 0; i < (UINT)SCALAR_PARAM::END; ++i)
		{
			// 1. 사용 여부 (체크 박스)
			auto& Scalar = pScalarParam[i];
			bool IsUse = Scalar.IsUse;

			ImGui::Checkbox(ToString(magic_enum::enum_name(SCALAR_PARAM(i))).c_str(), &IsUse);

			// 체크 박스를 체크했을 경우
			if (IsUse && !Scalar.IsUse)
			{
				pShader->AddScalarParam(SCALAR_PARAM(i), ToString(magic_enum::enum_name(SCALAR_PARAM(i))));
			}
			// 체크 박스를 해제했을 경우
			else if (!IsUse && Scalar.IsUse)
			{
				pShader->DeleteScalarParam(SCALAR_PARAM(i));
			}

			// 2. 사용 여부에 따른 정보 표시 
			if (Scalar.IsUse)
			{
				char cdesc[100]{};
				strcpy_s(cdesc, Scalar.Desc.c_str());
				ImGui::Text("Param Name");			ImGui::SameLine();
				ImGui::InputText(("##Scalar" + to_string(i)).c_str(), cdesc, 100, ImGuiInputTextFlags_None);

				char cscalar[100]{};
				strcpy_s(cscalar, Scalar.Tooltip.c_str());
				ImGui::Text("Tooltip");			ImGui::SameLine();
				ImGui::InputText(("##ScalarTooltip" + to_string(i)).c_str(), cscalar, 100, ImGuiInputTextFlags_None);

				pShader->AddScalarParam(SCALAR_PARAM(i), string(cdesc), Scalar.Min, Scalar.Max, Scalar.View, string(cscalar));
			}


			ImGui::Separator();
		}
	}

	if (TitleCollapse("Tex Param"))
	{
		auto pTexParam = pShader->GetTexParam();

		for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
		{
			// 1. 사용 여부 (체크 박스)
			auto& Tex = pTexParam[i];
			bool IsUse = Tex.IsUse;

			ImGui::Checkbox(ToString(magic_enum::enum_name(TEX_PARAM(i))).c_str(), &IsUse);

			// 체크 박스를 체크했을 경우
			if (IsUse && !Tex.IsUse)
				pShader->AddTexParam(TEX_PARAM(i), ToString(magic_enum::enum_name(TEX_PARAM(i))));

			// 체크 박스를 해제했을 경우
			else if (!IsUse && Tex.IsUse)
				pShader->DeleteTexParam(TEX_PARAM(i));


			// 2. 사용 여부에 따른 정보 표시 
			if (Tex.IsUse)
			{
				char cdesc[100]{};
				strcpy_s(cdesc, Tex.Desc.c_str());
				ImGui::Text("Param Name");			ImGui::SameLine();
				ImGui::InputText(("##Scalar" + to_string(i)).c_str(), cdesc, 100, ImGuiInputTextFlags_None);

				pShader->AddTexParam(TEX_PARAM(i), string(cdesc));
			}

			ImGui::Separator();
		}
	}
}
