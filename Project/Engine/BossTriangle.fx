#ifndef _BOSSTRIANGLE
#define _BOSSTRIANGLE

#include "value.fx"

#define ColorTexture        g_tex_0
#define NormalMap           g_tex_1
#define SpecularMap         g_tex_2
#define EmissiveMap         g_tex_3

#define ColorTextureCheck   g_btex_0
#define NormalMapCheck      g_btex_1
#define EmissiveTextureCheck      g_btex_3
#define BloomColor          g_vec4_0
#define ShieldTime          g_float_0
#define BloomTime          g_float_1

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    // Per Instance Data    
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
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


// ============
// BossTriangle
// ============
VS_OUT VS_BossTriangle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.0), g_matWVP);
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
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vEmissive : SV_Target3;
    float4 vRelativeLuminance : SV_Target4;
};

PS_OUT PS_BossTriangle(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    if (_in.vUV.y > ShieldTime)
        discard;
    
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    float3 vViewNormal = (float3) 0.f;
    
    if (ColorTextureCheck)
    {
        vOutColor = ColorTexture.Sample(g_sam_0, _in.vUV);
    }
    
    output.vColor = vOutColor;
    output.vPosition = float4(_in.vViewPos, 1.f);
    
    vViewNormal = _in.vViewNormal;
    
    //Bloom START
    float3 vRLWeight = float3(0.2126f, 0.7152f, 0.0722f);
    float3 startBloomColor = float3(1.0f, 1.0f, 1.0f); // 하얀색
    float3 endBloomColor = float3(1.0f, 1.0f, 0.0f); // 노란색
    
    // 블룸 색상 보간
    float3 bloomColor = lerp(startBloomColor, endBloomColor, saturate(BloomTime));
    float4 vBloomColor = float4(bloomColor, 1.0f);
    float fThreshold = 0.8f;

    float brightness = dot(vOutColor.rgb, vRLWeight);
    if (brightness > fThreshold)
    {
        output.vRelativeLuminance = vBloomColor;

    }
    else
    {
        output.vRelativeLuminance = float4(0.f, 0.f, 0.f, 1.f);
    }
    //Bloom E N D
    
    output.vNormal = float4(vViewNormal, 1.f);
    
    if (EmissiveTextureCheck)
    {
        output.vEmissive = EmissiveMap.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        output.vEmissive = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    return output;
}
#endif