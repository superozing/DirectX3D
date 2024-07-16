#ifndef BLURX
#define BLURX

Texture2D<float4> inputTexture : register(t0);
RWTexture2D<float4> outputTexture : register(u0);

static const float weight[5] = { 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };

[numthreads(32, 32, 1)]
void CS_BlurX(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint outputWidth, outputHeight;
    outputTexture.GetDimensions(outputWidth, outputHeight);
    
    if (outputWidth <= dispatchThreadID.x || outputHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    
    float3 vColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < 5; ++i)
    {
        int2 pos = int2(dispatchThreadID.x + i - 2, dispatchThreadID.y);
        
        if (pos.x < 0)
        {
            pos.x = 0;
        }
        
        if (pos.x >= outputWidth)
        {
            pos.x = outputWidth - 1;
        }
        
        vColor += saturate(inputTexture[pos].rgb * weight[i]);
    }
    
    outputTexture[dispatchThreadID.xy] = float4(vColor, 1.f);
}

#endif