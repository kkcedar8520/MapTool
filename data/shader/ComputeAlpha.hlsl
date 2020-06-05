Texture2D<float4> ReadTex : register(t0);
RWTexture2D<float4>	AlphaMap : register(u0);

struct BufSplatting
{
	float fRadian;
	float3 vIntersection;
	int index;
	int cellsize;
	int iRow;
	int iCol;
};
StructuredBuffer<BufSplatting> Buffer0 : register(t1);
#define size_x 32
#define size_y 32
[numthreads(size_x, size_y, 1)]
void main(uint3 GroupID : SV_GroupID, uint3 DispatchThreadID : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint GroupIndex : SV_GroupIndex)
{
	int3 texturelocation = int3(0, 0, 0);
	texturelocation.x = GroupID.x * size_x + GroupThreadID.x;
	texturelocation.y = GroupID.y * size_y + GroupThreadID.y;

	float fHalfCols = (Buffer0[0].iRow) / 2.0f;
	float fHalfRows = (Buffer0[0].iCol) / 2.0f;

	float2 vpix;
	vpix.x = (texturelocation.x - fHalfRows) * Buffer0[0].cellsize;
	vpix.y = -((texturelocation.y - fHalfCols) * Buffer0[0].cellsize);

	float fDistance = distance(vpix.xy, Buffer0[0].vIntersection.xz);

	float a = 1 - smoothstep(Buffer0[0].fRadian, Buffer0[0].fRadian+5, fDistance);

	if (a == 0)
	{
		return;
	}
	float4 fAlpha = ReadTex.Load(texturelocation);

	switch (Buffer0[0].index)
	{
		case 0:
		{
			if (fAlpha.x < a)
			{
				fAlpha.x = a;
			}
		}break;
		case 1:
		{
			if (fAlpha.y < a)
			{
				fAlpha.y = a;
			}
		}break;
		case 2: 
		{
			if (fAlpha.z < a)
			{
				fAlpha.z = a;
			}
		}break;
		case 3:
		{
			if (fAlpha.w < a)
			{
				fAlpha.w = a;
			}
		}break;
	}

	AlphaMap[texturelocation.xy] = fAlpha;
}