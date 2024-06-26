#ifndef _ImageWrap
#define _ImageWrap

#include "value.fx"
#include "func.fx"

// ======================
// Std3D_Deferred Shader
// MRT : Deferred MRT
#define ColorTexture        g_tex_0
#define NormalMap           g_tex_1
#define SpecularMap         g_tex_2
#define EmissiveMap         g_tex_3

#define ColorTextureCheck   g_btex_0
#define NormalMapCheck      g_btex_1

#define WrapCount           g_int_0
#define SizeRatio           g_float_0
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
    
   // float4 vViewPos : POSITION;
   // float4 vViewTangent : TANGENT;
   // float4 vViewNormal : NORMAL;
   // float4 vViewBinormal : BINORMAL;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

VS_OUT VS_ImageWrap(VS_IN _in)
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

VS_OUT VS_ImageWrap_Inst(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal
              , _in.vWeights, _in.vIndices, _in.iRowIndex);
    }
        
    output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), _in.matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), _in.matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), _in.matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), _in.matWV));
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vEmissive : SV_Target3;
};

PS_OUT PS_ImageWrap(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    float3 vViewNormal = (float3) 0.f;
    
    if (ColorTextureCheck)
    {
        vOutColor = ColorTexture.Sample(g_sam_0, (_in.vUV / WrapCount) * SizeRatio);
    }
    
    output.vColor = vOutColor;
    output.vPosition = float4(_in.vViewPos, 1.f);
    
    vViewNormal = _in.vViewNormal;
    
    if (NormalMapCheck)
    {
        float3 vNormal = NormalMap.Sample(g_sam_0, _in.vUV).rgb;
        vNormal = vNormal * 2.f - 1.f;
        
        float3x3 matRot =
        {
            _in.vViewTangent,
              _in.vViewBinormal,
              _in.vViewNormal,
        };
        
        vViewNormal = normalize(mul(vNormal, matRot));

    }
    
    output.vNormal = float4(vViewNormal, 1.f);
    output.vEmissive = float4(0.f, 0.f, 0.f, 1.f);
    
    return output;
}
#endif
