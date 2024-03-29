Texture2D g_HeightTexture : register(t0);
Texture2D g_NormalTexture : register(t1);

SamplerState g_samPoint : register(s0);

cbuffer unique : register(b0)
{
    row_major matrix g_world;
    row_major matrix g_view;
    row_major matrix g_proj;
};

struct VS_OUTPUT
{
    float3 pos : POSITION0;
    float3 pos_ : POSITION1;
    float2 uv : TEXCOORD0;
};

struct HS_CONSTANT_OUTPUT
{
    float factor[4] : SV_TessFactor;
    float inner_factor[2] : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct DS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float LdotN : TEXCOORD1;
};

//
//
//
VS_OUTPUT VS(float3 pos : POSITION, float2 uv : TEXCOORD0)
{
    matrix wvp = mul(g_world, mul(g_view, g_proj));

    VS_OUTPUT Out;
    Out.pos = pos;
    Out.pos_ = mul(pos, (float3x3) wvp);
    Out.uv = uv;

    return Out;
}
//
//
//
HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 4> ip, uint pid : SV_PrimitiveID)
{
    HS_CONSTANT_OUTPUT Out;
    float divide = 1280;

    Out.factor[0] = divide;
    Out.factor[1] = divide;
    Out.factor[2] = divide;
    Out.factor[3] = divide;

    Out.inner_factor[0] = divide; //u 縦の分割数（横のラインを何本ひくか）
    Out.inner_factor[1] = divide; //v

    return Out;
}
//
//
//
[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HSConstant")]
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 4> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)
{
    HS_OUTPUT Out;
    Out.pos = ip[cpid].pos;
    Out.uv = ip[cpid].uv;

    return Out;
}
//
//
//
[domain("quad")]
DS_OUTPUT DS(HS_CONSTANT_OUTPUT In, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 4> patch)
{
    DS_OUTPUT Out;

    matrix wvp = mul(g_world, mul(g_view, g_proj));

    float2 top_uv = lerp(patch[0].uv, patch[1].uv, UV.x);
    float2 bottom_uv = lerp(patch[3].uv, patch[2].uv, UV.x);
    float2 uv = float2(lerp(top_uv, bottom_uv, UV.y));
    Out.uv = uv;

    float4 height = g_HeightTexture.SampleLevel(g_samPoint, uv, 0) / 4;

    float3 top_pos = lerp(patch[0].pos, patch[1].pos, UV.x);
    float3 bottom_pos = lerp(patch[3].pos, patch[2].pos, UV.x);
    Out.pos = float4(lerp(top_pos, bottom_pos, UV.y), 1);
    Out.pos.y += height.x;
    Out.pos = mul(Out.pos, wvp);

    float4 normal = normalize(g_NormalTexture.SampleLevel(g_samPoint, uv, 0) * 2 - 1);

    normal.w = 0;
    normal = mul(normal, g_world);

    Out.LdotN = dot(normal, float4(1, 0, 1, 1));

    return Out;
}
//
//
//
float4 PS(DS_OUTPUT In) : SV_Target
{
    float4 col = g_HeightTexture.SampleLevel(g_samPoint, In.uv, 0);

    col.rgb *= In.LdotN;

    return col;
}