#ifndef _HEXSHIELD
#define _HEXSHIELD

#include "value.fx"

#define AlphaOffset g_float_0

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
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));
    
    return output;
}

float4 PS_HexShield(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float3 baseColor = float3(0.f, 0.6f, 1.f); // �⺻ ���� (�ϴû�)
    
    float3 normal = normalize(_in.vViewNormal);
    float3 normalGroup = round(normal * 2.f) / 2.0;

    // �׷캰 ���� ���� ���������� �����ϱ� ���� �ִϸ��̼�
    float timeOffset = frac(sin(dot(normalGroup.xy, float2(12.9898, 78.233))) * 43758.5453);
    float alpha = 0.35f + 0.5f * sin(g_time + timeOffset * 6.28);

    // ���� ���İ��� �ִ� 0.7�� ���� �ʵ��� ����
    alpha = min(alpha, 0.5f);

    // �ؽ�ó ���ø�
    float4 flareColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    // �ؽ�ó ������ Ǫ�� �迭�� ����
    float3 correctedColor = lerp(float3(0.f, 0.6f, 1.f), float3(1.f, 1.f, 1.f), flareColor.rgb);

    // ä�� ���� : ���İ��� �������� ä���� ������
    float3 finalColor = lerp(float3(0.f, 0.f, 0.f), correctedColor, alpha);
    
    vColor.rgb = finalColor * AlphaOffset;
    vColor.a = alpha;

    return vColor;
}

#endif