    #ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

StructuredBuffer<tParticleModule> g_Module : register(t20);
RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_SpawnCount : register(u1);

#define MAX_COUNT   g_int_0 
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]
#define CenterPos   g_vec4_0.xyz


[numthreads(32, 1, 1)]
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= id.x)
        return;
    
    // ��ƼŬ�� ��Ȱ��ȭ ���¶��
    if (0 == Particle.Active)
    {
        // ���� ��� Ȱ��ȭ üũ
        if (0 == Module.arrModuleCheck[0])
            return;
        
        while (0 < SpawnCount)
        {
            // Atomic �Լ� 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
           
            // InterlockedExchange �Լ��� �Ἥ SpawnCount �� ��ü, �����ϸ�
            // �ʱ� �õ��� ���尡 ���� �����尡 ������ ���Ŀ� �����ϴ� ��찡 �ִ�. 
            // �̶� SpawnCount �� ������ �÷������� ������ �߻��� �� �ִ�. 
            // InterlockedCompareExchange �� ���ؼ� ������ ���� ��ġ�� ��쿡�� 
            // ��ü�� �ϵ��� �ϴ� �Լ��� ����Ѵ�.
            InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            //InterlockedExchange(SpawnCount, Exchange, Origin);
            
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                Particle.vNoiseForce = (float3) 0.f;
                Particle.NoiseForceTime = 0.f;
                
                // ����
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                vUV.x += g_time * 0.2f;
                //                 ( ���ļ� )    (����)  (V �� offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0);
                
                // SpawnShape �� Sphere Ÿ���̶��
                if (0 == Module.SpawnShape)
                {
                    //float RandomRadius = vRand[0] * Module.Radius;
                    //float RandomAngle = vRand[1] * 2 * PI;
                    float RandomRadius = sqrt(vRand[0]) * Module.Radius;
                    float RandomTheta = vRand[1] * 2 * PI;
                    float RandomPhi = acos(2 * vRand[2] - 1);
                   
                    // Particle ������Ʈ(��ü) �� �߽���ġ(����) ����
                    // ���� ����, ���� �������� �ش��ϴ� ��ġ�� ����ؼ� ��ƼŬ�� �ʱ� ��ġ�� �ش�.
                    Particle.vLocalPos.xyz = float3(sin(RandomPhi) * cos(RandomTheta),
                    sin(RandomPhi) * sin(RandomTheta),
                    cos(RandomPhi)) * RandomRadius;
                }
                else
                {
                    Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale.x - (Module.vSpawnBoxScale.x / 2.f);
                    Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale.y - (Module.vSpawnBoxScale.y / 2.f);
                    Particle.vLocalPos.z = vRand[2] * Module.vSpawnBoxScale.z - (Module.vSpawnBoxScale.z / 2.f);
                }
                
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // ���� �÷� ����
                Particle.vColor = Module.vSpawnColor;
                
                // ���� ũ�� ����                
                Particle.vWorldInitScale = Particle.vWorldScale = (Module.vSpawnMaxScale - Module.vSpawnMinScale) * vRand[2] + Module.vSpawnMinScale;
                                
                // ���� Life ����
                Particle.Age = 0.f;
                Particle.Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MinLife;
                      
                // ���� Mass ����
                Particle.Mass = clamp(vRand1[0], Module.MinMass, Module.MaxMass);
                              
                // �ʱ� �ִϸ��̼� ������ ����
                Particle.CurFrame = 0;
                
                // Add VelocityModule
                if (Module.arrModuleCheck[3])
                {
                    // 0 : From Center
                    if (0 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                    if (1 == Module.AddVelocityType)
                    {
                        float3 vDir = -normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                    if (2 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Module.FixedDirection);
                        
                        float cosAngle = cos(radians(Module.FixedAngle) * 0.5);
                        float sinAngle = sqrt(1.0 - cosAngle * cosAngle);

                        float3 randDir = float3((vRand[0] * 2.0f - 1.0f) * sinAngle,
                                                (vRand[1] * 2.0f - 1.0f) * sinAngle, cosAngle);
                        
                        float3 tangent = normalize(cross(float3(0.0f, 1.0f, 0.0f), vDir));
                        float3 bitangent = cross(vDir, tangent);
                        float3 newDir = randDir.z * vDir + randDir.x * tangent + randDir.y * bitangent;
                        
                        Particle.vVelocity.xyz = normalize(newDir) * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                        
                        //Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }

                }
                else
                {
                     Particle.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                }
                
                break;
            }
        }
    }
    
    // ��ƼŬ�� Ȱ��ȭ ���¶��
    else
    {
        Particle.Age += g_EngineDT;
        if (Particle.Life < Particle.Age)
        {
            Particle.Active = 0;
            return;
        }
        
        // ������ ����
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        float3 Rand;
        GaussianSample(g_NoiseTex, g_NoiseTexResolution, vUV.x, Rand);
        
        //vUV.x += g_time * 0.2f;
        //vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
        //float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
        
        
        // �̹� �����ӿ��� �޴� ���� �ʱ�ȭ
        Particle.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        // Normalize Age ���
        Particle.NomalizedAge = Particle.Age / Particle.Life;
        
        // �ִϸ��̼� ������ ������Ʈ
        int totalFrame = Module.TileX * Module.TileY;
        float frameDuration = Particle.Life / totalFrame;
        Particle.CurFrame = int(Particle.Age / frameDuration) % totalFrame;
        
        // Scale ���
        if (Module.arrModuleCheck[2])
        {
            Particle.vWorldScale = Particle.vWorldInitScale * (1.f + (Module.vScaleRatio - 1.f) * Particle.NomalizedAge);
        }
        
        // Noise Force
        if (Module.arrModuleCheck[4])
        {
            if (Particle.NoiseForceTime == 0.f)
            {
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                Particle.NoiseForceTime = g_time;
            }
            else if (Module.NoiseForceTerm < g_time - Particle.NoiseForceTime)
            {
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                Particle.NoiseForceTime = g_time;
            }
        }
                
     
        
        // Calculate Force
        if (Module.arrModuleCheck[5])
        {
            Particle.vForce.xyz += Particle.vNoiseForce.xyz;
            
            // Force ����
            // F = M x A
            float3 vAccel = Particle.vForce.xyz / Particle.Mass;
            
            // Accel ����
            Particle.vVelocity.xyz += vAccel * g_EngineDT;
            
            // Drag ����� ����������
            if (Module.arrModuleCheck[1])
            {
                float LimitTime = Module.DragTime - Particle.Age;
            
                if (LimitTime <= 0.f)
                {
                    Particle.vVelocity = 0.f;
                }
                else
                {
                    float DT = g_dt / LimitTime;
                    Particle.vVelocity -= Particle.vVelocity * DT;
                }
            }
            
            // Velocity �� ���� ��ġ�̵� ����
            if (0 == Module.SpaceType)
            {
                Particle.vLocalPos.xyz += Particle.vVelocity.xyz * g_EngineDT;
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
            }
            else if (1 == Module.SpaceType)
            {
                Particle.vWorldPos.xyz += Particle.vVelocity.xyz * g_EngineDT;
            }
        }
    }
}

#endif