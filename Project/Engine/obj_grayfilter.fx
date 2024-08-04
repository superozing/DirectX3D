#ifndef _OBJ_GRAYFILTER
#define _OBJ_GRAYFILTER

#include "value.fx"

#define bWeight g_bool_0
#define texPreProcess g_tex_0
#define texDepthStencil g_tex_1

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

#define GrayMask (1<<1)
float4 PS_ObjGrayFilter(VS_OUT _in) : SV_Target
{
    float4 vOutColor = texPreProcess.Sample(g_sam_0, _in.vUV);
    uint stancil = texDepthStencil.Sample(g_sam_0, _in.vUV).r;
    bool bCheck = 0 != (GrayMask & stancil);
    
    if (true == bCheck)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    }
        
    return vOutColor;
}

#endif
