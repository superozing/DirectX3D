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
// 정점을 생성시킬 수 있는 쉐이더
// 1. 담당 파티클이 비활성화 상태인 경우, 렌더링을 정점연산 단계에서 중단시키기
// 2. 빌보드 구현의 편의성
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
    
    // 중심점을 Local -> World로 이동
    float3 vWorldPos = mul(float4(_in[0].vPos, 1.f), g_matWorld).xyz;
    
    // World 행렬에서 Transform Scale 값 가져오기
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
    
    // 파티클의 ViewSpace 상에서의 중심 포지션 구하기
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    // View 좌표로 이동, 투영행렬 적용
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
    // 출력 색상
    float4 vOutColor = (float4) 0.f;
    
    if (UseUVxDiscard && DiscardUVx < _in.vUV.x)
        discard;
    
    // 중앙 연장 기능을 사용한다면 vUV 조정하기.
    if (UseExpandCenter)
    {
        // 어떻게 float 하나를 가지고 중앙의 uv 값을 조절할 것인가?
        
        // 수식을 생각해보아요.
        
        // uv x가 
        // 0.f ~ (0.5 - ExpandCenterRatio / 2) 사이면 0 ~ 0.5의 UVx로 보간
        // (0.5 - ExpandCenterRatio / 2) ~ (0.5 + ExpandCenterRatio / 2) 사이면 0.5의 UVx로 고정
        // (0.5 + ExpandCenterRatio / 2) ~ 1.f 사이면 0.5 ~ 1의 UVx로 보간
        
        float CenterUVMin = 0.5 - ExpandCenterRatio / 2.f;
        float CenterUVMax = 0.5 + ExpandCenterRatio / 2.f;
        
        float UVx = _in.vUV.x;
        
        
        if (UVx < CenterUVMin)
        {
            // 0 ~ CenterUVMin 사이의 비율 계산
            float t = UVx / CenterUVMin;
            _in.vUV.x = lerp(0.f, 0.5, t);

        }
        else if (UVx > CenterUVMax)
        {
            // 0 ~ CenterUVMax 사이의 비율 계산
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