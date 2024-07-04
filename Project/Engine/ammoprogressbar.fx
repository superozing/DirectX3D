#ifndef _AMMOPROGRESSBAR
#define _AMMOPROGRESSBAR

#include "value.fx"
#include "func.fx"

#define RATIO g_float_0

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

VS_OUT VS_AmmoProgressBar(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_AmmoProgressBar(VS_OUT _in) : SV_Target
{
    if (1 - RATIO > _in.vUV.y || !g_btex_0)
        discard;
    
    float4 vColor = (float4) 0.f;
    
    if (RATIO < 0.5)
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    else if (RATIO < 0.8)
        vColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    else
        vColor = g_tex_2.Sample(g_sam_0, _in.vUV);
    
    if (vColor.a < 0.1f)
        discard;

    return vColor;
}

#endif