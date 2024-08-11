#ifndef _Laser
#define _Laser

#include "value.fx"

//===========
//Laser Shader
//===========
// MRT : SwapChain
// Mesh : PointMesh
// g_vec4_0 : LarserFrontDir
// g_vec4_1 :LaserColor
// g_float_0 : Length
// g_float_1 : Alpha 
// g_float_2 : Width
//===========

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

VS_OUT VS_Laser(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
    
    return output;
}

[maxvertexcount(8)] // 평면 2개
void GS_Laser(point VS_OUT input[1], inout TriangleStream<GS_OUT> triStream)
{
    float3 vLaserDir = normalize(g_vec4_0.xyz); // 방향
    float fLength = g_float_0; // 길이
    float fWidth = g_float_2; // 너비

    float4 vStart = input[0].vPos; // 시작
    float4 vEnd = vStart + float4(vLaserDir * fLength, 0.0f); // 끝

    float3 vUp = float3(-vLaserDir.y, vLaserDir.x, 0); // 위 벡터
    float3 vRight = cross(vLaserDir, vUp); // 우 벡터

    vUp = normalize(vUp);
    vRight = normalize(vRight);

    float2 uvs[4] =
    {
        float2(0, 0), float2(0, 1),
        float2(1, 0), float2(1, 1)
    };

    for (int plane = 0; plane < 2; ++plane)
    {
        float3 vNormal = (plane == 0) ? vUp : vRight;

        for (int i = 0; i < 4; ++i)
        {
            GS_OUT output;
            float4 vOffset = (i < 2 ? vStart : vEnd) + float4(vNormal * (i % 2 == 0 ? -fWidth : fWidth), 0);
            output.vPosition = mul(mul(vOffset, g_matView), g_matProj);
            output.vUV = uvs[i];
            output.vWorldPos = vOffset.xyz; // 월드 위치 전달 (필요시)
            
            triStream.Append(output);
        }
        triStream.RestartStrip();
    }
}

PS_OUT PS_Laser(GS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 uv = _in.vUV;
    
    output.vColor.xyz = g_vec4_1.xyz;
    output.vColor.a = g_float_1;
    
    return output;
}

#endif