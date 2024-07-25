#ifndef _STATICUI
#define _STATICUI

#include "value.fx"
#include "func.fx"

#define LeftTop g_vec2_0
#define Size g_vec2_1

#define AtlasCheck g_btex_0
#define Atlas g_tex_0

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

VS_OUT VS_StaticUI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_StaticUI(VS_OUT _in) : SV_Target
{
    // 출력 색상
    float4 vOutColor = (float4) 0.f;
    
    if (AtlasCheck)
    {
        float2 uv = _in.vUV * Size;
        uv += LeftTop;
        vOutColor = Atlas.Sample(g_sam_0, uv);
    }
    
    return vOutColor;
}

#endif