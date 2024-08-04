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
    const float Laplacian3[9] =
    {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    }; // Laplacian Filter 3x3
    
    const float Laplacian5[25] =
    {
        0, 0, -1, 0, 0,
        0, -1, -2, -1, 0,
        -1, -2, 16, -2, -1,
        0, -1, -2, -1, 0,
        0, 0, -1, 0, 0
    }; // Laplacian Filter 5x5
    
    const float sobelX3[9] =
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    }; // Sobel Filter X 3x3
    
    const float sobelY3[9] =
    {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    }; // Sobel Filter Y 3x3

    const float sobelX5[25] =
    {
        -2, -1, 0, 1, 2,
        -2, -1, 0, 1, 2,
        -4, -2, 0, 2, 4,
        -2, -1, 0, 1, 2,
        -2, -1, 0, 1, 2
    }; // Sobel Filter X 5x5
    
    const float sobelY5[25] =
    {
        -2, -2, -4, -2, -2,
        -1, -1, -2, -1, -1,
        0, 0, 0, 0, 0,
        1, 1, 2, 1, 1,
        2, 2, 4, 2, 2
    }; // Sobel Filter Y 5x5

    const float coord3[3] = { -1, 0, +1 };
    const float coord5[5] = { -2, -1, 0, +1, +2 };
    const float divider = 1;
    float MAP_CX = g_RenderResolution.x;
    float MAP_CY = g_RenderResolution.y;
    const float3 grayScale = float3(0.3, 0.59, 0.11);
    const float threshold = 0.5f; //g_float_1; // 외곽선 검출 임계값

    float4 vOutColor = (float4) 0.f;
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float fBlendIntence = 0.1f; //g_float_1;

    // Laplacian Edge Detection START
    if (false)
    {
        float4 vColorBuff = 0;
        if (true == g_bool_3) // 5x5 커널 사용
        {
            for (int i = 0; i < 25; i++)
            {
                vColorBuff += Laplacian5[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord5[i % 5] / MAP_CX, coord5[i / 5] / MAP_CY)));
            }
        }
        else // 3x3 커널 사용
        {
            for (int i = 0; i < 9; i++)
            {
                vColorBuff += Laplacian3[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord3[i % 3] / MAP_CX, coord3[i / 3] / MAP_CY)));
            }
        }
        float gray = 1 - dot(vColorBuff.xyz, grayScale);
        float4 OutLine = float4(gray, gray, gray, 1) / divider;
        float blendFactor = saturate(gray / threshold);
        vColor = (gray > threshold) ? vColor : lerp(vColor, OutLine, fBlendIntence);
    }
    // Laplacian Edge Detection END

    // sobel Edge Detection START
    if (true)
    {
        float4 vColorBuffX = 0;
        float4 vColorBuffY = 0;
        if (true == g_bool_3) // 5x5 커널 사용
        {
            for (int i = 0; i < 25; i++)
            {
                vColorBuffX += sobelX5[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord5[i % 5] / MAP_CX, coord5[i / 5] / MAP_CY)));
                vColorBuffY += sobelY5[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord5[i % 5] / MAP_CX, coord5[i / 5] / MAP_CY)));
            }
        }
        else // 3x3 커널 사용
        {
            for (int i = 0; i < 9; i++)
            {
                vColorBuffX += sobelX3[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord3[i % 3] / MAP_CX, coord3[i / 3] / MAP_CY)));
                vColorBuffY += sobelY3[i] * (g_tex_4.Sample(g_sam_0, _in.vUV + float2(coord3[i % 3] / MAP_CX, coord3[i / 3] / MAP_CY)));
            }
        }
        float grayX = dot(vColorBuffX.xyz, grayScale);
        float grayY = dot(vColorBuffY.xyz, grayScale);
        float gray = 1 - sqrt(grayX * grayX + grayY * grayY);
        float4 OutLine = float4(gray, gray, gray, 1) / divider;
        float blendFactor = saturate(gray / threshold);
        vColor = (gray > threshold) ? vColor : lerp(vColor, OutLine, fBlendIntence);
    }
    // sobel Edge Detection END
    
    if (true)
    {
    // Gaussian Blur START
        const float blurKernel[9] =
        {
            1 / 16.0, 2 / 16.0, 1 / 16.0,
        2 / 16.0, 4 / 16.0, 2 / 16.0,
        1 / 16.0, 2 / 16.0, 1 / 16.0
        };

        float4 blurredColor = 0;
        for (int i = 0; i < 9; i++)
        {
            blurredColor += blurKernel[i] * g_tex_0.Sample(g_sam_0, _in.vUV + float2(coord3[i % 3] / MAP_CX, coord3[i / 3] / MAP_CY));
        }
        vColor = lerp(vColor, blurredColor, 0.5);
    // Gaussian Blur END
    }
    
    float4 vDiffuse = g_tex_1.Sample(g_sam_0, _in.vUV);
    float4 Specular = g_tex_2.Sample(g_sam_0, _in.vUV);
    float4 Emissive = g_tex_3.Sample(g_sam_0, _in.vUV);
    
    ;
    if (0.f == (Emissive.r + Emissive.g + Emissive.b))
        vOutColor = (vColor * (vDiffuse + Emissive)) + Specular;
    else
        vOutColor = Emissive;
    //vOutColor = g_tex_4.Sample(g_sam_0, _in.vUV);
    vOutColor.a = 1.f;
    
    return vOutColor;
}


#endif