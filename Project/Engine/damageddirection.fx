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
    
    // 내적 값을 각도로 변환 (라디안)
    float angle = acos(DotProduct);

    // 피격 UI 범위 계산
    float minAngle = -RenderAngle * 0.5;
    float maxAngle = RenderAngle * 0.5;

    // 내적을 사용하여 현재 각도가 샘플링 범위 내에 있는지 확인
    if (angle >= minAngle && angle <= maxAngle)
    {
        // 텍스처 샘플링
        float4 sampledColor = tex.Sample(samLinear, texCoord);
        return float4(sampledColor.rgb * float3(1.0, 0.0, 0.0), sampledColor.a); // 빨간색 피격 표시
    }
    else
    {
        discard; // 범위 밖의 프래그먼트는 버림
    }
    
    
    
    
    
    return vColor;
}






#endif