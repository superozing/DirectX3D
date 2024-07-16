#ifndef DOWNSCALE
#define DOWNSCALE

#define INTERPOL_LEVEL 2

Texture2D<float4> inputTexture : register(t0); // ū ������ �ؽ���
RWTexture2D<float4> outputTexture : register(u0); // ���� ������ �ؽ���

[numthreads(32, 32, 1)]
void CS_DownScale(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint inputWidth, inputHeight;
    inputTexture.GetDimensions(inputWidth, inputHeight);

    uint outputWidth, outputHeight;
    outputTexture.GetDimensions(outputWidth, outputHeight);
    
    if (outputWidth <= dispatchThreadID.x || outputHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    // �Է� �ؽ�ó���� �ȼ� ���� �о�� ��ġ ���
    float2 inputUV = float2(dispatchThreadID.xy) / float2(outputWidth, outputHeight);
    float2 Location = inputUV * float2(inputWidth, inputHeight);

    // �Է� �ؽ�ó���� ���ø��Ͽ� ���ο� �ؽ�ó�� ����
    float4 color = inputTexture.Load(int3(Location.x, Location.y, 0));
    
    color += inputTexture.Load(int3(round(Location.x - 1), round(Location.y - 1), 0));
    color += inputTexture.Load(int3(round(Location.x - 1), round(Location.y + 1), 0));
    color += inputTexture.Load(int3(round(Location.x + 1), round(Location.y - 1), 0));
    color += inputTexture.Load(int3(round(Location.x + 1), round(Location.y + 1), 0));
    
    color += inputTexture.Load(int3(round(Location.x + 1), round(Location.y), 0));
    color += inputTexture.Load(int3(round(Location.x - 1), round(Location.y), 0));
    color += inputTexture.Load(int3(round(Location.x), round(Location.y - 1), 0));
    color += inputTexture.Load(int3(round(Location.x), round(Location.y + 1), 0));
    
    color /= 9;
    outputTexture[dispatchThreadID.xy] = color;
}
#endif