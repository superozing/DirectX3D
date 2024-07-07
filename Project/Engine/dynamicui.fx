#ifndef _DYNAMICUI
#define _DYNAMICUI

#include "value.fx"
#include "struct.fx"

#define UseExpandCenter     g_bool_0
#define UseUVxDiscard       g_bool_1

#define ExpandCenterRatio   g_float_0
#define DiscardUVx          g_float_1

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float3 vScale : SCALE;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

VS_OUT VS_DynamicUI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.InstID = _in.iInstID;
    
    return output;
}

// Geometry Shader
// ������ ������ų �� �ִ� ���̴�
// 1. ��� ��ƼŬ�� ��Ȱ��ȭ ������ ���, �������� �������� �ܰ迡�� �ߴܽ�Ű��
// 2. ������ ������ ���Ǽ�
struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

[maxvertexcount(6)]
void GS_DynamicUI(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    // �߽����� Local -> World�� �̵�
    float3 vWorldPos = mul(float4(_in[0].vPos, 1.f), g_matWorld).xyz;
    
    // World ��Ŀ��� Transform Scale �� ��������
    float3 vWorldScale = float3(
        length(g_matWorld[0].xyz),
        length(g_matWorld[1].xyz),
        length(g_matWorld[2].xyz)
    );
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    output[0].vPosition = float4((vWorldScale.x * -0.5f), (vWorldScale.y * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4((vWorldScale.x * 0.5f), (vWorldScale.y * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4((vWorldScale.x * 0.5f), (vWorldScale.y * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4((vWorldScale.x * -0.5f), (vWorldScale.y * -0.5f), 0.f, 1.f);
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    // ��ƼŬ�� ViewSpace �󿡼��� �߽� ������ ���ϱ�
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    // View ��ǥ�� �̵�, ������� ����
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
        
        output[i].InstID = _in[0].InstID;
    }
      
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[2]);
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.RestartStrip();
        
}

float4 PS_DynamicUI(GS_OUT _in) : SV_Target
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
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    if (vOutColor.a < 0.001f)
        discard;
    
    return vOutColor;
}


#endif