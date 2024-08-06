#ifndef AfterImageRender
#define AfterImageRender

#include "value.fx"
#include "struct.fx"
#include "func.fx"

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

// ======================
// AfterImageRender Shader
// MRT : AfterImage MRT
#define AfterImageTexture        g_tex_0

#define ColorTextureCheck   g_btex_0
// ======================

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
};

VS_OUT VS_AfterImageRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    AfterImageInfo data = g_AfterImage[0];
    
    float4x4 AnimationBoneInfo;
    
    if (_in.instanceID == 0)
        AnimationBoneInfo = g_AfterImageBoneMat[0];
    else if (_in.instanceID == 1)
        AnimationBoneInfo = g_AfterImageBoneMat2[0];
    else if (_in.instanceID == 2)
        AnimationBoneInfo = g_AfterImageBoneMat3[0];
    else if (_in.instanceID == 3)
        AnimationBoneInfo = g_AfterImageBoneMat4[0];
    else if (_in.instanceID == 4)
        AnimationBoneInfo = g_AfterImageBoneMat5[0];
    else if (_in.instanceID == 5)
        AnimationBoneInfo = g_AfterImageBoneMat6[0];
    else if (_in.instanceID == 6)
        AnimationBoneInfo = g_AfterImageBoneMat7[0];
    else if (_in.instanceID == 7)
        AnimationBoneInfo = g_AfterImageBoneMat8[0];
    else if (_in.instanceID == 8)
        AnimationBoneInfo = g_AfterImageBoneMat9[0];
    else if (_in.instanceID == 9)
        AnimationBoneInfo = g_AfterImageBoneMat10[0];
    
     // 애니메이션 행렬 적용
    float4 animatedPos = mul(float4(_in.vPos, 1.f), AnimationBoneInfo);
    float3 animatedNormal = mul(float4(_in.vNormal, 0.f), AnimationBoneInfo).xyz;
    float3 animatedTangent = mul(float4(_in.vTangent, 0.f), AnimationBoneInfo).xyz;
    float3 animatedBinormal = mul(float4(_in.vBinormal, 0.f), AnimationBoneInfo).xyz;
    
    // 인스턴스별 월드 변환 행렬 적용
    float4x4 worldMat = data.AfterImageWorldMat[_in.instanceID];
    float4 worldPos = mul(float4(_in.vPos, 1.f), worldMat);
    
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal
              , _in.vWeights, _in.vIndices, 0);
    }
    
    // 뷰 및 투영 변환
    output.vPosition = mul(mul(worldPos, g_matView), g_matProj);
    output.vUV = _in.vUV;
    
    // 뷰 공간에서의 위치 및 법선 계산
    output.vViewPos = mul(worldPos, g_matView).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), mul(worldMat, g_matView)).xyz);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), mul(worldMat, g_matView)).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), mul(worldMat, g_matView)).xyz);
    
    // 디버깅: 인스턴스 ID에 따라 색상 변경
    float3 debugColor = float3(1.0, 0.0, 0.0) * (_in.instanceID / float(data.NodeCount));
    output.vViewNormal = debugColor;

    // 알파값 조정
    output.fAlpha = 0.5 - (_in.instanceID / float(data.NodeCount)) * 0.5;
    
    // 시간에 따른 알파값 계산 (예시)
    //output.fAlpha = 1.0 - (_in.instanceID / float(data.NodeCount));
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

PS_OUT PS_AfterImageRender(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    //float4 vOutColor = float4(1.f, 0.f, 1.f, _in.fAlpha);
    
    //if (ColorTextureCheck)
    //{
    //    vOutColor = AfterImageTexture.Sample(g_sam_0, _in.vUV);
    //    vOutColor.a *= _in.fAlpha; // 알파값 적용
    //}
    
    //output.vColor = vOutColor;
    //return output;
    
    
    output.vColor = float4(_in.vViewNormal, _in.fAlpha);
    return output;
}

#endif