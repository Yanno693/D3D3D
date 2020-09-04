struct VSOut
{
	float3 color : Color;
    float4 pos : SV_Position;
    float2 screen_pos : C_Position;
};

VSOut main(float2 pos : Position)
{
    VSOut vsout;
    vsout.pos = float4(pos, 0.0f, 1.0f);
    vsout.screen_pos = pos;
    vsout.color = float3(0, 0, 0.1f);
	
	return vsout;
}