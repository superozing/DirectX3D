#ifndef _DAMAGEFONT
#define _DAMAGEFONT

#include "value.fx"
#include "struct.fx"

#define DIGIT   g_int_0
#define DAMAGE  g_int_1

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

VS_OUT VS_DamageFont(VS_IN _in)
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
void GS_DamageFont(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
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
    output[1].vUV = float2(DIGIT, 0.f); // �ڸ� �� �Ǻ��� ���� �ϱ� ���ؼ� UV x�� ���Ƿ� �÷�����.
    output[2].vUV = float2(DIGIT, 1.f); // �ڸ� �� �Ǻ��� ���� �ϱ� ���ؼ� UV x�� ���Ƿ� �÷�����.
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

float4 PS_DamageFont(GS_OUT _in) : SV_Target
{
    
    // ���� ��� 10�� ����Ѵٰ� �����ϰ�, ¥����
    // DIGIT : 2
    // DAMAGE: 10
    
    // �˾Ƴ��� �ϴ� �� 
    // 1. ���� ������ �ȼ��� UV��
    //  0~DIGIT ������ ���� �����ϱ� 
    //  10�� �ڸ��� ��� ���ؼ��� �Է� UV�� 0 ~ 1 ���� �ϰ�
    //  1�� �ڸ��� ��� ���ؼ��� �Է� UV�� 1 ~ 2 ��� �Ѵ�.
    //  �ϴ� 10�� �ڸ��� ���� ���÷�����.
    //  ù(���� ū) ��° �ڸ����� 1�̰�, �Է� UV�� 0~1�� ��������?
    
    // 2. ���� ���� �ϴ� �ڸ���
    //  ���� ���� �ϴ� �ڸ����� �Է� UV�� x ������ �Ǵ��� �� �ִ�.
    //  ���� �� 1���� ���ų� ������ ���� ù ��°(ū) �ڸ���
    //  2���� ���ų� ������ �� ��° �ڸ���.
    
    // 3. ���� ���� �ϴ� �ڸ����� �ش��ϴ� ����
    //  �ϴ� UV�� 12��� �ϸ�
    //  0 ~ 1/12������ 1, 1/12 ~ 2/12������ 2... 9/12 ~ 10/12������ 0�� ��������?
    //  ���� �Է� UV�� 0~1�� ���, ù �ڸ��� ����.
    //  10�� (DIGIT - (n��° �ڸ�)) ������ DAMAGE�� / �ϸ� � ���ڸ� ���� �ϴ��� �� �� �ְ�,
    //  10^(2 - 1) / 10 = 1
    //  
    //  (���� �� ����) - 1 / 12 �� �ؽ��Ŀ��� ������ UV�� ���߸� �ȴ�.
    
    
    // ���� UV�� �ش��ϴ� �ڸ��� 
    // ���� ���, �ټ� �ڸ� ���� �ִٸ�
    // 10000�� �ڸ��� 1, 1000�� �ڸ��� 2, 100�� �ڸ��� 3...
    int digitnum = trunc(_in.vUV.x) + 1;
    

    // �ڸ����� �ش��ϴ� ����
    int rendernum = (DAMAGE / pow(10, DIGIT - digitnum)) % 10;

    float UVx = frac(_in.vUV.x);
    
    // ���� ���ø��� UV��ǥ ���
    _in.vUV.x = rendernum == 0 ? (9 + UVx) / 12.f : (rendernum - 1 + UVx) / 12.f;
    
    // ��� ����
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    if (vOutColor.a < 0.001f)
        discard;
    
    return vOutColor;
}


#endif