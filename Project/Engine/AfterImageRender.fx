#ifndef AfterImageRender
#define AfterImageRender

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// ======================
// AfterImageRender Shader
// MRT : AfterImage MRT
#define AfterImageTexture        g_tex_0

#define ColorTextureCheck   g_btex_0
// ======================

StructuredBuffer<Matrix> g_AfterImageBoneMat : register(t40);
StructuredBuffer<Matrix> g_AfterImageBoneMat2 : register(t41);
StructuredBuffer<Matrix> g_AfterImageBoneMat3 : register(t42);
StructuredBuffer<Matrix> g_AfterImageBoneMat4 : register(t43);
StructuredBuffer<Matrix> g_AfterImageBoneMat5 : register(t44);
StructuredBuffer<Matrix> g_AfterImageBoneMat6 : register(t45);
StructuredBuffer<Matrix> g_AfterImageBoneMat7 : register(t46);
StructuredBuffer<Matrix> g_AfterImageBoneMat8 : register(t47);
StructuredBuffer<Matrix> g_AfterImageBoneMat9 : register(t48);
StructuredBuffer<Matrix> g_AfterImageBoneMat10 : register(t49);

matrix GetInstanceBoneMat(int iBoneIdx, int iInstanceID)
{
    int reversedInstanceID = 9 - iInstanceID;
    
    switch (reversedInstanceID)
    {
        case 0:
            return g_AfterImageBoneMat[iBoneIdx];
        case 1:
            return g_AfterImageBoneMat2[iBoneIdx];
        case 2:
            return g_AfterImageBoneMat3[iBoneIdx];
        case 3:
            return g_AfterImageBoneMat4[iBoneIdx];
        case 4:
            return g_AfterImageBoneMat5[iBoneIdx];
        case 5:
            return g_AfterImageBoneMat6[iBoneIdx];
        case 6:
            return g_AfterImageBoneMat7[iBoneIdx];
        case 7:
            return g_AfterImageBoneMat8[iBoneIdx];
        case 8:
            return g_AfterImageBoneMat9[iBoneIdx];
        case 9:
            return g_AfterImageBoneMat10[iBoneIdx];
        default:
            return g_AfterImageBoneMat[iBoneIdx];
    }
}

void AfterImageSkinning(inout float3 vPos, inout float3 vTangent, inout float3 vBinormal, inout float3 vNormal,
                        float4 vWeight, uint4 vIndices, int iInstanceID)
{
    AfterImageInfo info = g_AfterImage[0];
    int iBoneCount = info.iBoneCount;
    
    float3 skinned_pos = float3(0.0f, 0.0f, 0.0f);
    float3 skinned_tangent = float3(0.0f, 0.0f, 0.0f);
    float3 skinned_binormal = float3(0.0f, 0.0f, 0.0f);
    float3 skinned_normal = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        if (vWeight[i] > 0.0f)
        {
            int boneIndex = iInstanceID * iBoneCount + vIndices[i];
            matrix bone_matrix = g_arrBoneMat[boneIndex];
            
            skinned_pos += mul(float4(vPos, 1.0f), bone_matrix).xyz * vWeight[i];
            skinned_tangent += mul(vTangent, (float3x3) bone_matrix) * vWeight[i];
            skinned_binormal += mul(vBinormal, (float3x3) bone_matrix) * vWeight[i];
            skinned_normal += mul(vNormal, (float3x3) bone_matrix) * vWeight[i];
        }
    }
    
    vPos = skinned_pos;
    vTangent = normalize(skinned_tangent);
    vBinormal = normalize(skinned_binormal);
    vNormal = normalize(skinned_normal);
}


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
    
    uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
    float fAlpha : TEXCOORD1;
    
    uint instanceID : SV_InstanceID;
};

VS_OUT VS_AfterImageRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    AfterImageInfo data = g_AfterImage[0];
    
    // 스키닝 적용
    float3 skinnedPos = _in.vPos;
    float3 skinnedTangent = _in.vTangent;
    float3 skinnedBinormal = _in.vBinormal;
    float3 skinnedNormal = _in.vNormal;
    
    if (g_iAnim)
    {
        AfterImageSkinning(skinnedPos, skinnedTangent, skinnedBinormal, skinnedNormal,
                           _in.vWeights, _in.vIndices, _in.instanceID);
    }
    
    // 인스턴스별 월드 변환 행렬 적용
    float4x4 worldMat = data.AfterImageWorldMat[_in.instanceID];
    float4 worldPos = mul(float4(skinnedPos, 1.f), worldMat);
    
    // 뷰 및 투영 변환
    output.vPosition = mul(mul(worldPos, g_matView), g_matProj);
    output.vUV = _in.vUV;
    
    // 뷰 공간에서의 위치 및 법선 계산
    output.vViewPos = mul(worldPos, g_matView).xyz;
    output.vViewNormal = normalize(mul(float4(skinnedNormal, 0.f), mul(worldMat, g_matView)).xyz);
    output.vViewTangent = normalize(mul(float4(skinnedTangent, 0.f), mul(worldMat, g_matView)).xyz);
    output.vViewBinormal = normalize(mul(float4(skinnedBinormal, 0.f), mul(worldMat, g_matView)).xyz);
    
    // 알파값 계산
    output.fAlpha = data.fLifeTIme[_in.instanceID] / data.fMaxLifeTime;
    
    output.instanceID = _in.instanceID;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

PS_OUT PS_AfterImageRender(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    AfterImageInfo data = g_AfterImage[0];
    
    if (data.bRenderFlags[_in.instanceID] == 0)
    {
        discard;
    }
    
    float4 vOutColor = float4(1.f, 0.f, 1.f, _in.fAlpha);
    
    if (data.iColorMode == 0)
    {
        if (ColorTextureCheck)
        {
            vOutColor = AfterImageTexture.Sample(g_sam_0, _in.vUV);
            vOutColor.a *= _in.fAlpha; // 알파값 적용
        }
    }
    else if (data.iColorMode == 1)
    {
        vOutColor = data.AfterImageColor * (data.fLifeTIme[_in.instanceID] / data.fMaxLifeTime);

    }

    
    output.vColor = vOutColor;
    return output;
}

#endif