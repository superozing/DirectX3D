#ifndef _STATICUI
#define _STATICUI

#include "value.fx"
#include "func.fx"

#define LeftTop g_vec2_0
#define Size g_vec2_1
#define Ratio g_vec2_2

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
    float4 vOutColor = (float4) (1.f, 0.f, 1.f, 1.f);
    
    if (AtlasCheck)
    {
        float2 uv = float2(LeftTop.x + Size.x * 0.5f, LeftTop.y + _in.vUV.y * Size.y);

        // 0 ~ 0.5f * Ratio
        if (_in.vUV.x <= 0.5f * Ratio.x)
        {
            uv = _in.vUV * Size / Ratio;
            uv += LeftTop;
        }
        
        // (1-0.5f * Ratio) ~ 1
        if (_in.vUV.x >= 1 - 0.5f * Ratio.x)
        {
            uv = (_in.vUV) * Size / Ratio;
            uv.x = (_in.vUV.x - (1 - 0.5f * Ratio.x)) * Size.x / Ratio.x;
            uv += LeftTop;
            uv.x += Size.x * 0.5f;
        }
        
        vOutColor = Atlas.Sample(g_sam_0, uv);
    }
    
    return vOutColor;
}

#endif