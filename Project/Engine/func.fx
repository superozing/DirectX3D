#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"


void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    // 빛을 적용시킬 광원의 정보
    tLightInfo info = g_Light2D[_LightIdx];    
    
    // Directional Light
    if (0 == info.LightType)
    {
        _output.vAmbient += info.Color.vAmbient;
    }
    
    // Point Light
    else if (1 == info.LightType)
    {
        float fAttenu = 1.f;
        
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        if (fDist < info.fRadius)
        {
            if (g_int_0)
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            }
            else
            {
                fAttenu = saturate(1.f - fDist / g_Light2D[0].fRadius);
            }
            
            _output.vColor += info.Color.vColor * fAttenu;
        }
    }
    
    // Spot Light
    else
    {
        float fAttenu = 1.f; // 각도 감쇠
        float fAttenu2 = 1.f; // 거리 감쇠
        
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        
        float2 lightDir = normalize(info.vWorldDir.xy); // 초기 광원의 방향 벡터
        float2 targetDir = normalize(_WorldPos.xy - info.vWorldPos.xy); // 타겟 벡터
        
        
        // 광원 방향과 타겟 방향 사이의 각도를 계산
        float fTheta = acos(dot(lightDir, targetDir));
        
        // 타겟 각도가 광원의 각도 범위 안에 있을 때
        if (fTheta < info.fAngle)
        {
            // 각도에 따른 감쇠 계산
            fAttenu = saturate(1.f - fTheta / info.fAngle);
            
            if (fDist < info.fRadius)
            {
                float fTheta2 = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu2 = saturate(cos(fTheta2));
            }
            else
            {
                fAttenu2 = saturate(1.f - fDist / g_Light2D[0].fRadius);
            }
        }
        // 타겟 각도가 광원 각도 범위 밖이면 보이지 않음
        else
        {
            fAttenu = 0.f;
        }
        
        _output.vColor += info.Color.vColor * fAttenu * fAttenu2;
    }
}

void CalLight3D(int _LightIdx, float3 _vViewPos, float3 _vViewNormal, inout tLightColor _LightColor)
{
    // 광원의 정보를 확인
    tLightInfo Light = g_Light3D[_LightIdx];
    
    // 광원이 물체를 향하는 방향벡터
    float3 vViewLightDir = (float3) 0.f;

    float fDistanceRatio = 1.f;

    // Directional Light
    if (0 == Light.LightType)
    {
        // 광원 연산이 ViewSpace 에서 진행되기로 했기 때문에,
        // 광원이 진입하는 방향도 View 공간 기준으로 변경함
        vViewLightDir = normalize(mul(float4(Light.vWorldDir, 0.f), g_matView).xyz);
    }

    // Point Light
    else if (1 == Light.LightType)
    {
        float3 vLightViewPos = mul(float4(Light.vWorldPos, 1.f), g_matView).xyz;
        vViewLightDir = _vViewPos - vLightViewPos;

        // 광원과 물체 사이의 거리
        float fDistance = length(vViewLightDir);
        vViewLightDir = normalize(vViewLightDir);
        
        // 광원 반경과 물체까지의 거리에 따른 빛의 세기
        //fDistanceRatio = saturate(1.f - (fDistance / Light.fRadius)); // 선형적
        fDistanceRatio = saturate(cos(fDistance / Light.fRadius * (PI / 2.f))); // cos
    }

    // Spot Light
    else
    {
        float3 vLightViewPos = mul(float4(Light.vWorldPos, 1.f), g_matView).xyz;
        vViewLightDir = _vViewPos - vLightViewPos;

        // 광원과 물체 사이의 거리
        float fDistance = length(vViewLightDir);
        
        // 광원에서 물체를 향하는 방향 벡터
        vViewLightDir = normalize(vViewLightDir);

        // 광원 방향 벡터
        float3 vLightDir = mul(float4(Light.vWorldDir, 0.f), g_matView).xyz;
        
        // 광원의 방향이 (0.f, 0.f, 0.f)로 설정되어 있다면, 이것은 기본 원뿔 메쉬의 방향인 z축 방향을 가리킨다
        if (0.f == vLightDir.x && 0.f == vLightDir.y && 0.f == vLightDir.z)
        {
            vLightDir = float3(0.f, 0.f, 1.f);
        }
        vLightDir = normalize(vLightDir);
        
        // 물체와 광원 사이의 각도가 광원에 설정된 fAngle과 가까울 수록 빛의 세기가 약해져야 함
        if (0.f != Light.fAngle)
        {
        }        
    }

     // ViewSpace 에서 광원의 방향과, 물체 표면의 법선를 이용해서 광원의 진입 세기(Diffuse) 를 구한다.
    float LightPow = saturate(dot(_vViewNormal, -vViewLightDir));
            
    // 빛이 표면에 진입해서 반사되는 방향을 구한다.
    float3 vReflect = vViewLightDir + 2 * dot(-vViewLightDir, _vViewNormal) * _vViewNormal;
    vReflect = normalize(vReflect);
    
    // 카메라가 물체를 향하는 방향
    float3 vEye = normalize(_vViewPos);
    
    // 시선벡터와 반사벡터 내적, 반사광의 세기
    float ReflectPow = saturate(dot(-vEye, vReflect));
    ReflectPow = pow(ReflectPow, 20.f);

    _LightColor.vColor += Light.Color.vColor * LightPow * fDistanceRatio;
    _LightColor.vAmbient += Light.Color.vAmbient;
    _LightColor.vSpecular += Light.Color.vColor * Light.Color.vSpecular * ReflectPow * fDistanceRatio;
}

// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_time * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_time /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)].xyz * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}


int IntersectsLay(float3 _vertices[3], float3 _vStart, float3 _vDir, out float3 _vCrossPoint, out float _fResult)
{
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;

    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _vDir);

    float3 w0 = _vStart - _vertices[0].xyz;
    float a = -dot(normal, w0);
    float t = a / b;

    _fResult = t;

    float3 p = _vStart + t * _vDir;

    _vCrossPoint = p;

    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }

    return 1;
}


float GetTessFactor(float _Length, int _iMinLevel, int _iMaxLevel, float _MinDistance, float _MaxDistance)
{
    if (_MaxDistance < _Length)
    {
        return 0.f;
    }
    else if (_Length < _MinDistance)
    {
        return _iMaxLevel;
    }
    else
    {
        float fLevel = _iMaxLevel - (_iMaxLevel - _iMinLevel) * ((_Length - _MinDistance) / (_MaxDistance - _MinDistance));

        return fLevel;
    }
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

#endif