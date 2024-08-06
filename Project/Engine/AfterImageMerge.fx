#ifndef AfterImageMerge
#define AfterImageMerge

#include "value.fx"
#include "struct.fx"
#include "func.fx"


#define AfterImageOriginTexture        g_tex_0
#define AfterImageMergeTexture         g_tex_1


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

VS_OUT VS_MergeAfterImage(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_MergeAfterImage(VS_OUT _in) : SV_Target
{
    float4 OutputColor = float4(0.f, 0.f, 0.f, 1.f);

    float4 vAfterImageColor = float4(0.f, 0.f, 0.f, 1.f);
    
    vAfterImageColor = AfterImageMergeTexture.Sample(g_sam_0, _in.vUV);
    
    if (vAfterImageColor.a <= 0.f)
        discard;
    
    OutputColor = vAfterImageColor;
    
    return OutputColor;
}

#endif