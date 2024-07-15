#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

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
VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;    
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = g_postprocess.Sample(g_sam_0, _in.vUV);
    
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}


// 
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // 픽셀쉐이더에 SV_Position 으로 입력된 값은 픽셀 쉐이더를 호출한 해당 픽셀의 좌표가 들어있다.
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    //vScreenUV.y += cos((vScreenUV.x + (g_time * (속도) )) * (주파수)) * (진폭);
    //vScreenUV.y += cos((vScreenUV.x + (g_time *  0.1f))   *   40.f)  *  0.1f;
    
    if (g_btex_0)
    {
        float2 vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
        vNoise = (vNoise.xy - 0.5f) * 0.1f;
        
        vScreenUV += vNoise;
    }
        
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}

VS_OUT VS_Vignette(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Vignette(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // 블룸 이후의 이미지를 샘플링
    vColor = g_postprocess.Sample(g_sam_0, _in.vUV);

    float4 vignetteTex = (float4) 0.f;
    // 비네트 전용 텍스처를 샘플링
    if (g_int_1 == 0)
        vignetteTex = g_tex_0.Sample(g_sam_0, _in.vUV);
    else if (g_int_1 == 1)
        vignetteTex = g_tex_1.Sample(g_sam_0, _in.vUV);
    else if (g_int_1 == 2)
        vignetteTex = g_tex_2.Sample(g_sam_0, _in.vUV);
    else if (g_int_1 == 3)
        vignetteTex = g_tex_3.Sample(g_sam_0, _in.vUV);
    else if (g_int_1 == 4)
        vignetteTex = g_tex_4.Sample(g_sam_0, _in.vUV);

    // 비네트 효과를 위한 알파 값 설정
    float alphaValue = vignetteTex.a * g_float_0;
    
    // 비네트 색상 정의 (여기서는 빨간색으로 설정)
    float4 vignetteColor = float4(0.8f, 0.0f, 0.0f, alphaValue);

    // 원래 색상과 비네트 색상을 혼합
    vColor = lerp(vColor, vignetteColor, vignetteColor.a);

    return vColor;
}

#endif