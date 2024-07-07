#ifndef _MERGE
#define _MERGE

#include "value.fx"


// ===============
// Merge Shader
// MRT : SwapChain
// Mesh : RectMesh
// g_tex_0 : ColorTargetTex
// g_tex_1 : DiffuseTargetTex
// g_tex_2 : SpecularTargetTex
// g_tex_3 : EmissiveTargetTex
// g_tex_4 : NormalTargetTex
// ===============

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

VS_OUT VS_Merge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Merge(VS_OUT _in) : SV_Target
{
    const float mask[9] =
    {
        -1, -1, -1,
  -1, 8, -1,
  -1, -1, -1
    }; // Laplacian Filter
    const float coord[3] = { -1, 0, +1 };
    const float divider = 1;
    float MAP_CX = g_RenderResolution.x;
    float MAP_CY = g_RenderResolution.y;
    const float3 grayScale = float3(0.3, 0.59, 0.11);
    const float threshold = 0.1f; // 외곽선 검출 임계값
    
    float4 vOutColor = (float4) 0.f;
    
    //Edge Detection START
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vColorBuff = 0;
    float4 Ret = 0;
    for (int i = 0; i < 9; i++)
    {
        //float2 AddUv;
        //AddUv.x = coord[i % 3] / MAP_CX;
        //AddUv.y = coord[i / 3] / MAP_CY;
        //float2 NewUv = _in.vUV + AddUv;
        //vColorBuff += mask[i] * (g_tex_4.Sample(g_sam_0, NewUv));
        vColorBuff += mask[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord[i % 3] / MAP_CX, coord[i / 3] / MAP_CY)));
    }

    float gray = 1 - dot(vColorBuff.xyz, grayScale);
    Ret = float4(gray, gray, gray, 1) / divider;
    // 외곽선 검출 임계값을 기준으로 색상을 결정
    Ret = (gray > threshold) ? vColor : Ret;
    //Edge Detection E N D
    
    float4 vDiffuse = g_tex_1.Sample(g_sam_0, _in.vUV);
    float4 Specular = g_tex_2.Sample(g_sam_0, _in.vUV);
    float4 Emissive = g_tex_3.Sample(g_sam_0, _in.vUV);
        
    vOutColor = (Ret * (vDiffuse + Emissive)) + Specular;
    vOutColor.a = 1.f;
    
    return vOutColor;
}





#endif