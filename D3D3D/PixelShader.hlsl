float4 main(float3 color : Color, float4 pos : SV_Position) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
    
    
    float4 ret = float4(color.xyz, 1.0f);
    //float4 ret = float4(1, 1, 1, 1);
    
    //if(pos.x > 320.0f)
        //ret.y = 1;
    
    return ret;
}