
Texture2D		g_txDiffuseA : register(t0);
Texture2D		g_txDiffuseB : register(t1);
SamplerState	g_Sampler0	:	register(s0);



struct PS_IN
{
	float4 p : SV_POSITION;
	float4 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	//constantData
	float4 cb : TEXCOORD1;
};
struct PS_IN_LINE
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
struct PS_OUT
{
	float4 c : SV_TARGET;
};

struct PS_OUT1
{
	float4 add : SV_TARGET0;
	float4 mul : SV_TARGET1;
};

PS_OUT1 PS(PS_IN inData)
{
	PS_OUT1 vOut;
	
	float4 vFinalA = g_txDiffuseA.Sample(g_Sampler0, inData.t);
	float alpha = max(max(vFinalA.r, vFinalA.g), vFinalA.b);
	vOut.mul = float4(1 - alpha, 1 - alpha, 1 - alpha, alpha);
	vOut.add = vFinalA;//lerp(vFinalA, vFinalB, time.x);

	return vOut;
}


struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	//constantData
	float4 cb : TEXCOORD1;
};
struct VS_OUT_LINE
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
//struct VS_IN
//{
//	float4 p : pos;
//	float4 n : normal;
//	float4 c : color;
//	float2 t : tx;
//};
//kgca_Engine use
struct VS_IN
{
	float4 p : POINT;
	float4 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXT;
};
struct VS_IN_LINE
{
	float4 p : pos;
	float4 c : color;
};
//constant buff 0
cbuffer cb_buffer:register(b0)
{
	//월드, 뷰, 투영 행렬
	float4x4 g_matWorld : packoffset(c0);
	float4x4 g_matView : packoffset(c4);
	float4x4 g_matProj : packoffset(c8);
	//time.x
	float4  time : packoffset(c12);
};

VS_OUT VS(VS_IN inData)
{
	VS_OUT vOut;
	//월드, 뷰, 투영 행렬을 모두 연산한 버텍스를 
	//float4 vLocal = float4(inData.p,1.0f);
	
	float4 vLocal = inData.p;
	vLocal.w = 1.0f;
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.n = inData.n;
	//inData.p * 10;
	vOut.c = inData.c;
	vOut.t = inData.t;
	vOut.cb = time;
	return vOut;
}

VS_OUT_LINE VSLine(VS_IN_LINE inData)
{
	VS_OUT_LINE vOut;
	//월드, 뷰, 투영 행렬을 모두 연산한 버텍스를 
	//float4 vLocal = float4(inData.p,1.0f);
	float4 vLocal = inData.p;
	vLocal.w = 1.0f;
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.c = inData.c;
	return vOut;
}