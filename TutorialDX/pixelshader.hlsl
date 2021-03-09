struct PS_INPUT
{
    float3 inPos : SV_POSITION;
    float4 inColor : COLOR;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    return float4(input.inColor);
}