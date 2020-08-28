#define inf 10000.0f

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

    if(t > 0) {
        res = t;
    }

    return res;
}

struct ConstantStruct
{
    float a, b, c;
};

cbuffer cb : register(b0)
{
    ConstantStruct cs[2];
};

cbuffer cbMatrix : register(b1)
{
    float4x4 m;
};

float4 main(float3 color : Color, float4 pos : SV_Position) : SV_TARGET
{
    float3 triangleList[3];
    triangleList[0] = float3(0, 1, 5);
    triangleList[1] = float3(-1, 0, 5);
    triangleList[2] = float3(1, 0, 5);
    
    //return float4(1.0f, 1.0f, 0.0f, 1.0f);
    
    float rep = 0.0f;
    
    float4 t1 = pos;
    float4 t2 = mul(m, pos);
    
    if (t1.x == t2.x && t1.y == t2.y && t1.z == t2.z && t1.w == t2.w)
    {
        rep = 1.0f;
    }
    
    //float4 ret = rep;
    float4 ret = float4(color, 1.0f);
    //float4 ret = float4(pos.xy / 256.0f, 0.0f, 1.0f);
    //float4 ret = float4(int(pos.y) % 2, int(pos.y) % 2, 0.0f, 1.0f);
    //float4 ret = float4(1, 1, 1, 1);
    
    //if(pos.x > 320.0f)
        //ret.y = 1;
    
    return ret;
}