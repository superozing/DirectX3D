#ifndef _Blur
#define _Blur

#include "value.fx"

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

VS_OUT VS_Blur(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Blur(VS_IN _in) : SV_Target
{
    //float4 vOutColor = 1.f;
    
    //float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    vOutColor.a = 1.f;
    
    return vOutColor;
}

#endif