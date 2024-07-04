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
    // ������۷κ��� �Է¹��� ����
    //float4 vColor = CROSSHAIR_COLOR;
    
    // �߽� UV
    float2 center = float2(0.5, 0.5);

    // �������� ����
    // UV�� 0~1 ������ ���� ������. 
    // ���� 1�̶�� �߽ɿ��� �����¿�� 0.5 �� ������� �ִ�� �������� �ȴ�.
    float spreadRatio = SPREADRATIO / 2.f;
    
    float THICKNESS = 0.02;
    float LENGTH = 0.05;
    
    // 1. ���� UV �� �� ���� ���� �ʿ� �ִ°�?
    // 2. ���� UV ���� �� ���� �� �ʿ� �ִ°�?
    // 3. ���� UV �Ʒ� �� ���� ���� �ִ°�?
    // 4. ���� UV �� �� ���� �Ʒ��� �ִ°�?
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