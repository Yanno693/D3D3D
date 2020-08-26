struct VSOut
{
	float3 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : Position, float3 col : Color)
{
    VSOut vsout;
    vsout.pos = float4(pos.xy, 0.0f, 1.0f);
    vsout.color = col.xyz;
	
	return vsout;
}