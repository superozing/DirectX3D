#ifndef _BOSSSHIELD
#define _BOSSSHIELD

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    // Per Instance Data    
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

// ==========
// BossShield
// ==========
VS_OUT VS_BossShield(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.0), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));;
    
    return output;
}

float4 PS_BossShield(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // View 공간에서 카메라(원점) 가 픽셀(대상) 을 바라보는 시선 방향벡터는 물체의 좌표이다.
    float3 vEye = normalize(_in.vViewPos);
    
    float shieldTime = saturate(g_float_0) * 0.5f;
    //float shieldTime = 1.f;
    
    // 프레넬 효과 계산
    float fresnel = dot(_in.vViewNormal, -vEye);
    fresnel = 1.0 - fresnel;
    fresnel = pow(fresnel, 3.0);

    // 쉴드 색상 (푸른 계열)
    float3 shieldColor = float3(0, 0.6, 1);

    float3 tex = g_tex_0.Sample(g_sam_0, _in.vUV + float2(g_time * 0.1, 0)).rgb;
   
    // 최종 색상 결정
    float3 finalColor = lerp(tex, shieldColor, fresnel);
    
    //                 cos((vScreenUV.x + (g_time * (속도) )) * (주파수)) * (진폭);
    float noiseValue = cos((_in.vUV.x + (g_time * 0.05f)) * 100.f) * 0.05f;
    if (_in.vUV.y + noiseValue * 0.1f > shieldTime)
    {
        discard; // 렌더링하지 않음
    }
    
    vColor.rgb = finalColor;
    vColor.a = fresnel;
    
    return vColor;
}


#endif