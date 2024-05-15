#include "pch.h"
#include "GraphicsShaderUI.h"

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

	string strShaderName = ToString(pShader->GetKey());

	ImGui::Separator();

	ImGui::Text("Graphics Shader ");
	ImGui::SameLine();
	ImGui::InputText("##GraphicsShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);

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
		ImGui::InputText("##ShaderTopology", (char*)strEnum.c_str(), strEnum.length(), ImGuiInputTextFlags_ReadOnly);
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetRSType()));
	if (!strEnum.empty())
	{
		ImGui::Text("Resterizer State   ");
		ImGui::SameLine();
		ImGui::InputText("##ResterizerState", (char*)strEnum.c_str(), strEnum.length(), ImGuiInputTextFlags_ReadOnly);
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetDSType()));
	if (!strEnum.empty())
	{
		ImGui::Text("Depth Stancil State");
		ImGui::SameLine();
		ImGui::InputText("##DepthStancilState", (char*)strEnum.c_str(), strEnum.length(), ImGuiInputTextFlags_ReadOnly);
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetBSType()));
	if (!strEnum.empty())
	{
		ImGui::Text("Blend State        ");
		ImGui::SameLine();
		ImGui::InputText("##BlendState", (char*)strEnum.c_str(), strEnum.length(), ImGuiInputTextFlags_ReadOnly);
	}

	strEnum = ToString(magic_enum::enum_name(pShader->GetDomain()));
	if (!strEnum.empty())
	{
		ImGui::Text("Shader Domain      ");
		ImGui::SameLine();
		ImGui::InputText("##ShaderDomain", (char*)strEnum.c_str(), strEnum.length(), ImGuiInputTextFlags_ReadOnly);
	}

}
