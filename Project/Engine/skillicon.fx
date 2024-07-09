#ifndef _SKILLICON
#define _SKILLICON

#include "value.fx"
#include "func.fx"

#define RATIO   g_float_0

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_SkillIcon(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_SkillIcon(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_btex_0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (1 - RATIO < _in.vUV.y)
        vColor *= 0.6f;
    
    return vColor;
}

#endif