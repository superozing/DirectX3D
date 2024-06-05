#include "pch.h"
#include "MaterialUI.h"

#include <Engine\CKeyMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>

#include "CImGuiMgr.h"
#include "ParamUI.h"
#include "ListUI.h"

MaterialUI::MaterialUI()
	: AssetUI("Material", "##Material", ASSET_TYPE::MATERIAL)
    , m_TargetMtrl(nullptr)
    , m_SelectTexParam(TEX_PARAM::END)
{
}

MaterialUI::~MaterialUI()
{
}


void MaterialUI::render_update()
{
	AssetUI::render_update();

    // 해당 텍스쳐 이미지 출력
    m_TargetMtrl = (CMaterial*)GetAsset().Get();
    
    char MtrlKey[255];
    string strKey = string(m_TargetMtrl->GetKey().begin(), m_TargetMtrl->GetKey().end());
    string prevKey = strKey;
    
    string FullPath = ToString(CPathMgr::GetContentPath()) + strKey;
    if (exists(FullPath))
    {
        filesystem::path pathObj(FullPath);

        strcpy(MtrlKey, pathObj.stem().string().c_str());
    }

    string strPath = string(m_TargetMtrl->GetKey().begin(), m_TargetMtrl->GetKey().end());
    
    ImGui::Text("Material");
    ImGui::SameLine();
    ImGui::InputText("##MtrlName", MtrlKey, 255);

    string NewName = "material\\" + string(MtrlKey) + ".mtrl";
    if (prevKey != NewName)
    {
        if (KEY_TAP_EDITOR(ENTER))
        {
            ChangeAssetName(strKey, NewName);
            AssetUI::SetAssetKey((CAsset*)m_TargetMtrl.Get(), ToWString(NewName));
            prevKey = NewName;
        }
        else if (KEY_TAP_EDITOR(ESC))
        {
        }
    }


    Ptr<CGraphicsShader> pShader = m_TargetMtrl->GetShader();
    string strShaderName;
    if (nullptr != pShader)
    {
        strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
    }
    else
    {
        m_TargetMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(SHADER_deferred));
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

    const tScalarParam* vecScalarParam = pShader->GetScalarParam();
    for (size_t i = 0; i < (UINT)SCALAR_PARAM::END; ++i)
    {
        if (!vecScalarParam[i].IsUse)
            continue;

        switch (vecScalarParam[i].Type)
        {
        case SCALAR_PARAM::BOOL_0:
        case SCALAR_PARAM::BOOL_1:
        case SCALAR_PARAM::BOOL_2:
        case SCALAR_PARAM::BOOL_3:
            ParamUI::Param_BOOL((bool*)m_TargetMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::INT_0:
        case SCALAR_PARAM::INT_1:
        case SCALAR_PARAM::INT_2:
        case SCALAR_PARAM::INT_3:                   
            ParamUI::Param_INT((int*)m_TargetMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, (int)vecScalarParam[i].Min, (int)vecScalarParam[i].Max,vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::FLOAT_0:
        case SCALAR_PARAM::FLOAT_1:
        case SCALAR_PARAM::FLOAT_2:
        case SCALAR_PARAM::FLOAT_3:
            ParamUI::Param_FLOAT((float*)m_TargetMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].Min, vecScalarParam[i].Max, vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::VEC2_0:
        case SCALAR_PARAM::VEC2_1:
        case SCALAR_PARAM::VEC2_2:
        case SCALAR_PARAM::VEC2_3:
            ParamUI::Param_VEC2((Vec2*)m_TargetMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].Min, vecScalarParam[i].Max,  vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::VEC4_0:
        case SCALAR_PARAM::VEC4_1:
        case SCALAR_PARAM::VEC4_2:
        case SCALAR_PARAM::VEC4_3:
            ParamUI::Param_VEC4((Vec4*)m_TargetMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc, vecScalarParam[i].Min, vecScalarParam[i].Max,  vecScalarParam[i].View, vecScalarParam[i].Tooltip);
            break;
        case SCALAR_PARAM::MAT_0:
        case SCALAR_PARAM::MAT_1:
        case SCALAR_PARAM::MAT_2:
        case SCALAR_PARAM::MAT_3:
            break;
        }        
    }

    const tTexParam* vecTexParam = pShader->GetTexParam();
    for (size_t i = 0; i < (size_t)TEX_PARAM::END; ++i)
    {
        if (!vecTexParam[i].IsUse)
            continue;

        Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(vecTexParam[i].Type);
        if (ParamUI::Param_TEXTURE(pTex, vecTexParam[i].Desc, this, (Delegate_1)&MaterialUI::SelectTexture))
        {           
            // 리스트 버튼을 눌렀다면
            m_SelectTexParam = vecTexParam[i].Type;
        }
        m_TargetMtrl->SetTexParam(vecTexParam[i].Type, pTex);
    }
}

void MaterialUI::SelectTexture(DWORD_PTR _dwData)
{
    string strTex = (char*)_dwData;
    wstring strTexName = ToWString(strTex);

    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexName);
    m_TargetMtrl = (CMaterial*)GetAsset().Get();
    m_TargetMtrl->SetTexParam(m_SelectTexParam, pTex);
}

void MaterialUI::ShaderSelect(DWORD_PTR _ptr)
{
    string strShader = (char*)_ptr;
    wstring strShaderName = ToWString(strShader);

    Ptr<CGraphicsShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(strShaderName);

    m_TargetMtrl = (CMaterial*)GetAsset().Get();
    m_TargetMtrl->SetShader(pShader);
}
