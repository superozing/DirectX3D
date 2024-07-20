#ifndef _DAMAGEDDIRECTION
#define _DAMAGEDDIRECTION

#include "value.fx"
#include "func.fx"

#define DotProduct  g_float_0 
#define RenderAngle g_float_1 

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
    
    // ���� ���� ������ ��ȯ (����)
    float angle = acos(DotProduct);

    // �ǰ� UI ���� ���
    float minAngle = -RenderAngle * 0.5;
    float maxAngle = RenderAngle * 0.5;

    // ������ ����Ͽ� ���� ������ ���ø� ���� ���� �ִ��� Ȯ��
    if (angle >= minAngle && angle <= maxAngle)
    {
        // �ؽ�ó ���ø�
        float4 sampledColor = tex.Sample(samLinear, texCoord);
        return float4(sampledColor.rgb * float3(1.0, 0.0, 0.0), sampledColor.a); // ������ �ǰ� ǥ��
    }
    else
    {
        discard; // ���� ���� �����׸�Ʈ�� ����
    }
    
    
    
    
    
    return vColor;
}






#endif