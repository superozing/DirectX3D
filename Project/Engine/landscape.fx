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
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
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
    
    return output;
}

struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
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
    
    return output;
}

struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};



[domain("tri")]
DS_OUT DS_LandScape(PatchLevel _pathlevel // 각 제어점 별 분할 레벨
             , const OutputPatch<HS_OUT, 3> _Origin // 패치 원본 정점
             , float3 _Weight : SV_DomainLocation)   // 각 원본 정점에 대한 가중치)
{
    DS_OUT output = (DS_OUT) 0.f;
        
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _Origin[i].vPos * _Weight[i];
        vUV += _Origin[i].vUV * _Weight[i];
    }
    
    if (g_btex_0)
    {
        float2 FullUV = vUV / float2(g_int_0, g_int_1);
        vLocalPos.y = g_tex_0.SampleLevel(g_sam_0, FullUV, 0).x;
    }
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    
    return output;
}


//struct PS_OUT
//{
//    float4 vColor : SV_Target0;
//    float4 vPosition : SV_Target1;
//    float4 vNormal : SV_Target2;
//    float4 vEmissive : SV_Target3;
//};

float4 PS_LandScape(DS_OUT _in) : SV_Target
{
    return float4(1.f, 0.f, 1.f, 1.f);
}




#endif