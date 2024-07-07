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
    const float mask[25] =
    {
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, 24, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1
    }; // Larger Laplacian Filter
    const float coord[5] = { -2, -1, 0, 1, 2 };
    const float divider = 1;
    float MAP_CX = g_RenderResolution.x;
    float MAP_CY = g_RenderResolution.y;
    const float3 grayScale = float3(0.3, 0.59, 0.11);
    const float threshold = 0.1f; // �ܰ��� ���� �Ӱ谪
    
    float4 vOutColor = (float4) 0.f;
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    //Edge Detection START
    if (1 == g_int_0 && 0.f < g_float_0)
    {
        float4 vColorBuff = 0;
        for (int i = 0; i < 25; i++)
        {
            vColorBuff += mask[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord[i % 5] / MAP_CX, coord[i / 5] / MAP_CY)));
        }
        float gray = 1 - dot(vColorBuff.xyz, grayScale);
        float4 OutLine = float4(gray, gray, gray, 1) / divider;
        // �ܰ��� ���� �Ӱ谪�� �������� ������ ����
        vColor = (gray > g_float_0) ? vColor : OutLine;
        
        // ��Ƽ�ٸ���� ����
        float4 antiAliasedColor = 0;
        float kernel[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                antiAliasedColor += kernel[(i + 1) * 3 + (j + 1)] * g_tex_0.Sample(g_sam_0, _in.vUV + float2(i / MAP_CX, j / MAP_CY));
            }
        }
        antiAliasedColor /= 16;
        vColor = lerp(vColor, antiAliasedColor, 0.85); // ���� ���� ����
        //Edge Detection E N D
    }
    float4 vDiffuse = g_tex_1.Sample(g_sam_0, _in.vUV);
    float4 Specular = g_tex_2.Sample(g_sam_0, _in.vUV);
    float4 Emissive = g_tex_3.Sample(g_sam_0, _in.vUV);
        
    vOutColor = (vColor * (vDiffuse + Emissive)) + Specular;
    vOutColor.a = 1.f;
    
    return vOutColor;
}





#endif