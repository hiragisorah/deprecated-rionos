//グローバル

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
    row_major matrix g_W : packoffset(c0); //ワールドから射影までの変換行列
    float2 g_ViewPort : packoffset(c4); //ビューポート（スクリーン）サイズ
};
//構造体
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

//
//
//バーテックスシェーダー
PS_INPUT VS(float3 Pos : POSITION, float2 UV : TEXCOORD)
{
    PS_INPUT Out;

    Out.Pos = mul(float4(Pos, 1.f), g_W);

    Out.Pos.x = (Out.Pos.x / g_ViewPort.x) * 2;
    Out.Pos.y = (Out.Pos.y / g_ViewPort.y) * 2;

    Out.UV = UV;

    return Out;
}
//
//
//ピクセルシェーダー
float4 PS(PS_INPUT Input) : SV_Target
{
    float4 color = g_Texture.Sample(g_Sampler, Input.UV);

    return color;
}