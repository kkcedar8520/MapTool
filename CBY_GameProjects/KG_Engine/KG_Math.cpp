#include "KG_Math.h"

namespace CMA
{
	Vector2::Vector2()
	{
		x = y = 0.0f;
	}

	Vector2::Vector2(float fx, float fy)
	{
		x = fx;
		y = fy;
	}

	Vector2::Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	Vector2 Vector2::operator + (Vector2 const& v)
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 Vector2::operator - (Vector2 const& v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 Vector2::operator * (float const& fs)
	{
		return Vector2(x * fs, y * fs);
	}

	Vector2 Vector2::operator + (float const& fs)
	{
		return Vector2(x + fs, y + fs);
	}

	Vector2 Vector2::operator / (float const& fs)
	{
		return Vector2(x / fs, y / fs);
	}

	bool Vector2::operator == (Vector2 const& v)
	{
		if (fabs(x - v.x) <= C_EPSILON)
		{
			if (fabs(y - v.y) <= C_EPSILON)
			{
				return true;
			}
		}
		return false;
	}

	Vector3::Vector3()
	{
		x = y = z = 0.0f;
	}

	Vector3::Vector3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}

	Vector3::Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	float Vector3::Length()
	{
		return sqrt(x*x + y * y + z * z);
	}

	Vector3 Vector3::Nomal()
	{
		float size = 1.0f / Length();
		return Vector3(x * size, y * size, z * size);
	}

	Vector3 Vector3::operator + (Vector3 const& v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 Vector3::operator - (Vector3 const& v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 Vector3::operator * (float const& fs)
	{
		return Vector3(x * fs, y * fs, z * fs);
	}

	Vector3 Vector3::operator + (float const& fs)
	{
		return Vector3(x + fs, y + fs, z + fs);
	}

	Vector3 Vector3::operator / (float const& fs)
	{
		return Vector3(x / fs, y / fs, z / fs);
	}

	float  Vector3::operator | (Vector3 const &v)
	{
		return x * v.x, +y * v.y + z * v.z;
	}
	Vector3  Vector3::operator ^ (Vector3 const &v)
	{
		return Vector3(y*v.z - z * v.y,
			z*v.x - x * v.z,
			x*v.y - y * v.x);
	}

	bool Vector3::operator == (Vector3 const& v)
	{
		if (fabs(x - v.x) <= C_EPSILON)
		{
			if (fabs(y - v.y) <= C_EPSILON)
			{
				if (fabs(z - v.z) <= C_EPSILON)
				{
					return true;
				}
			}
		}
		return false;
	}

	Vector4::Vector4()
	{
		x = y = z = w = 0.0f;
	}

	Vector4::Vector4(float fx, float fy, float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	Vector4::Vector4(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	Vector4 Vector4::operator + (Vector4 const& v)
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4 Vector4::operator - (Vector4 const& v)
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4 Vector4::operator * (float const& fs)
	{
		return Vector4(x * fs, y * fs, z * fs, w * fs);
	}

	Vector4 Vector4::operator + (float const& fs)
	{
		return Vector4(x + fs, y + fs, z + fs, w + fs);
	}

	Vector4 Vector4::operator / (float const& fs)
	{
		return Vector4(x / fs, y / fs, z / fs, w / fs);
	}

	bool Vector4::operator == (Vector4 const& v)
	{
		if (fabs(x - v.x) <= C_EPSILON)
		{
			if (fabs(y - v.y) <= C_EPSILON)
			{
				if (fabs(z - v.z) <= C_EPSILON)
				{
					if (fabs(w - v.w) <= C_EPSILON)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	Matrix::Matrix()
	{
		Identity();
	}

	Matrix::Matrix(const Matrix& mat)
	{
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				m[row][col] = mat.m[row][col];
			}
		}
	}

	void Matrix::Identity()
	{
		_11 = _12 = _13 = _14 = 0.0f;
		_21 = _22 = _23 = _24 = 0.0f;
		_31 = _32 = _33 = _34 = 0.0f;
		_41 = _42 = _43 = _44 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}

	void Matrix::MOVE(float x, float y, float z)
	{
		Identity();
		_41 = x;
		_42 = y;
		_43 = z;
	}

	void Matrix::MOVE(Vector3 v)
	{
		Identity();
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}

	void Matrix::Scale(float x, float y, float z)
	{
		Identity();
		_11 = x;
		_22 = y;
		_33 = z;
	}

	void Matrix::Scale(Vector3 v)
	{
		Identity();
		_11 = v.x;
		_22 = v.y;
		_33 = v.z;
	}

	void Matrix::RotateX(float rad)
	{
		Identity();
		_22 = cos(rad), _23 = sin(rad);
		_32 = -sin(rad), _33 = cos(rad);
	}

	void Matrix::RotateY(float rad)
	{
		Identity();
		_11 = cos(rad), _13 = sin(rad);
		_31 = -sin(rad), _33 = cos(rad);
	}

	void Matrix::RotateZ(float rad)
	{
		Identity();
		_11 = cos(rad), _12 = sin(rad);
		_21 = -sin(rad), _22 = cos(rad);
	}

	void Matrix::Transpose()
	{
		Matrix mat = *this;
		Matrix self;
		self.Identity();
		self._11 = mat._11; self._12 = mat._21; self._13 = mat._31; self._14 = mat._41;
		self._21 = mat._12; self._22 = mat._22; self._23 = mat._32; self._24 = mat._42;
		self._31 = mat._13; self._32 = mat._23; self._33 = mat._33; self._34 = mat._43;
		self._41 = mat._14; self._42 = mat._24; self._43 = mat._34; self._44 = mat._44;
		*this = self;
	}

	Matrix Matrix::operator *(Matrix const &mat)
	{
		Matrix re;
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				re.m[row][col] = m[row][0] * mat.m[0][col] + m[row][1] * mat.m[1][col] + m[row][2] * mat.m[2][col] + m[row][3] * mat.m[3][col];
			}
		}
		return re;
	}

	Matrix Matrix::CreateViewLookAt(Vector3& pos, Vector3& At, Vector3& up)
	{
		Matrix mat;
		Vector3 Look = (At - pos).Nomal();
		Vector3 Right = (up ^ Look).Nomal();
		Vector3 Up = (Look^Right).Nomal();

		mat._11 = Right.x; mat._12 = Up.x; mat._13 = Look.x;
		mat._21 = Right.y; mat._22 = Up.y; mat._23 = Look.y;
		mat._31 = Right.z; mat._32 = Up.z; mat._33 = Look.z;
		mat._41 = -(pos | Right);
		mat._42 = -(pos | Up);
		mat._43 = -(pos | Look);

		return mat;
	}

	Matrix Matrix::CreatePerspectiveFovLH(float Near, float Far, float FovY, float Aspect)
	{
		Matrix mat;
		ZeroMemory(&mat, sizeof(Matrix));
		float h, w, q;
		h = 1.0f / tan(FovY*0.5f);
		w = h / Aspect;
		q = Far / (Far - Near);
		mat._11 = w;
		mat._22 = h;
		mat._33 = q;
		mat._43 = -q * Near;
		mat._34 = 1;
		return mat;
	}

}

namespace KG_Math
{
	float Distance(fPOINT pt1, fPOINT pt2)
	{
		float distance = sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y));
		return distance;
	}

	fPOINT Center(fRECT rt1)
	{
		float PointX = (rt1.right + rt1.left) / 2.0f;
		float PointY = (rt1.top + rt1.bottom) / 2.0f;

		return { PointX, PointY };
	}

	float GetDistance(float ax, float ay, float bx, float by)		//두점의 거리를 구하는 함수
	{
		float Distance;
		Distance = sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by));
		return Distance;
	}
	float GetDistance(D3DXVECTOR3* v1, D3DXVECTOR3* v2)			//두 벡터의 거리
	{
		D3DXVECTOR3 vlen = *v1 - *v2;
		float fret = D3DXVec3Length(&vlen);
		return fret;
	}

