#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"

// =======================================
// LandScape Shader
// MRT      : Deferred
// RS_TYPE  : CULL_BACK
// DS_TYPE  : LESS
// BS_TYPE  : DEFAULT

// Parameter
// g_int_0  : Face X
// g_int_1  : Face Z
// g_tex_0  : HeightMap Texture
// =======================================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
    output.vTangent = _in.vTangent;
    output.vBinormal = _in.vBinormal;
    output.vNormal = _in.vNormal;
    
    return output;
}

// Hull Shader
struct PatchLevel
{
    float arrEdge[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

PatchLevel PatchConstFunc(InputPatch<VS_OUT, 3> _in, uint patchID : SV_PrimitiveID)
{
    PatchLevel output = (PatchLevel) 0.f;
        
    output.arrEdge[0] = g_vec4_0.x;
    output.arrEdge[1] = g_vec4_0.y;
    output.arrEdge[2] = g_vec4_0.z;
    output.Inside = g_vec4_0.w;
    
    for (int i = 0; i < 3; i++)
    {
        if (output.arrEdge[i] == 0.f)
            output.arrEdge[i] = 1.f;
    }
    
    if (output.Inside == 0.f)
    {
        output.Inside = 1.f;
    }
    
    return output;
}

struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

[patchconstantfunc("PatchConstFunc")]
[outputtopology("triangle_cw")]
[domain("tri")]
[maxtessfactor(64)]
[partitioning("integer")] //[partitioning("fractional_odd")]
[outputcontrolpoints(3)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _in, uint _idx : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vPos = _in[_idx].vPos;
    output.vUV = _in[_idx].vUV;
    
    output.vTangent = _in[_idx].vTangent;
    output.vBinormal = _in[_idx].vBinormal;
    output.vNormal = _in[_idx].vNormal;
    
    return output;
}

struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
};

[domain("tri")]
DS_OUT DS_LandScape(PatchLevel _pathlevel // 각 제어점 별 분할 레벨
             , const OutputPatch<HS_OUT, 3> _Origin // 패치 원본 정점
             , float3 _Weight : SV_DomainLocation)   // 각 원본 정점에 대한 가중치)
{
    DS_OUT output = (DS_OUT) 0.f;
        
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    float3 vTangent = (float3) 0.f;
    float3 vBinormal = (float3) 0.f;
    float3 vNormal = (float3) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _Origin[i].vPos * _Weight[i];
        vUV += _Origin[i].vUV * _Weight[i];
        
        vTangent += _Origin[i].vTangent * _Weight[i];
        vBinormal += _Origin[i].vBinormal * _Weight[i];
        vNormal += _Origin[i].vNormal * _Weight[i];
    }
    
    // 높이맵 텍스쳐가 있을 때
    if (g_btex_0)
    {
        float2 FullUV = vUV / float2(g_int_0, g_int_1);
        vLocalPos.y = g_tex_0.SampleLevel(g_sam_0, FullUV, 0).x;
        
        // 주변 정점(위, 아래, 좌, 우) 로 접근할때의 로컬스페이스상에서의 간격
        float LocalStep = 1.f / _pathlevel.Inside;
        
        // 주변 정점(위, 아래, 좌, 우) 의 높이를 높이맵에서 가져올때 중심UV 에서 주변UV 로 접근할때의 UV 변화량
        float2 vUVStep = LocalStep / float2(g_int_0, g_int_1);
        
        // 위
        float3 vUp = float3(vLocalPos.x
                            , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x, FullUV.y - vUVStep.y), 0).x
                            , vLocalPos.z + LocalStep);
        
        // 아래
        float3 vDown = float3(vLocalPos.x
                             , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x, FullUV.y + vUVStep.y), 0).x
                             , vLocalPos.z - LocalStep);
        
        // 좌
        float3 vLeft = float3(vLocalPos.x - LocalStep
                             , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x - vUVStep.x, FullUV.y), 0).x
                             , vLocalPos.z);
        
        // 우
        float3 vRight = float3(vLocalPos.x + LocalStep
                            , g_tex_0.SampleLevel(g_sam_0, float2(FullUV.x + vUVStep.x, FullUV.y), 0).x
                            , vLocalPos.z);
        
        
        vTangent = mul(float4(vRight, 1.f), g_matWorld).xyz - mul(float4(vLeft, 1.f), g_matWorld).xyz;
        vBinormal = mul(float4(vDown, 1.f), g_matWorld).xyz - mul(float4(vUp, 1.f), g_matWorld).xyz;
        vNormal = normalize(cross(vTangent, vBinormal));
        
        output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matView).xyz);
    }
    else
    {
        output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matWV).xyz);
    }
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV).xyz;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vEmissive : SV_Target3;
};

PS_OUT PS_LandScape(DS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
        
    output.vColor = float4(0.4f, 0.4f, 0.4f, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vNormal = float4(_in.vViewNormal, 1.f);
    output.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    
    return output;
}

#endif