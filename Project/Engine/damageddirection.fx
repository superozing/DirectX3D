#ifndef _DAMAGEDDIRECTION
#define _DAMAGEDDIRECTION

#include "value.fx"
#include "func.fx"

#define DotProduct  g_float_0 
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
    
    // UV 좌표의 기울기 계산을 위해 중앙으로 옮김
    // (0, 0) ~ (1, 1) => (1, -1) ~ (-1, 1)
    float2 centeredUV = _in.vUV * 2.0 - 1.0;
    
    // 아크탄젠트 함수를 사용해서 비율로 각도 구하기
    float UVAngle = atan2(centeredUV.y, centeredUV.x);
    
    // 내적 값을 각도로 변환 (라디안)
    float angle = acos(DotProduct);

    // UVAngle을 [-PI, PI] 범위로 조정
    if (UVAngle < 0.0)
    {
        UVAngle += 2.0 * PI;
    }

    // hitAngle을 [-PI, PI] 범위로 조정
    float hitAngleRange = angle;
    if (hitAngleRange < 0.0)
    {
        hitAngleRange += 2.0 * PI;
    }

    // 피격 UI 범위 계산
    float minAngle = hitAngleRange - RenderAngle * 0.5;
    float maxAngle = hitAngleRange + RenderAngle * 0.5;

    
    // 내적을 사용하여 현재 UV 각도가 샘플링 범위 내에 있는지 확인
    if (UVAngle >= minAngle && UVAngle <= maxAngle)
    {
        // 텍스처 샘플링
        vColor.r = g_tex_0.Sample(g_sam_0, _in.vUV).r;
        
        vColor.a = vColor.r;
        
        // 바인딩 된 알파 값 적용
        vColor *= Alpha;
    }
    else
    {
        discard;
    }
    
    
    return vColor;
}






#endif