cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float dynamicLightStrength;
    float3 dynamicLightPosition;
        
    float mDynamicLightAttenuation_a;
    float mDynamicLightAttenuation_b;
    float mDynamicLightAttenuation_c;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    //float3 sampleColor = input.inNormal;
    
    float3 ambientColor = ambientLightColor * ambientLightStrength;
    
    float3 appliedLight = ambientColor;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    
    float3 diffuseLightIntensity = max(0, dot(vectorToLight, input.inNormal));
    
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
    float attenuationFactor = 1 / (mDynamicLightAttenuation_a + mDynamicLightAttenuation_b
        * distanceToLight + mDynamicLightAttenuation_c * pow(distanceToLight,2));
    diffuseLightIntensity *= attenuationFactor;
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
    appliedLight += diffuseLight;
    
    float3 finalColor = sampleColor * saturate(appliedLight);
    
    return float4(finalColor, 1.0f);
}