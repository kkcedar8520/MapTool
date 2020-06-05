#pragma once
#include <math.h>
#include "KG_DxHelperEx.h"

#define C_EPSILON 0.01f

struct fPOINT
{
	float x;
	float y;
};

struct fRECT
{
	float left;
	float top;
	float right;
	float bottom;
};

struct fSphere
{
	fPOINT fpt;
	float radius;
};

namespace CMA
{
	struct float2
	{
		union														//메모리 공유를 통해 float x,y나 v[2]나 같은 메모리를 쓰기에 어떻게 쓰던 상관이 없다.
		{
			struct { float x, y; };
			float v[2];
		};
	};

	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};

	struct float4
	{
		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};

	struct float4x4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};



	class Vector2 : public float2
	{
	public:
		Vector2 operator + (Vector2 const& v);
		Vector2 operator - (Vector2 const& v);
		Vector2 operator * (float const& fs);
		Vector2 operator + (float const& fs);
		Vector2 operator / (float const& fs);
		bool operator == (Vector2 const& v);

	public:
		Vector2();
		Vector2(float fx, float fy);
		Vector2(const Vector2& v);
		~Vector2() {};
	};

	class Vector3 : public float3
	{
	public:
		Vector3 operator + (Vector3 const& v);
		Vector3 operator - (Vector3 const& v);
		Vector3 operator * (float const& fs);
		Vector3 operator + (float const& fs);
		Vector3 operator / (float const& fs);
		float  operator | (Vector3 const &v);
		Vector3  operator ^ (Vector3 const &v);

		bool operator == (Vector3 const& v);
		float Length();
		Vector3 Nomal();

	public:
		Vector3();
		Vector3(float fx, float fy, float fz);
		Vector3(const Vector3& v);
		~Vector3() {};
	};

	class Vector4 : public float4
	{
	public:
		Vector4 operator + (Vector4 const& v);
		Vector4 operator - (Vector4 const& v);
		Vector4 operator * (float const& fs);
		Vector4 operator + (float const& fs);
		Vector4 operator / (float const& fs);
		bool operator == (Vector4 const& v);

	public:
		Vector4();
		Vector4(float fx, float fy, float fz, float fw);
		Vector4(const Vector4& v);
		~Vector4() {};
	};

	class Matrix : public float4x4
	{
	public:
		Matrix operator *(Matrix const &mat);

		void Identity();			//단위행렬 만들기
		void MOVE(float x, float y, float z);
		void MOVE(Vector3 v);			//이동
		void Scale(float x, float y, float z);
		void Scale(Vector3 v);					//크기 변환
		void RotateX(float rad);
		void RotateY(float rad);
		void RotateZ(float rad);				//회전
		void Transpose();
		static Matrix CreateViewLookAt(Vector3& pos, Vector3& At, Vector3& up);
		static Matrix CreatePerspectiveFovLH(float near, float Far, float FovY, float Aspect);


	public:
		Matrix();
		Matrix(const Matrix& mat);
		~Matrix() {};
	};
}

namespace KG_Math
{
	float Distance(fPOINT pt1, fPOINT pt2);
	fPOINT Center(fRECT rt1);
	
	float GetDistance(float ax, float ay, float bx, float by);		//두점의 거리를 구하는 함수
	float GetDistance(D3DXVECTOR3* v1, D3DXVECTOR3* v2);			//두 벡터의 거리
	
	float Clamp(float x, float lowerlimit, float upperlimit);

	float Smoothstep(float edge0, float edge1, float x);
	float Lerp(float value1, float value2, float amount);
	

	bool Spherecollision(fSphere sh1, fSphere sh2);				//두 원의 충돌 확인함수
	bool fRectCollision(fRECT rt1, fRECT rt2);						//두 RECT가 충돌하는지 확인하는 함수
	fSphere RecttoSphere(fRECT rt1);						//RECT를 원으로 바꾸는 함수
	
}

class KG_Math_0
{
public:
	KG_Math_0();
	virtual ~KG_Math_0();
};



