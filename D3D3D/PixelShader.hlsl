#define inf 100000.0f

float _triangle ( float3 o, float3 d, float3 a, float3 ab, float3 ac) {    
    float res = inf;

    float3 pvec = cross(d, ac);
    float det = dot(ab, pvec);
    float inv_det = 1/det;
    
    float3 uvec = o - a;
    float u = dot(uvec, pvec) * inv_det;
    
    if(u < 0.0f || u > 1.0f)
        return res;

    float3 vvec = cross(uvec, ab);
    float v = dot(d, vvec) * inv_det;
    if(v < 0.0f || u + v > 1.0f)
        return res;

    float t = dot(ac, vvec) * inv_det;

    if(t > 0 && t < inf) {
        res = t;
    }

    return res;
}

cbuffer cbMatrix : register(b1)
{
    float4x4 m;
    int w;
    int h;
    //int h2;
    //int h3;
};

float4 main(float3 color : Color, float4 pos : SV_Position, float2 screen_pos : C_Position) : SV_TARGET
{
    float tdist = 5.0f;
    
    float3 triangleList[3];
    triangleList[0] = float3(0.0f, 1.0f, tdist);
    triangleList[1] = float3(-1.0f, 0.0f, tdist);
    triangleList[2] = float3(1.0f, 0.0f, tdist);
    
    //return float4(1.0f, 1.0f, 0.0f, 1.0f);

    float4 o = mul(float4(screen_pos, 0.f, 1.0f), m);
    float4 e = mul(float4(screen_pos, 1.f, 1.0f), m);
    float3 _o = o.xyz / o.w;
    float3 _e = e.xyz / e.w;
    
    float touche = _triangle(
        _o, 
        normalize(_e - _o), 
        triangleList[0], 
        triangleList[2] - triangleList[0], 
        triangleList[1] - triangleList[0]);
    
    //float4 ret = rep; 
    float4 ret = float4(color, 1.0f);
    //float4 ret = float4(pos.xy / 256.0f, 0.0f, 1.0f);
    //float4 ret = float4(int(pos.y) % 2, int(pos.y) % 2, 0.0f, 1.0f);
    //float4 ret = float4(1, 1, 1, 1);
    
    
    if (touche != inf)
        ret = 1;
    
    return ret;
}