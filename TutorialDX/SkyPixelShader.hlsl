struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inLocalPos : POSITION;
};

//Texture2D objTexture : TEXTURE : register(t0);
TextureCube gCubeMap : register(t0);
SamplerState sam : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    uint width, height;
    gCubeMap.GetDimensions(width, height);
    
    if (width > 10)
    {
        //return float4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    
    return gCubeMap.Sample(sam, input.inLocalPos);
}
