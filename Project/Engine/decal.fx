#ifndef _DECAL
#define _DECAL

#include "value.fx"
#include "func.fx"

#define bCustomAlpha g_int_1
#define CustomAlpha g_float_0
// ==========================
// Decal Shader
// Domain   : DOMAIN_DECAL
// MRT      : Decal
// RS_TYPE  : CULL_FRONT
// DS_TYPE  : NoTest NoWrite
// BS_TYPE  : DECAL

// Parameter
// g_int_0 : As Emissive
// g_int_1 : Use CustomAlpha
// g_float_0 : CustomAlpha
// g_mat_0  : ViewInv * WorldInv
// g_tex_0  : Output Texture
// g_tex_1  : Emissive Texture
// g_tex_2  : PositionTargetTex
// ===========================
struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

VS_OUT VS_Decal(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target;
    float4 vEmissive : SV_Target1;
};

PS_OUT PS_Decal(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // ȣ��� �ȼ��� ��ġ�� UV ������ ȯ��
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // PositionTarget ���� ���� ȣ��� �ȼ����̴��� ������ ������ �����ؼ� ��ǥ���� Ȯ��
    float4 vViewPos = g_tex_2.Sample(g_sam_0, vScreenUV);
    
    // Deferred �ܰ迡�� �׷����� ���ٸ� ���� �� �� ����.
    if (-1.f == vViewPos.w)
    {
        discard;
    }
    
    // Cube �����޽��� ���� �������� ��������.
    float3 vLocal = mul(float4(vViewPos.xyz, 1.f), g_mat_0).xyz;
    vLocal += 0.5f;
    if (vLocal.x < 0.f || 1.f < vLocal.x ||
        vLocal.y < 0.f || 1.f < vLocal.y ||
        vLocal.z < 0.f || 1.f < vLocal.z)
    {
        discard;
    }
    
    // �����޽� ���� ���� ���� ��
    if (g_btex_0)
    {
        output.vColor = g_tex_0.Sample(g_sam_0, vLocal.xz);
        
        if (bCustomAlpha)
        {
            output.vColor.a = CustomAlpha;
        }
        
        if (g_int_0)
        {
            if (g_btex_1)
            {
                output.vEmissive.rgb = g_tex_1.Sample(g_sam_0, vLocal.xz);
            }
            else
            {
                if (bCustomAlpha)
                    output.vEmissive.rgb = output.vColor.rgb * CustomAlpha;
                else
                    output.vEmissive.rgb = output.vColor.rgb * output.vColor.a;
            }
        }
    }
    else
    {
        output.vColor = float4(1.f, 0.f, 1.f, 1.f);
        
        if (g_int_0)
        {
            output.vEmissive = float4(0.4f, 0.4f, 0.4f, 1.f);
        }
    }
    
    return output;
}



//====================
//������ ���� üũ
//====================

//PS_OUT PS_Decal(VS_OUT _in)
//{
//    PS_OUT output = (PS_OUT) 0.f;
    
//    // ȣ��� �ȼ��� ��ġ�� UV ������ ȯ��
//    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
//    // PositionTarget ���� ���� ȣ��� �ȼ����̴��� ������ ������ �����ؼ� ��ǥ���� Ȯ��
//    float4 vViewPos = g_tex_1.Sample(g_sam_0, vScreenUV);
    
//    // Deferred �ܰ迡�� �׷����� ���ٸ� ���� �� �� ����.
//    if (-1.f == vViewPos.w)
//    {   
//        output.vColor = float4(1.f, 0.f, 0.f, 1.f);
//        return output;
//        //discard;
//    }
    
//    // Cube �����޽��� ���� �������� ��������.
//    float3 vLocal = mul(float4(vViewPos.xyz, 1.f), g_mat_0).xyz;    
//    vLocal += 0.5f;    
//    if (vLocal.x < 0.f || 1.f < vLocal.x ||
//        vLocal.y < 0.f || 1.f < vLocal.y || 
//        vLocal.z < 0.f || 1.f < vLocal.z)
//    {
//        output.vColor = float4(0.f, 0.f, 1.f, 1.f);
//        return output;
//        //discard;
//    }
        
//    output.vColor = float4(0.f, 1.f, 0.f, 1.f);
//    return output;
//}




#endif