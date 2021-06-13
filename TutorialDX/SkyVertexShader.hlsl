#pragma pack_matrix( row_major )
cbuffer perObjectBuffer : register(b0)
{
    float4x4 wvpMatrix;
    float4x4 worldMatrix;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT vout;
	
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.outPosition = mul(float4(input.inPos, 1.0f), wvpMatrix).xyww;
	
	// Use local vertex position as cubemap lookup vector.
    vout.outWorldPos = input.inPos;
	
    return vout;
}
