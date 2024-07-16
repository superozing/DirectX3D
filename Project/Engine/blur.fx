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
    static const float weight[5] = { 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };
    
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    float Width = g_RenderResolution.x;
    float Height = g_RenderResolution.y;
    float2 texelSize = float2(1.0f / Width, 1.0f / Height);
    
    //Blur Logic
    if (true == g_bool_0)
    {
        for (int i = -2; i <= 2; ++i)
        {
            vOutColor += saturate(weight[abs(i)] * g_tex_0.Sample(g_sam_0, _in.vUV + float2(i, 0.f) * texelSize));
        }
    }
    else
    {
        for (int i = -2; i <= 2; ++i)
        {
            vOutColor += saturate(weight[abs(i)] * g_tex_0.Sample(g_sam_0, _in.vUV + float2(0.f, i) * texelSize));
        }
    }
    
    vOutColor.a = 1.f;
    return vOutColor;
}

#endif