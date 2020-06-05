


Texture2D<float4>		InputMap : register(t0);
RWTexture2D<float4>		OutputMap : register(u0);

struct BufType
{

	float3	vPickPos;
	float	g_fRadius;
	float   Alpha[4];
	int		iIndex;
	int		iCol;
	int		iRow;
};

struct PosBuf
{
	float3 vPos;
};

StructuredBuffer<BufType> Buffer0 : register(t1);
//StructuredBuffer<PosBuf> PosBuf : register(t0);

// Group size
#define size_x 16
#define size_y 16

[numthreads(size_x, size_y, 1)]
void CSMAIN(uint3 GroupID : SV_GroupID, uint3 DispatchThreadID : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint GroupIndex : SV_GroupIndex)
{
	int3 texturelocation = int3(0, 0, 0);
	texturelocation.x = GroupID.x * size_x + GroupThreadID.x;
	texturelocation.y = GroupID.y * size_y + GroupThreadID.y;
	//texturelocation.x = DispatchThreadID.x;
	//texturelocation.y = DispatchThreadID.y;

	float iHalfCol = Buffer0[0].iCol / 2.0f;
	float iHalfRow = Buffer0[0].iRow / 2.0f;



	float2 vPos;
	vPos.x = (texturelocation.x - iHalfRow);
	vPos.y = -(texturelocation.y - iHalfRow);

	float fDistance = distance(texturelocation.xy, Buffer0[0].vPickPos.xz);


	float a = 1 - smoothstep(Buffer0[0].g_fRadius, Buffer0[0].g_fRadius + 20, fDistance);



	float4 fAlpha = InputMap.Load(texturelocation);

	//if (a == 0)
	//{
	//	return;
	//}




	switch (Buffer0[0].iIndex)
	{
	case 0:
	{
		if (fAlpha.x < a)
		{

			fAlpha.x = a;//* Buffer0[0].Alpha[0];
		}
		break;
	}

	case 1:
	{
		if (fAlpha.y < a)
		{
			fAlpha.y = a;// *Buffer0[0].Alpha[1];;

		}
		break;
	}
	case 2:
	{
		if (fAlpha.z < a)
		{
			fAlpha.z = a;// *Buffer0[0].Alpha[2];

		}
		break;
	}
	case 3:
	{
		if (fAlpha.a < a)
		{
			fAlpha.a = a;

		}
		break;
	}
	default:
	{

		break;
	}
	}



	OutputMap[texturelocation.xy] = fAlpha;
	// Color * float4(fDot, fDot, fDot, fDot);





}
//void CSEDITER(uint3 GroupID : SV_GroupID, uint3 DispatchThreadID : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint GroupIndex : SV_GroupIndex)
//{
//	int3 texturelocation = int3(0, 0, 0);
//	texturelocation.x = GroupID.x * size_x + GroupThreadID.x;
//	texturelocation.y = GroupID.y * size_y + GroupThreadID.y;
//	//texturelocation.x = DispatchThreadID.x;
//	//texturelocation.y = DispatchThreadID.y;
//
//	float iHalfCol = Buffer0[0].iCol / 2.0f;
//	float iHalfRow = Buffer0[0].iRow / 2.0f;
//
//
//
//	float2 vPos;
//	vPos.x = (texturelocation.x - iHalfRow);
//	vPos.y = -(texturelocation.y - iHalfRow);
//
//	float fDistance = distance(PosBuf[Buffer0[0], Buffer0[0].vPickPos.xz);
//
//
//	float a = (1 - smoothstep(Buffer0[0].g_fRadius, Buffer0[0].g_fRadius + 50, fDistance))*20;
//
//
//
//	float4 fAlpha = PosBuf[texturelocation];
//
//	//if (a == 0)
//	//{
//	//	return;
//	//}
//
//
//
//
//}
