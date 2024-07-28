#ifndef _CROMATICABERRATION
#define _CROMATICABERRATION

#include "value.fx"

#define Activate g_bool_0
#define RedOffset g_vec2_0
#define GreenOffset g_vec2_1
#define BlueOffset g_vec2_2

// mesh : RectMesh
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

// ==========
// GrayFilter
// ==========
VS_OUT VS_CromaticAberration(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_CromaticAberration(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    float Width = g_RenderResolution.x;
    float Height = g_RenderResolution.y;
    float2 texelSize = float2(1.0f / Width, 1.0f / Height);
        
    float red = g_tex_0.Sample(g_sam_0, _in.vUV + g_vec2_0 * texelSize).r;
    float green = g_tex_0.Sample(g_sam_0, _in.vUV + g_vec2_1 * texelSize).g;
    float blue = g_tex_0.Sample(g_sam_0, _in.vUV + g_vec2_2 * texelSize).b;
    
        
    vColor.rgb = float3(red, green, blue);
    vColor.a = 1.f;
    return vColor;
}

#endif