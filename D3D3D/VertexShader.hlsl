cbuffer cbMatrix : register(b0)
{
    float4x4 m[2];
};

struct VSOut
{
	float3 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 col : Color)
{
    VSOut vsout;
    //vsout.pos = float4(pos.xy, 0.0f, 1.1f);
    vsout.pos = mul(float4(pos, 1.0f), m[1]);
    vsout.color = col.xyz;
	
	return vsout;
}