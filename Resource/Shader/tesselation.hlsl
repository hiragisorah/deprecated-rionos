Texture2D g_HeightTexture : register(t0);

SamplerState g_samPoint : register(s0);

struct VS_OUTPUT
{
    float3 position_ : POSITION;
    float2 uv_: TEXCOORD0;
};

struct HS_CONSTANT_OUTPUT
{
    float factor[4] : SV_TessFactor;
    float inner_factor[2] : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 position_ : POSITION;
    float2 uv_: TEXCOORD0;
};

struct DS_OUTPUT
{
    float4 position_ : SV_POSITION;
    float2 uv_: TEXCOORD0;
};

//
//
//
VS_OUTPUT VS(float3 position_ : POSITION, float2 uv_: TEXCOORD0)
{
    VS_OUTPUT output;

    output.position_ = position_;
    output.uv_ = uv_;

    return output;
}
//
//
//
HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 4> ip, uint pid : SV_PrimitiveID)
{
    HS_CONSTANT_OUTPUT Out;
    float divide = 64;

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
    Out.position_ = ip[cpid].position_;
    Out.uv_= ip[cpid].uv_;

    return Out;
}
//
//
//
[domain("quad")]
DS_OUTPUT DS(HS_CONSTANT_OUTPUT In, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 4> patch)
{
    //matrix wvp = mul(g_world, mul(g_view, g_proj));

    DS_OUTPUT Out;

    float2 top_uv_= lerp(patch[0].uv_, patch[1].uv_, UV.x);
    float2 bottom_uv_= lerp(patch[3].uv_, patch[2].uv_, UV.x);
    float2 uv_= float2(lerp(top_uv_, bottom_uv_, UV.y));
    Out.uv_= uv_;

    float4 height = g_HeightTexture.SampleLevel(g_samPoint, uv_, 0) / 4;

    float3 top_position_ = lerp(patch[0].position_, patch[1].position_, UV.x);
    float3 bottom_position_ = lerp(patch[3].position_, patch[2].position_, UV.x);
    Out.position_ = float4(lerp(top_position_, bottom_position_, UV.y), 1);
    Out.position_.y += height.x;
    //Out.position_ = mul(Out.position_, wvp);

    return Out;
}
//
//
//
float4 PS(DS_OUTPUT In) : SV_Target
{
    float4 col = g_HeightTexture.SampleLevel(g_samPoint, In.uv_, 0);
    return col;
}

