TextureCube			g_txCubeMap						: register(t3);
SamplerState        g_samPointClamp : register(s0);
SamplerState        g_samLinearWrap : register(s1);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};
struct VS_INPUT
{
	float3 p : POINT;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXT;

};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
	uint PrimitiveID : SV_PrimitiveID;
};
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.p = mul(float4(vIn.p,1.0f), g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.p = output.p.xyww;
	output.n = vIn.n;
	output.t = vIn.t;
	output.r = normalize(vIn.p.xyz);
	output.c = vIn.c;
	return output;
}

float4 PS_Cube(PS_INPUT vIn) : SV_Target
{
	float4 fColor = float4(1, 1, 1, 1);
	fColor = g_txCubeMap.Sample(g_samPointClamp, vIn.r);
	return fColor;
}