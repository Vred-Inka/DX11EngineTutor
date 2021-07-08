struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float IsEnable;
};
struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Att;
    float IsEnable;
};
struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Direction;
    float Spot;
    float3 Att;
    float IsEnable;
};

struct Material
{   
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Reflect;
    float pad;
};

struct SimpleLight
{
    float3 dir;
    float pad;
    float4 ambient;
    float4 diffuse;
};

cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;

    float3 dynamicLightColor;
    float dynamicLightStrength;
    
    float3 dynamicLightPosition;    
    float dynamicLightAttenuation_a;
    
    float3 dirLightAmbient;
    float dynamicLightAttenuation_b;    
    
    float3 dirlightDiffuse;
    float dynamicLightAttenuation_c;    
    
    SimpleLight simpleLight;
    Material gMaterial;
    
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    float3 gEyePosW;
    float pad;
     
}

cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
    float4x4 gWorld;
}

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

void ComputeDirectionalLight(Material mat, DirectionalLight L,float3 normal, float3 toEye,
out float4 ambient,out float4 diffuse,out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;
// Add ambient term.
    ambient = mat.Ambient * L.Ambient;
// Add diffuse and specular term, provided the surface is in
// the line of site of the light.
    float diffuseFactor = dot(lightVec, normal);
// Flatten to avoid dynamic branching.
[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
    
}

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
out float4 ambient, out float4 diffuse, out float4 spec)
{
// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f); // The vector from the surface to the light.
    float3 lightVec = L.Position - pos;
// The distance from surface to light.
    float d = length(lightVec);
// Range test.
    if (d > L.Range)
        return;
// Normalize the light vector.
    lightVec /= d;
// Ambient term.
    ambient = mat.Ambient * L.Ambient;
// Add diffuse and specular term, provided the surface is in
// the line of site of the light.
float diffuseFactor = dot(lightVec, normal);
// Flatten to avoid dynamic branching.
[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    } 
    //    Attenuate

    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
    diffuse *= att;
    spec *= att;
}

void ComputeSpotLight(Material mat, SpotLight L,float3 pos, float3 normal, float3 toEye,
out float4 ambient, out float4 diffuse, out float4 spec)
{
// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
// The vector from the surface to the light.
    float3 lightVec = L.Position - pos;
// The distance from surface to light.
    float d = length(lightVec);
// Range test.if( d > L.Range )
    return;
// Normalize the light vector.
    lightVec /= d;
// Ambient term.
    ambient = mat.Ambient * L.Ambient;
// Add diffuse and specular term, provided the surface is in
// the line of site of the light.
    float diffuseFactor = dot(lightVec, normal);
// Flatten to avoid dynamic branching.
[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    } 
    //   Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
// Scale by spotlight factor and attenuate.
    float att = spot / dot(L.Att, float3(1.0f, d,d * d));
    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

float4 main(PS_INPUT pin) : SV_TARGET
{
   // Interpolating normal can unnormalize it, so normalize it.
    pin.inNormal = normalize(pin.inNormal);
    float3 toEyeW = normalize(gEyePosW - pin.inWorldPos);
// Start with a sum of zero.
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
// Sum the light contribution from each light source.
    float4 A, D, S;
    
    if (gDirLight.IsEnable)
    {
        ComputeDirectionalLight(gMaterial, gDirLight, pin.inNormal, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    
    if (gPointLight.IsEnable)
    {
        ComputePointLight(gMaterial, gPointLight, pin.inWorldPos, pin.inNormal, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    
    if (gSpotLight.IsEnable)
    {
        float4 ASpot; 
        ComputeSpotLight(gMaterial, gSpotLight, pin.inWorldPos, pin.inNormal, toEyeW, ASpot, D, S);
        
        ambient += A;
        diffuse += D;
        spec += S;
    }
    float4 litColor = ambient + diffuse + spec;
// Common to take alpha from diffuse material.
    litColor.a = gMaterial.Diffuse.a;
    return litColor;
    
}

/*
float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 sampleColor = input.inNormal;

    float3 ambientLight = ambientLightColor * ambientLightStrength;

    float3 appliedLight = ambientLight;

    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);

    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);
    
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);

    float attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));

    diffuseLightIntensity *= attenuationFactor;

    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;

    appliedLight += diffuseLight;

    float3 finalColor = sampleColor * appliedLight;

    return float4(finalColor, 1.0f);
}
*/

