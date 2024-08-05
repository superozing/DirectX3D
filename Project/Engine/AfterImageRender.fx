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
    
    // 인스턴스별 월드 변환 행렬 적용
    float4x4 worldMat = data.AfterImageWorldMat[_in.instanceID];
    float4 worldPos = mul(float4(_in.vPos, 1.f), worldMat);
    
    // 뷰 및 투영 변환
    output.vPosition = mul(mul(worldPos, g_matView), g_matProj);
    output.vUV = _in.vUV;
    
    // 뷰 공간에서의 위치 및 법선 계산
    output.vViewPos = mul(worldPos, g_matView).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), mul(worldMat, g_matView)).xyz);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), mul(worldMat, g_matView)).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), mul(worldMat, g_matView)).xyz);
    
    // 시간에 따른 알파값 계산 (예시)
    output.fAlpha = 1.0 - (_in.instanceID / float(data.NodeCount));
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

PS_OUT PS_AfterImageRender(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float4 vOutColor = float4(1.f, 0.f, 1.f, _in.fAlpha);
    
    if (ColorTextureCheck)
    {
        vOutColor = AfterImageTexture.Sample(g_sam_0, _in.vUV);
        vOutColor.a *= _in.fAlpha; // 알파값 적용
    }
    
    output.vColor = vOutColor;
    return output;
}

#endif