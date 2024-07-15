#ifndef _DIALOGFONTBG
#define _DIALOGFONTBG

#include "value.fx"
#include "func.fx"

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

VS_OUT VS_DialogFontBg(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_DialogFontBg(VS_OUT _in) : SV_Target
{
    // 출력 색상
    float4 vOutColor = float4(0.f, 0.f, 0.2f, 1.f);
    
    float alpha = 0.5 * (1.0 + cos((1.0 - _in.vUV.y) * PI));
    
    vOutColor *= alpha;
    
    if (vOutColor.a < 0.001f)
        discard;
    
    return vOutColor;
}


#endif