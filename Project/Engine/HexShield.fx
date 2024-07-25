#ifndef _HEXSHIELD
#define _HEXSHIELD

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
// HexShield
// ==========
VS_OUT VS_HexShield(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.0), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));
    
    return output;
}

float hexagon(float2 p)
{
    p = abs(p);
    return max(dot(p, float2(0.8660254, 0.5)), p.y);
}

float tilePattern(float2 uv, float scale)
{
    uv *= scale;
    float2 gv = frac(uv) - 0.5;
    float d = hexagon(gv);
    return smoothstep(0.49, 0.5, d);
}

float4 PS_HexShield(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;

    float3 baseColor = float3(0.0, 0.6, 1.0); // 기본 색상 (하늘색)

    float scale = 10.0; // 타일 패턴의 스케일
    float noise = frac(sin(dot(_in.vUV, float2(12.9898, 78.233))) * 43758.5453);
    float tiles = tilePattern(_in.vUV, scale);

    // 타일별 알파값을 독립적으로 조절하기 위한 애니메이션
    //float alpha = 0.5 + 0.5 * sin(g_time + timeOffset * 6.28);
    //float alpha = tiles + sin(g_time + noise * 6.28);
    float alpha = 1.f;

    // 최종 알파값이 최대 0.8를 넘지 않도록 설정
    alpha = min(alpha, 0.8);

    // 텍스처 샘플링
    float4 TexColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    // 텍스처 색상을 푸른 계열로 보정
    float3 correctedColor = lerp(float3(0.0, 0.6, 1.0), float3(1.0, 1.0, 1.0), TexColor.rgb);

    // 채도 조절: 알파값이 높을수록 채도가 높아짐
    float3 finalColor = lerp(float3(0.2, 0.2, 0.2), correctedColor, alpha);

    // 텍스처와 알파값 조절
    vColor.rgb = finalColor;
    vColor.a = alpha;
    
    //if (vColor.a < 0.1f)
    //    discard;
    
    return vColor;
}

#endif