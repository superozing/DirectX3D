#ifndef _OBJ_GRAYFILTER
#define _OBJ_GRAYFILTER

#include "value.fx"

#define bWeight g_bool_0
#define texPreProcess g_tex_0

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

VS_OUT VS_ObjGrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_ObjGrayFilter(VS_OUT _in) : SV_Target
{
    float4 vOutColor = texPreProcess.Sample(g_sam_0, _in.vUV);
    
    if (true == bWeight)
    {
        
    }
    else
    {
        float avg = (vOutColor.r + vOutColor.g + vOutColor.b) / 3.f;
        return float4(avg, avg, avg, 1.f);
    }

    return vOutColor;
}

#endif
