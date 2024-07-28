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

[maxvertexcount(247)] // 하나의 노드를 만들때 13개가 필요함. 최대 19. 20개는 경고를 먹는다.
void GS_AfterImage(point VS_OUT _in[1], inout LineStream<GS_OUT> _OutStream)
{
    GS_OUT output = (GS_OUT) 0;
    float3 parentPosition = g_AfterImage[0].NodePosition[0];
    float3 currentCenter = parentPosition;
    float3 worldRotation = -g_AfterImage[0].NodeRotation[0];
    float3x3 worldRotationMatrix = CreateRotationMatrix(worldRotation);

    for (int j = 0; j < g_AfterImage[0].NodeCount; ++j)
    {
        // 중심점 출력
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
            // 중심점
            output.vPosition = mul(mul(float4(currentCenter, 1), g_matView), g_matProj);
            _OutStream.Append(output);

            // 회전된 모서리 점
            float3 rotatedOffset = mul(baseOffsets[i], worldRotationMatrix);
            float3 edgePos = currentCenter + rotatedOffset;
            output.vPosition = mul(mul(float4(edgePos, 1), g_matView), g_matProj);
            _OutStream.Append(output);
            _OutStream.RestartStrip();
        }

        if (j < g_AfterImage[0].NodeCount - 1)
        {
            // 추가 선 그리기
            output.vPosition = mul(mul(float4(currentCenter, 1), g_matView), g_matProj);
            _OutStream.Append(output);
            
            // 다음 노드의 위치 계산 (트레일 방향을 따라)
            float3 nextCenter = g_AfterImage[0].NodePosition[j + 1];
            output.vPosition = mul(mul(float4(nextCenter, 1), g_matView), g_matProj);
            _OutStream.Append(output);
            _OutStream.RestartStrip();

            // 다음 반복을 위해 현재 위치 업데이트
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