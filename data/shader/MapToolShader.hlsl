#define MAX_TEXTUR 3

struct BufType
{
	int i;
	float f;
};

struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORDO;
	float3 world : TEXCOORD1;
	float time : TEXCOORD2;
};

struct VS_IN
{
	float3 p : POINT;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXT;
};

cbuffer m_ConData : register(b0)
{
	float4x4 g_matWorld : packoffset(c0);
	float4x4 g_matView : packoffset(c4);
	float4x4 g_matProj : packoffset(c8);
	float4 time : packoffset(c12);
	uint index : packoffset(c16);

};

cbuffer m_Light : register(b1)
{
	float4 g_cAmbientLightColor;
	float4 g_cDifuseLightColor;
	float4 g_cSpecularLightColor;
	float4 g_vLightDir;	//감쇠
	float4 g_vLightPos;	//반지름
	float4 g_vEyeDir;	//w 강도
	float4 g_vEyePos;	//반지름
	float4 g_vSpot;
};

float4 Specular(float3 vNormal)
{
	float3 vReflect = reflect(g_vLightDir.xyz, vNormal);
	float fdot = max(0, dot(vReflect, normalize(-g_vEyeDir.xyz)));
	float fPower = pow(fdot, g_vEyeDir.w);
	float4 Color = g_cAmbientLightColor + fPower * g_cSpecularLightColor;
	Color.w = 1.0f;
	return Color;
}

//vPos = world
float4 ComputePointLight(float3 vPos, float3 vNormal)
{
	float Distance = distance(vPos, g_vLightPos.xyz);
	float4 fDiffuse = g_cAmbientLightColor;

	// (min)a~b(max), t의 값
	float offset = smoothstep(Distance - 30, Distance, g_vLightPos.w);
	float fdot = max(0, dot(vNormal, normalize(-g_vLightDir.xyz)));
	fDiffuse = g_cAmbientLightColor + (offset * fdot * g_cDifuseLightColor);
	fDiffuse.w = 1.0f;

	return fDiffuse;

}

float4 ComputeSpotLight(float3 vPos, float3 vNormal)
{
	float Distance = distance(vPos, g_vLightPos.xyz);
	float4 fDiffuse = g_cDifuseLightColor;

	float fInner = g_vSpot.x;
	float fOutner = g_vSpot.y;
	float fFalloff = g_vSpot.z;
	float fRange = g_vLightPos.w;

	float3 vLight = normalize(vPos - g_vLightPos.xyz);
	//float Intensity = max(0, dot(vNormal, normalize(-g_vLightDir.xyz)));
	//float fDot = saturate(dot(g_vLightDir.xyz, vNormal));
	float fDot = saturate(dot(g_vLightDir.xyz, vLight));
	float fOffset = smoothstep(Distance - fFalloff, Distance, fRange);
	//float fOffset =smoothstep( g_vLightPos.w - fFalloff , g_vLightPos.w , fRange);
	if (fDot > fInner)
	{

		return fDiffuse * fOffset;
	}
	else if (fDot > fOutner)
	{
		float fPhi = fOutner - fInner;
		float fValue = 1.0f - ((fDot - fInner) / fPhi);
		return fDiffuse * fValue * fOffset;
	}
	else
	{
		return float4(0, 0, 0, 1);
	}

	/*// (min)a~b(max), t의 값
	float offset = smoothstep( Distance-30 , Distance , g_vLightPos.w);
	float fIntensity = max(0, dot(vNormal, normalize(-g_vLightDir.xyz)));
	fDiffuse = g_cAmbientLightColor + (offset * fIntensity * g_cDifuseLightColor);
	fDiffuse.w = 1.0f;


	return fDiffuse;*/

}

float4 Diffuse(float3 vNormal)
{
	float fdot = max(0, dot(vNormal, normalize(-g_vLightDir.xyz)));
	float4 fDiffuse = g_cAmbientLightColor + fdot * g_cDifuseLightColor;
	fDiffuse.w = 1.0f;
	return fDiffuse;
}

VS_OUT VS(VS_IN data)
{
	VS_OUT vout;
	vout.p = float4(data.p, 1.0f);
	vout.n = data.n;
	vout.c = data.c;
	vout.t = data.t;
	return vout;
}

