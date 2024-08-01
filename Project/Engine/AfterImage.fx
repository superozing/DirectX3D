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
             
    for (int j = 0; j < g_AfterImage[0].NodeCount; ++j)
    {
        float4x4 wvpMatrix = mul(mul(g_AfterImage[0].AfterImageWorldMat[j], g_matView), g_matProj);
        float4 centerPos = mul(float4(0, 0, 0, 1), wvpMatrix);
        float3 worldPosition = float3(g_AfterImage[0].AfterImageWorldMat[j]._41, g_AfterImage[0].
        AfterImageWorldMat[j]._42, g_AfterImage[0].AfterImageWorldMat[j]._43);
        centerPos.xyz /= centerPos.w; // 원근 나눗셈

        // 중심점 출력
        output.vPosition = centerPos;
        _OutStream.Append(output);
        _OutStream.RestartStrip();

        // 회전 행렬 추출 (정규화 포함)
        float3x3 rotationMatrix = (float3x3) g_AfterImage[0].AfterImageWorldMat[j];
        rotationMatrix[0] = normalize(rotationMatrix[0]);
        rotationMatrix[1] = normalize(rotationMatrix[1]);
        rotationMatrix[2] = normalize(rotationMatrix[2]);

        float3 baseOffsets[4] =
        {
            float3(-50, 0, -50),
            float3(50, 0, -50),
            float3(0, 100, -50),
            float3(0, -100, -50)
        };

        for (int i = 0; i < 4; i++)
        {
            output.vPosition = centerPos;
            _OutStream.Append(output);

            float3 rotatedOffset = mul(baseOffsets[i], rotationMatrix);
            float4 edgePos = mul(float4(rotatedOffset, 0.f), wvpMatrix) + centerPos;
            edgePos.xyz /= edgePos.w; // 원근 나눗셈
            output.vPosition = edgePos;
            _OutStream.Append(output);
            _OutStream.RestartStrip();
        }

        if (j < g_AfterImage[0].NodeCount - 1)
        {
            output.vPosition = centerPos;
            _OutStream.Append(output);
            
            float4x4 nextWVPMatrix = mul(mul(g_AfterImage[0].AfterImageWorldMat[j + 1], g_matView), g_matProj);
            float4 nextCenterPos = mul(float4(0.f, 0.f, 0.f, 1.f), nextWVPMatrix);
            nextCenterPos.xyz /= nextCenterPos.w; // 원근 나눗셈
   
            output.vPosition = nextCenterPos;
            _OutStream.Append(output);
            _OutStream.RestartStrip();
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