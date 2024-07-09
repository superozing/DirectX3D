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
    float CurUVYRatio = 1 - RATIO; 
    
    if (_in.vUV.y < CurUVYRatio)
        discard;
    
    if (CurUVYRatio < 0.5)
        return float4(0.f, 1.f, 0.f, 1.f);
    else if (CurUVYRatio < 0.8)
        return float4(1.f, 0.5f, 0.f, 1.f);
    else
        return float4(1.f, 0.f, 0.f, 1.f);
    
}

#endif