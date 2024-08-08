#ifndef _DECAL
#define _DECAL

#include "value.fx"
#include "func.fx"

#define bCustomAlpha g_int_1
#define CustomAlpha g_float_0
#define DecalShape g_vec2_0.x
#define DecalType g_vec2_0.y
#define ScaleRatio g_float_1
#define MaxAnimtime g_vec2_1.x
#define CurAnimtime g_vec2_1.y
#define InsideColor g_vec4_1
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
// g_int_2 : Use Custom Decal
// g_float_0 : CustomAlpha
// g_float_1 :DecalScaleRatio

// g_vec2_0 : DecalInfo : x = shape, y = type
// g_vec2_1 : Animation info : x = Max Animation time y = Current Animation Time
// g_vec4_0 : Animation OutLinerColor
// g_vec4_1 : Circle InsideColor
// g_mat_0  : ViewInv * WorldInv
// g_tex_0  : Output Texture
// g_tex_1  : Emissive Texture
// g_tex_2  : PositionTargetTex
// ===========================

void RenderOutline(int Shape, float2 vLocal, inout float4 _Color)
{
    float2 center = float2(0.5, 0.5);
    float lineWidth = 0.05f; // ���� �β�
    float4 originalColor = _Color; // ���� ���� ����
    float4 outlineColor = g_vec4_0; // �ܰ��� ����
    float outlineStrength = 0.0f;
    
    // �ܰ��� ���� ���
    if (Shape == 0) // rect
    {
        float2 distFromEdge = min(vLocal, 1.0 - vLocal);
        float minDist = min(distFromEdge.x, distFromEdge.y);
        outlineStrength = smoothstep(lineWidth, 0.0, minDist);
    }
    else // circle
    {
        float2 CircleCenter = float2(0.5f, 0.5f);
        float fRadius = 0.5f;
        float PixelDistance = distance(vLocal.xy, CircleCenter);
        outlineStrength = smoothstep(lineWidth, 0.0, abs(fRadius - PixelDistance));
    }
    
    // ���� ����� �ܰ��� ���� ����
    float3 blendedColor = lerp(originalColor.rgb, outlineColor.rgb, outlineStrength);
    
    // ���� �� ���
    float insideAlpha = originalColor.a;
    float outlineAlpha = outlineColor.a * outlineStrength;
    float blendedAlpha = max(insideAlpha, outlineAlpha);
    
    // ���� ���� ����
    _Color = float4(blendedColor, blendedAlpha);
    
   //// �ɼ�: �߽����κ����� �Ÿ��� ���� ���̵� ȿ��
   //float distFromCenter = distance(vLocal, center);
   //float fadeEffect = smoothstep(0.5, 0.3, distFromCenter);
   //_Color.a *= fadeEffect;
}


void RenderInSide(int Shape, float2 vLocal, inout float4 _Color)
{
    float2 center = float2(0.5, 0.5);
    float aspectRatio = g_float_1; // ����, ���� ����
    float animProgress = smoothstep(0, 2, 1 - (CurAnimtime / MaxAnimtime)); // 0���� 1 ���̷� ����
    
    if (Shape == 0) // �簢��
    {
        float2 CurrnetDist = abs(vLocal - center);
        float2 adjustedDist = CurrnetDist * float2(1.0 / aspectRatio, 1.0); // aspectRatio�� ���� ���� ����
        float2 animSize = float2(animProgress, animProgress);
        if (all(adjustedDist < animSize))
            _Color = InsideColor;
    }
    else // ��
    {
        float fRadius = animProgress;
        
        float CurrentPixelDistance = distance(vLocal, center);
        
        if (fRadius > CurrentPixelDistance)
            _Color = InsideColor;
    }

}

void AnimationRisingSquare(float2 vLocal, inout float4 _Color) // �簢�� ���ϸ��̼��� �׳� �̰͸� ��
{
    float animProgress = smoothstep(0, 1, 1 - (CurAnimtime / MaxAnimtime)); // 0���� 1 ���̷� ����
    
    if (vLocal.x <= animProgress)
    {
        _Color = InsideColor;

    }

    
}


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
    
    if (DecalShape == 1)
    {
        float2 CircleCenter = float2(0.5f, 0.5f);
        float fRaidus = 0.5f;
        
        float PixelDistance = distance(vLocal.xz, CircleCenter);
        
        if (fRaidus < PixelDistance)
            discard;

    }
    
    if (DecalType == 0)
    {
        if (g_btex_0)
        {
            output.vColor = g_tex_0.Sample(g_sam_0, vLocal.xz);
        
            if (bCustomAlpha && output.vColor.a > 0.5f)
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
    }
    else
    {
        float4 _Color = { 0.f, 0.f, 0.f, 0.f };

        if (DecalShape == 1)
        {
            RenderInSide(DecalShape, vLocal.xz, _Color);
        
            RenderOutline(DecalShape, vLocal.xz, _Color);
   
        }
        else
        {
            AnimationRisingSquare(vLocal.xz, _Color);
            
            RenderOutline(DecalShape, vLocal.xz, _Color);
        }
    
        output.vColor = _Color;
        
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