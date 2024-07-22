#ifndef _DAMAGEDDIRECTION
#define _DAMAGEDDIRECTION

#include "value.fx"
#include "func.fx"

#define RenderAngle g_float_1
#define Alpha       g_float_2

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

VS_OUT VS_DamagedDirection(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DamagedDirection(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // UV ��ǥ�� ���� ����� ���� �߾����� �ű�
    // (0, 0) ~ (1, 1) => (1, -1) ~ (-1, 1)
    float2 centeredUV = _in.vUV * 2.0 - 1.0;
    
    // ��ũź��Ʈ �Լ��� ����ؼ� ������ ���� ���ϱ�
    float UVAngle = atan2(centeredUV.y, centeredUV.x);

    // UVAngle�� [0, 2*PI] ������ ����
    if (UVAngle < 0.0)
    {
        UVAngle += 2.0 * PI;
    }

    // ���� ���� ���� (���)
    float baseAngle = PI / 2;

    // �ǰ� UI ���� ���
    float minAngle = baseAngle - RenderAngle * 0.5;
    float maxAngle = baseAngle + RenderAngle * 0.5;

    // UV ��ǥ�� ������ ���ø� ���� ���� �ִ��� Ȯ��
    if (UVAngle >= minAngle && UVAngle <= maxAngle)
    {
        // �ؽ�ó ���ø�
        vColor.r = g_tex_0.Sample(g_sam_0, _in.vUV).r;
        
        vColor.a = vColor.r;
        
        // ���ε� �� ���� �� ����
        vColor *= Alpha;
    }
    else
    {
        discard;
    }
    
    return vColor;
}

#endif