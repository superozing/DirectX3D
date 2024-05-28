#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>

#include "CImGuiMgr.h"
#include "ParamUI.h"
#include "ListUI.h"

MaterialUI::MaterialUI()
	: AssetUI("Material", "##Material", ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}


void MaterialUI::render_update()
{
	AssetUI::render_update();

    // 해당 텍스쳐 이미지 출력
    Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
    string strPath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());

    ImGui::Text("Material");
    ImGui::SameLine();
    ImGui::InputText("##TexName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    Ptr<CGraphicsShader> pShader = pMtrl->GetShader();
    string strShaderName;
    if (nullptr != pShader)
    {
        strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
    }
    else
    {
        pMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(SHADER_deferred));
    }

    ImGui::Text("Shader  "); ImGui::SameLine();
    ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();

    if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
    {
        // 리스트 UI
        ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

        vector<string> vecShaderName;
        CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::GRAPHICS_SHADER, vecShaderName);

        pListUI->AddString(vecShaderName);
        pListUI->SetDbClickDelegate(this, (Delegate_1)&MaterialUI::ShaderSelect);
        pListUI->Activate();
    }

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::Text("Material Parameter");
    ImGui::Spacing(); ImGui::Spacing(); 

    // Shader Parameter
    if (nullptr == pShader)
        return;

    const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].Type)
        {
        case SCALAR_PARAM::BOOL_0:
        case SCALAR_PARAM::BOOL_1:
        case SCALAR_PARAM::BOOL_2:
        case SCALAR_PARAM::BOOL_3:
            ParamUI::Param_BOOL((bool*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::INT_0:
        case SCALAR_PARAM::INT_1:
        case SCALAR_PARAM::INT_2:
        case SCALAR_PARAM::INT_3:                   
            ParamUI::Param_INT((int*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, (int)vecScalarParam[i].min, (int)vecScalarParam[i].Max,vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::FLOAT_0:
        case SCALAR_PARAM::FLOAT_1:
        case SCALAR_PARAM::FLOAT_2:
        case SCALAR_PARAM::FLOAT_3:
            ParamUI::Param_FLOAT((float*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].min, vecScalarParam[i].Max, vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::VEC2_0:
        case SCALAR_PARAM::VEC2_1:
        case SCALAR_PARAM::VEC2_2:
        case SCALAR_PARAM::VEC2_3:
            ParamUI::Param_VEC2((Vec2*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].min, vecScalarParam[i].Max,  vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::VEC4_0:
        case SCALAR_PARAM::VEC4_1:
        case SCALAR_PARAM::VEC4_2:
        case SCALAR_PARAM::VEC4_3:
            ParamUI::Param_VEC4((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].min, vecScalarParam[i].Max,  vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::MAT_0:
        case SCALAR_PARAM::MAT_1:
        case SCALAR_PARAM::MAT_2:
        case SCALAR_PARAM::MAT_3:
            break;
        }        
    }

    const vector<tTexParam>& vecTexParam = pShader->GetTexParam();
    for (size_t i = 0; i < vecTexParam.size(); ++i)
    {
        Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTexParam[i].Type);      
        if (ParamUI::Param_TEXTURE(pTex, vecTexParam[i].Desc, this, (Delegate_1)&MaterialUI::SelectTexture))
        {           
            // 리스트 버튼을 눌렀다면
            m_SelectTexParam = vecTexParam[i].Type;
        }
        pMtrl->SetTexParam(vecTexParam[i].Type, pTex);
    }
}

void MaterialUI::SelectTexture(DWORD_PTR _dwData)
{
    string strTex = (char*)_dwData;
    wstring strTexName = ToWString(strTex);

    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexName);
    Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
    pMtrl->SetTexParam(m_SelectTexParam, pTex);    
}

void MaterialUI::CreateAssetInstance(Ptr<CAsset> _Asset)
{
    auto pOriginMtrl = dynamic_cast<CMaterial*>(_Asset.Get());

    wstring strKey = _Asset->GetKey();
    strKey.erase(strKey.length() - 5, 5);

    wchar_t szPath[255] = {};
    wstring FilePath = CPathMgr::GetContentPath();

    int num = 0;
    while (true)
    {
        swprintf_s(szPath, (strKey + L"_Inst_%d.mtrl").c_str(), num);
        if (!exists(FilePath + szPath))
            break;

        ++num;
    }

    CMaterial* pNewMtrl = pOriginMtrl->Clone();
    pNewMtrl->SetName(szPath);
    pNewMtrl->Save(szPath);
    GamePlayStatic::AddAsset(pNewMtrl);
}


void MaterialUI::ShaderSelect(DWORD_PTR _ptr)
{
    string strShader = (char*)_ptr;
    wstring strShaderName = ToWString(strShader);

    Ptr<CGraphicsShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(strShaderName);

    Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
    pMtrl->SetShader(pShader);
}
