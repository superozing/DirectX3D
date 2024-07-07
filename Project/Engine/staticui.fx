#ifndef _STATICUI
#define _STATICUI

#include "value.fx"
#include "func.fx"

#define UseExpandCenter     g_bool_0
#define UseUVxDiscard       g_bool_1
#define UseMulColor         g_bool_2

#define ExpandCenterRatio   g_float_0
#define DiscardUVx          g_float_1

#define MulColor         g_vec4_0

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

VS_OUT VS_StaticUI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_StaticUI(VS_OUT _in) : SV_Target
{
    // ��� ����
    float4 vOutColor = (float4) 0.f;
    
    if (UseUVxDiscard && DiscardUVx < _in.vUV.x)
        discard;
    
    // �߾� ���� ����� ����Ѵٸ� vUV �����ϱ�.
    if (UseExpandCenter)
    {
        // ��� float �ϳ��� ������ �߾��� uv ���� ������ ���ΰ�?
        
        // ������ �����غ��ƿ�.
        
        // uv x�� 
        // 0.f ~ (0.5 - ExpandCenterRatio / 2) ���̸� 0 ~ 0.5�� UVx�� ����
        // (0.5 - ExpandCenterRatio / 2) ~ (0.5 + ExpandCenterRatio / 2) ���̸� 0.5�� UVx�� ����
        // (0.5 + ExpandCenterRatio / 2) ~ 1.f ���̸� 0.5 ~ 1�� UVx�� ����
        
        float CenterUVMin = 0.5 - ExpandCenterRatio / 2.f;
        float CenterUVMax = 0.5 + ExpandCenterRatio / 2.f;
        
        float UVx = _in.vUV.x;
        
        
        if (UVx < CenterUVMin)
        {
            // 0 ~ CenterUVMin ������ ���� ���
            float t = UVx / CenterUVMin;
            _in.vUV.x = lerp(0.f, 0.5, t);

        }
        else if (UVx > CenterUVMax)
        {
            // 0 ~ CenterUVMax ������ ���� ���
            float t = (UVx - CenterUVMax) / (1.f - CenterUVMax);
            _in.vUV.x = lerp(0.5f, 1.f, t);

        }
        else
        {
            _in.vUV.x = 0.5f;
        }
    }
    
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (UseMulColor)
        vOutColor *= MulColor;
    
    if (vOutColor.a < 0.001f)
        discard;
    
    return vOutColor;
}

#endif