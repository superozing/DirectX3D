#ifndef _CROSSHAIR
#define _CROSSHAIR

#include "value.fx"
#include "func.fx"

#define SPREADRATIO     g_float_0 

#define CROSSHAIR_COLOR g_vec4_0

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

VS_OUT VS_Crosshair(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Crosshair(VS_OUT _in) : SV_Target
{
    // 상수버퍼로부터 입력받은 색상
    //float4 vColor = CROSSHAIR_COLOR;
    
    // 중심 UV
    float2 center = float2(0.5, 0.5);

    // 벌어지는 비율
    // UV는 0~1 사이의 값을 가진다. 
    // 만약 1이라면 중심에서 상하좌우로 0.5 씩 뻗어나가야 최대로 벌어지게 된다.
    float spreadRatio = SPREADRATIO / 2.f;
    
    float THICKNESS = 0.02;
    float LENGTH = 0.05;
    
    // 1. 판정 UV 왼 쪽 보다 오른 쪽에 있는가?
    // 2. 판정 UV 오른 쪽 보다 왼 쪽에 있는가?
    // 3. 판정 UV 아래 쪽 보다 위에 있는가?
    // 4. 판정 UV 위 쪽 보다 아래에 있는가?
    bool isTopBar =
        (_in.vUV.x >= center.x - THICKNESS / 2 &&
         _in.vUV.x <= center.x + THICKNESS / 2 &&
         _in.vUV.y <= center.y - spreadRatio &&
         _in.vUV.y >= center.y - spreadRatio - LENGTH);
    
    bool isBottomBar =
        (_in.vUV.x >= center.x - THICKNESS / 2 &&
         _in.vUV.x <= center.x + THICKNESS / 2 &&
         _in.vUV.y >= center.y + spreadRatio &&
         _in.vUV.y <= center.y + spreadRatio + LENGTH);

    bool isLeftBar =
        (_in.vUV.y >= center.y - THICKNESS / 2 &&
         _in.vUV.y <= center.y + THICKNESS / 2 &&
         _in.vUV.x <= center.x - spreadRatio &&
         _in.vUV.x >= center.x - spreadRatio - LENGTH);

    bool isRightBar =
        (_in.vUV.y >= center.y - THICKNESS / 2 &&
         _in.vUV.y <= center.y + THICKNESS / 2 &&
         _in.vUV.x >= center.x + spreadRatio &&
         _in.vUV.x <= center.x + spreadRatio + LENGTH);

    if (!isTopBar && !isBottomBar && !isLeftBar && !isRightBar)
        discard;

    return CROSSHAIR_COLOR / 255.f;
}






#endif