cbuffer alphaBuffer : register(b0)
{
    float alpha;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float4 coord : COLOR;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 pixelColor = input.coord;
    return pixelColor;

}