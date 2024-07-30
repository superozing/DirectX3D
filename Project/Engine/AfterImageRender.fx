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
    
    // Per Instance Data    
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint iRowIndex : ROWINDEX;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

VS_OUT VS_AfterImageRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal
              , _in.vWeights, _in.vIndices, 0);
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;

};

PS_OUT PS_AfterImageRender(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    float3 vViewNormal = (float3) 0.f;
    
    if (ColorTextureCheck)
    {
        vOutColor = AfterImageTexture.Sample(g_sam_0, _in.vUV);
    }
    
    output.vColor = vOutColor;
    
    return output;
}

#endif