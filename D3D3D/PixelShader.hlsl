#define inf 100000.0f

float2 conv(float2 _in)
{
    return float2(_in.x / 320.0f - 1.0f, _in.y / 240.0f - 1.0f);
}

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
    float4x4 m[2];
};

float4 main(float3 color : Color, float4 pos : SV_Position) : SV_TARGET
{
    float tdist = 20.0f;
    
    float3 triangleList[3];
    triangleList[0] = float3(0, 1, tdist);
    triangleList[1] = float3(-1, 0, tdist);
    triangleList[2] = float3(1, 0, tdist);
    
    //return float4(1.0f, 1.0f, 0.0f, 1.0f);
    
    float rep = 0.0f;
    
    float2 npos = conv(pos.xy);

    float4 o = mul(float4(npos, 0.f, 1), m[0]);
    float4 e = mul(float4(npos, 1.f, 1), m[0]);
    float3 _o = o.xyz / o.w;
    float3 _e = e.xyz / e.w;
    //float4 o = mul(m[0], float4(pos.xy, 0, 1));
    //float4 e = mul(m[0], float4(pos.xy, 1, 1));
    
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
    
    float2 mconv = conv(pos.xy);
    //if(pos.x > 320.0f)
        //ret.y = 1;
    
    if (touche != inf)
    //if (_o.z == 0.0f)
    //if (_e.z > 9.99f && _e.z < 10.01f)
    //if (npos.y < 0.1f && npos.y > -0.1f)
        ret = 1;
    
    return ret;
}