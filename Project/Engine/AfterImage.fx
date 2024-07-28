#ifndef AfterImage
#define AfterImage

#include "value.fx"
#include "struct.fx"
#include "func.fx"

#define NodeRender g_bool_0

struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
};

struct PS_OUT
{
    float4 vColor : SV_Target0;
};


VS_OUT VS_AfterImage(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    return output;
}

[maxvertexcount(247)] // �ϳ��� ��带 ���鶧 13���� �ʿ���. �ִ� 19. 20���� ��� �Դ´�.
void GS_AfterImage(point VS_OUT _in[1], inout LineStream<GS_OUT> _OutStream)
{
    GS_OUT output = (GS_OUT) 0;
    float3 parentPosition = g_AfterImage[0].NodePosition[0];
    float3 currentCenter = parentPosition;
    float3 worldRotation = -g_AfterImage[0].NodeRotation[0];
    float3x3 worldRotationMatrix = CreateRotationMatrix(worldRotation);

    for (int j = 0; j < g_AfterImage[0].NodeCount; ++j)
    {
        // �߽��� ���
        output.vPosition = mul(mul(float4(currentCenter, 1), g_matView), g_matProj);
        _OutStream.Append(output);
        _OutStream.RestartStrip();

        float3 baseOffsets[4] =
        {
            float3(-50, 0, -50),
            float3(50, 0, -50),
            float3(0, 100, -50),
            float3(0, -100, -50)
        };

        for (int i = 0; i < 4; i++)
        {
            // �߽���
            output.vPosition = mul(mul(float4(currentCenter, 1), g_matView), g_matProj);
            _OutStream.Append(output);

            // ȸ���� �𼭸� ��
            float3 rotatedOffset = mul(baseOffsets[i], worldRotationMatrix);
            float3 edgePos = currentCenter + rotatedOffset;
            output.vPosition = mul(mul(float4(edgePos, 1), g_matView), g_matProj);
            _OutStream.Append(output);
            _OutStream.RestartStrip();
        }

        if (j < g_AfterImage[0].NodeCount - 1)
        {
            // �߰� �� �׸���
            output.vPosition = mul(mul(float4(currentCenter, 1), g_matView), g_matProj);
            _OutStream.Append(output);
            
            // ���� ����� ��ġ ��� (Ʈ���� ������ ����)
            float3 nextCenter = g_AfterImage[0].NodePosition[j + 1];
            output.vPosition = mul(mul(float4(nextCenter, 1), g_matView), g_matProj);
            _OutStream.Append(output);
            _OutStream.RestartStrip();

            // ���� �ݺ��� ���� ���� ��ġ ������Ʈ
            currentCenter = nextCenter;
            worldRotation = -g_AfterImage[0].NodeRotation[j + 1];
            worldRotationMatrix = CreateRotationMatrix(worldRotation);
        }
        
    
    }
    
   
}


PS_OUT PS_AfterImage(GS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
   
    if (NodeRender == true)
    {
        output.vColor = float4(1.f, 0.f, 0.f, 1.f);
        
    }
    else
    {
        discard;
    }
    
    return output;
  
};

#endif