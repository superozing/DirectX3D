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
void GS_DamageFont(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
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
    output[1].vUV = float2(DIGIT, 0.f); // 자릿 수 판별을 쉽게 하기 위해서 UV x를 임의로 늘려놓음.
    output[2].vUV = float2(DIGIT, 1.f); // 자릿 수 판별을 쉽게 하기 위해서 UV x를 임의로 늘려놓음.
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

float4 PS_DamageFont(GS_OUT _in) : SV_Target
{
    
    // 예를 들어 10을 출력한다고 가정하고, 짜보자
    // DIGIT : 2
    // DAMAGE: 10
    
    // 알아내야 하는 것 
    // 1. 현재 찍히는 픽셀의 UV값
    //  0~DIGIT 사이의 값이 들어오니까 
    //  10의 자리를 찍기 위해서는 입력 UV가 0 ~ 1 여야 하고
    //  1의 자리를 찍기 위해서는 입력 UV가 1 ~ 2 어야 한다.
    //  일단 10의 자리를 먼저 떠올려보자.
    //  첫(가장 큰) 번째 자릿수는 1이고, 입력 UV는 0~1이 들어오겠지?
    
    // 2. 현재 찍어야 하는 자릿수
    //  현재 찍어야 하는 자릿수는 입력 UV의 x 값으로 판단할 수 있다.
    //  예를 들어서 1보다 같거나 작으면 가장 첫 번째(큰) 자릿수
    //  2보다 같거나 작으면 두 번째 자릿수.
    
    // 3. 현재 찍어야 하는 자릿수에 해당하는 숫자
    //  일단 UV를 12등분 하면
    //  0 ~ 1/12까지는 1, 1/12 ~ 2/12까지는 2... 9/12 ~ 10/12까지는 0이 나오겠죠?
    //  만약 입력 UV가 0~1일 경우, 첫 자리를 찍음.
    //  10의 (DIGIT - (n번째 자리)) 제곱을 DAMAGE와 / 하면 어떤 숫자를 찍어야 하는지 알 수 있고,
    //  10^(2 - 1) / 10 = 1
    //  
    //  (찍어야 할 숫자) - 1 / 12 를 텍스쳐에서 가져올 UV로 맟추면 된다.
    
    
    // 현재 UV에 해당하는 숫자 자릿수
    int digitnum = ceil(_in.vUV.x);
    
    // 자릿수에 해당하는 숫자
    int rendernum = pow(10, DIGIT - digitnum) / DAMAGE;
    
    float UVx = frac(_in.vUV.x);
    
    // 0일 경우 예외 처리 필요
    _in.vUV.x = (rendernum - 1 + UVx) / 12.f;
    
    // 출력 색상
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