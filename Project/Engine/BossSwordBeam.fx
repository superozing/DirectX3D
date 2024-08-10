#ifndef _BOSSSWORDBEAM
#define _BOSSSWORDBEAM

#include "value.fx"

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

// =============
// BossSwordBeam
// =============
VS_OUT VS_BossSwordBeam(VS_IN _in)
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

float4 PS_BossSwordBeam(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float4 TexColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // 검기 색상 (푸른 계열)
    float3 BeamColor = float3(0, 0.6, 1);
    vColor.rgb = TexColor.rgb * BeamColor * TexColor.a;
    
    return vColor;
}


// ================
// BossSwordBeamSub
// ================
VS_OUT VS_BossSwordBeamSub(VS_IN _in)
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

float4 PS_BossSwordBeamSub(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float4 TexColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // 검기 색상 (푸른 계열)
    float3 BeamColor = float3(0, 0.6, 1);
    vColor.rgb = TexColor.rgb * BeamColor * TexColor.a;
    
    return vColor;
}


#endif