	float Clamp(float x, float lowerlimit, float upperlimit)
	{
		if (x < lowerlimit)
			x = lowerlimit;
		if (x > upperlimit)
			x = upperlimit;
		return x;
	}

	float Smoothstep(float edge0, float edge1, float x)
	{
		// Scale, bias and saturate x to 0..1 range
		x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		// Evaluate polynomial
		return x * x * (3 - 2 * x);
	}

	float Lerp(float value1, float value2, float amount)
	{
		return float(value1 + ((float)(value2 - value1) * amount));
	}

	bool Spherecollision(fSphere sh1, fSphere sh2)				//두 원의 충돌 확인함수
	{
		float truedistance = Distance(sh1.fpt, sh2.fpt);
		float radiusdistance = sh1.radius + sh2.radius;

		if (truedistance > radiusdistance)
		{
			return false;
		}
		else
			return true;
	}
	bool fRectCollision(fRECT rt1, fRECT rt2)						//두 RECT가 충돌하는지 확인하는 함수
	{
		fPOINT fp1;
		fPOINT fp2;
		float distanceX;
		float distanceY;
		float width;
		float height;

		fp1 = Center(rt1);
		fp2 = Center(rt2);

		distanceX = abs(fp1.x - fp2.x);
		distanceY = abs(fp1.y - fp2.y);
		width = abs((((float)rt1.right - (float)rt1.left) / 2) + (((float)rt2.right - (float)rt2.left) / 2));
		height = abs((((float)rt1.bottom - (float)rt1.top) / 2) + (((float)rt2.bottom - (float)rt2.top) / 2));

		if (width > distanceX && height > distanceY)
		{
			return true;
		}
		else
			return false;
	}
	fSphere RecttoSphere(fRECT rt1)							//RECT를 원으로 바꾸는 함수
	{
		fPOINT rt1Center;
		fSphere rt1Sphere;
		rt1Center = Center(rt1);
		float distance = GetDistance(rt1Center.x, rt1Center.y, rt1.right, rt1.bottom);
		rt1Sphere.fpt.x = rt1Center.x;
		rt1Sphere.fpt.y = rt1Center.y;
		rt1Sphere.radius = distance;

		return rt1Sphere;
	}
}

KG_Math_0::KG_Math_0()
{
}


KG_Math_0::~KG_Math_0()
{
}