VS_OUT VSmat(VS_IN data)
{
	VS_OUT vout;
	float4 vLocal = float4(data.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vout.p = vProj;
	vout.n = data.n;
	vout.world = vWorld.xyz;
	//vout.world.x = vWorld.x;
	//vout.world.y = vWorld.y;
	//vout.world.z = vWorld.z;
	vout.c = Diffuse(vout.n); //ComputePointLight(vWorld.xyz, vout.n);//+Specular(vout.n); //
	vout.t = data.t;
	return vout;
}

VS_OUT VSTile(VS_IN data)
{
	VS_OUT vout;
	float4 vLocal = float4(data.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vout.p = vProj;
	vout.n = data.n;
	vout.world = vWorld.xyz;
	vout.c = Diffuse(vout.n); //ComputePointLight(vWorld.xyz, vout.n);//+Specular(vout.n); 
	//vout.t = data.t*10;
	vout.t.x = (1.0f / (time.x) * vWorld.x) * time.x;			//vWorld.x / (time.x) * time.x;
	vout.t.y = -(1.0f / (time.z) * vWorld.z) * time.z;		//		//vWorld.z / (time.z) * time.z;
	//vout.time = index;
	return vout;
}



Texture2D		g_txDiffuseA : register(t0);
StructuredBuffer<BufType> StructBuf : register(t1);
Texture2D		g_txAlpha : register(t2);
Texture2D		g_txDiffuseC[MAX_TEXTUR] : register(t3);
SamplerState	g_Sample0 : register(s0);

struct PS_IN
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORDO;
	float3 world : TEXCOORD1;
	float time : TEXCOORD2;
};

struct PS_OUT
{
	float4 add : SV_TARGET0;
	float4 mul : SV_TARGET1;
};-
PS_OUT PS(PS_IN vsdata)
{
	PS_OUT pout;
	float4 FinalA = g_txDiffuseA.Sample(g_Sample0, vsdata.t);

	pout.add = FinalA;
	return pout;
}

PS_OUT PSLight(PS_IN vsdata)
{
	PS_OUT pout;
	float4 FinalA = g_txDiffuseA.Sample(g_Sample0, vsdata.t)*vsdata.c;
	//float4 FinalA = g_txDiffuseA.Sample(g_Sample0, vsdata.t) * (g_cAmbientLightColor + ComputeSpotLight(vsdata.world, vsdata.n));//+Specular(vsdata.n));
	pout.add = FinalA;
	//pout.add=vsdata.c;
	return pout;
}

PS_OUT PSBlend(PS_IN vsdata)
{
	PS_OUT pout;
	float4 FinalA = g_txDiffuseA.Sample(g_Sample0, vsdata.t);
	//	float4 FinalB = g_txDiffuseB.Sample(g_Sample0, vsdata.t);
	float alpha = (FinalA.r + FinalA.g + FinalA.b) / 3;
	pout.mul = float4(1 - alpha, 1 - alpha, 1 - alpha, alpha);
	//pout.add=lerp(FinalA,FinalB,vsdata.time);
	return pout;
}

PS_OUT PSBlend2(PS_IN vsdata)
{
	PS_OUT pout;
	float4 SRC = g_txDiffuseA.Sample(g_Sample0, vsdata.t);
	if (SRC.x == 1.0f)
	{
		if (SRC.y == 1.0f)
		{
			if (SRC.z == 1.0f)
			{
				SRC.w = 0.0f;
			}
		}
	}

	pout.add = SRC;
	return pout;
}

float4 PSTile(PS_IN vsdata) : SV_TARGET0
{
	//PS_OUT pout;
	float4 FinalA = float4(0,0,0,0);


	switch (StructBuf[0].i)
	{
		case 0:
		{
		FinalA = g_txDiffuseA.Sample(g_Sample0, vsdata.t)*vsdata.c;
		return FinalA;
		}

		case 1:
		{
		FinalA = g_txDiffuseC[0].Sample(g_Sample0, vsdata.t)*vsdata.c;
		return FinalA;
		}

		case 2:
		{
		FinalA = g_txDiffuseC[1].Sample(g_Sample0, vsdata.t)*vsdata.c;
		return FinalA;
		}

		case 3:
		{
		FinalA = g_txDiffuseC[2].Sample(g_Sample0, vsdata.t)*vsdata.c;
		return FinalA;
		}
	}
	return FinalA;

}