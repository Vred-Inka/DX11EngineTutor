

cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};


struct VS_INPUT
{
    float3 inPos : POSITION;
    float4 inColor : COLOR;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float4 outColor : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT vout;
	
	// Transform to homogeneous clip space.
    vout.outPosition = mul(float4(input.inPos, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.outColor = input.inColor;
    
    return vout;
}


technique11 ColorTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
      //  SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}