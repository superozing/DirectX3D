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
    float lineWidth = 0.05f; // 선의 두께
    float4 originalColor = _Color; // 원래 색상 저장
    float4 outlineColor = g_vec4_0; // 외곽선 색상
    float outlineStrength = 0.0f;
    
    // 외곽선 강도 계산
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
    
    // 내부 색상과 외곽선 색상 블렌딩
    float3 blendedColor = lerp(originalColor.rgb, outlineColor.rgb, outlineStrength);
    
    // 알파 값 계산
    float insideAlpha = originalColor.a;
    float outlineAlpha = outlineColor.a * outlineStrength;
    float blendedAlpha = max(insideAlpha, outlineAlpha);
    
    // 최종 색상 설정
    _Color = float4(blendedColor, blendedAlpha);
    
   //// 옵션: 중심으로부터의 거리에 따른 페이드 효과
   //float distFromCenter = distance(vLocal, center);
   //float fadeEffect = smoothstep(0.5, 0.3, distFromCenter);
   //_Color.a *= fadeEffect;
}


void RenderInSide(int Shape, float2 vLocal, inout float4 _Color)
{
    float2 center = float2(0.5, 0.5);
    float aspectRatio = g_float_1; // 가로, 세로 비율
    float animProgress = smoothstep(0, 2, 1 - (CurAnimtime / MaxAnimtime)); // 0에서 1 사이로 제한
    
    if (Shape == 0) // 사각형
    {
        float2 CurrnetDist = abs(vLocal - center);
        float2 adjustedDist = CurrnetDist * float2(1.0 / aspectRatio, 1.0); // aspectRatio로 가로 방향 조정
        float2 animSize = float2(animProgress, animProgress);
        if (all(adjustedDist < animSize))
            _Color = InsideColor;
    }
    else // 원
    {
        float fRadius = animProgress;
        
        float CurrentPixelDistance = distance(vLocal, center);
        
        if (fRadius > CurrentPixelDistance)
            _Color = InsideColor;
    }

}

void AnimationRisingSquare(float2 vLocal, inout float4 _Color) // 사각형 에니메이션은 그냥 이것만 씀
{
    float animProgress = smoothstep(0, 1, 1 - (CurAnimtime / MaxAnimtime)); // 0에서 1 사이로 제한
    
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
    
    // 호출된 픽셀의 위치를 UV 값으로 환산
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // PositionTarget 에서 현재 호출된 픽셀쉐이더랑 동일한 지점에 접근해서 좌표값을 확인
    float4 vViewPos = g_tex_2.Sample(g_sam_0, vScreenUV);
    
    // Deferred 단계에서 그려진게 없다면 빛을 줄 수 없다.
    if (-1.f == vViewPos.w)
    {
        discard;
    }
    
    // Cube 볼륨메쉬의 로컬 공간으로 데려간다.
    float3 vLocal = mul(float4(vViewPos.xyz, 1.f), g_mat_0).xyz;
    vLocal += 0.5f;
    if (vLocal.x < 0.f || 1.f < vLocal.x ||
        vLocal.y < 0.f || 1.f < vLocal.y ||
        vLocal.z < 0.f || 1.f < vLocal.z)
    {
        discard;
    }
    
    // 볼륨메쉬 내부 판정 성공 시
    
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
//색으로 영역 체크
//====================

//PS_OUT PS_Decal(VS_OUT _in)
//{
//    PS_OUT output = (PS_OUT) 0.f;
    
//    // 호출된 픽셀의 위치를 UV 값으로 환산
//    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
//    // PositionTarget 에서 현재 호출된 픽셀쉐이더랑 동일한 지점에 접근해서 좌표값을 확인
//    float4 vViewPos = g_tex_1.Sample(g_sam_0, vScreenUV);
    
//    // Deferred 단계에서 그려진게 없다면 빛을 줄 수 없다.
//    if (-1.f == vViewPos.w)
//    {   
//        output.vColor = float4(1.f, 0.f, 0.f, 1.f);
//        return output;
//        //discard;
//    }
    
//    // Cube 볼륨메쉬의 로컬 공간으로 데려간다.
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