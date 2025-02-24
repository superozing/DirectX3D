#ifndef _BOSSHP
#define _BOSSHP

#include "value.fx"
#include "func.fx"

#define LINE        g_int_0

#define LERPRATIO   g_float_0
#define RATIO       g_float_1


#define UseUVSplitRender    g_bool_0
#define SplitRenderCount    g_int_1

#define SINGLE  0
#define ODD     1
#define EVEN    2


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_BossHP(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_BossHP(VS_OUT _in) : SV_Target
{
    float4 FirLineColor = float4(1.f, 0.f, 0.f, 1.f);
    float4 SecLineColor = float4(1.f, 0.5f, 0.f, 1.f);
    
    if (g_btex_0)
    {
        // UV를 분할해서 여러 번 렌더링
        if (UseUVSplitRender)
        {
            _in.vUV.x *= (float) SplitRenderCount;
        }
        
        float4 returnColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        _in.vUV.x /= (float) SplitRenderCount;
        
        switch (LINE)
        {
            case SINGLE:
        {
                    if (LERPRATIO < _in.vUV.x)
                        discard;
        
                    if (RATIO < _in.vUV.x)
                        returnColor *= (FirLineColor * float4(0.6f, 0.6f, 0.6f, 1.f));
                    else
                        returnColor *= FirLineColor;
                }
                break;
        
            case ODD:
        {
                    if (LERPRATIO < _in.vUV.x)
                        returnColor *= SecLineColor;
                    else if (RATIO < _in.vUV.x)
                        returnColor *= FirLineColor * float4(0.6f, 0.6f, 0.6f, 1.f);
                    else
                        returnColor *= FirLineColor;
                }
                break;
    
            case EVEN:
        {
                    if (LERPRATIO < _in.vUV.x)
                        returnColor *= FirLineColor;
                    else if (RATIO < _in.vUV.x)
                        returnColor *= SecLineColor * float4(0.6f, 0.6f, 0.6f, 1.f);
                    else
                        returnColor *= SecLineColor;
                }
                break;
        }
        
        
        return returnColor;

    }
    
    else
    {
        switch (LINE)
        {
        case SINGLE:
        {
            if (LERPRATIO < _in.vUV.x)
                discard;
        
            if (RATIO < _in.vUV.x)
                return FirLineColor * float4(0.6f, 0.6f, 0.6f, 1.f);
            else    
                return FirLineColor;      
        }
            break;
        
        case ODD:
        {
            if (LERPRATIO < _in.vUV.x)
                return SecLineColor;
            else if (RATIO < _in.vUV.x)
                return FirLineColor * float4(0.6f, 0.6f, 0.6f, 1.f);
            else
                return FirLineColor;
         }
            break;        
    
        case EVEN:
        {
            if (LERPRATIO < _in.vUV.x)
                return FirLineColor;
            else if (RATIO < _in.vUV.x)
                return SecLineColor * float4(0.6f, 0.6f, 0.6f, 1.f);
            else
                return SecLineColor;
        }
            break;
        }
    }

    return float4(0.f, 0.f, 0.f, 0.f);
}

#endif