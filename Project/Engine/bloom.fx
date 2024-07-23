#ifndef _Bloom
#define _Bloom

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

VS_OUT VS_Bloom(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Bloom(VS_OUT _in) : SV_Target
{
    //float BloomRatio = 2.f;
    float BloomRatio = g_float_0;
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    vOutColor = saturate(g_tex_0.Sample(g_sam_0, _in.vUV) + BloomRatio * g_tex_1.Sample(g_sam_0, _in.vUV));
    
    vOutColor.a = 1.f;
    return vOutColor;
}

#endif