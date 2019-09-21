#pragma once
#include<cmath>

class Matrix4x4
{
public:
#pragma warning (push)
#pragma warning (disable : 26495)
	Matrix4x4() { MatrixIdentity(); }
	~Matrix4x4() { };
	Matrix4x4(float* viewMatrix)
#pragma warning (pop)
	{
		pFloatToMatrix(viewMatrix);
	}
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	inline void operator *= (float v)
	{
		_11 *= v;
		_12 *= v;
		_13 *= v;
		_14 *= v;

		_21 *= v;
		_22 *= v;
		_23 *= v;
		_24 *= v;

		_31 *= v;
		_32 *= v;
		_33 *= v;
		_34 *= v;

		_41 *= v;
		_42 *= v;
		_43 *= v;
		_44 *= v;
	}

	inline void operator /= (float v)
	{
		_11 /= v;
		_12 /= v;
		_13 /= v;
		_14 /= v;

		_21 /= v;
		_22 /= v;
		_23 /= v;
		_24 /= v;

		_31 /= v;
		_32 /= v;
		_33 /= v;
		_34 /= v;

		_41 /= v;
		_42 /= v;
		_43 /= v;
		_44 /= v;
	}

	inline void MatrixRotX(float Angle)
	{
		float cosAng = cosf(Angle);
		float sinAng = sinf(Angle);

		_22 = cosAng;
		_33 = cosAng;
		_23 = sinAng;
		_32 = -sinAng;

	}

	inline void MatrixRotY(float Angle)
	{
		float cosAng = cosf(Angle);
		float sinAng = sinf(Angle);

		_11 = cosAng;
		_33 = cosAng;
		_13 = sinAng;
		_31 = -sinAng;

	}

	inline void MatrixRotZ(float Angle)
	{
		float cosAng = cosf(Angle);
		float sinAng = sinf(Angle);

		_11 = cosAng;
		_22 = cosAng;
		_12 = sinAng;
		_21 = -sinAng;
	}

	inline void MatrixIdentity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_14 = 0.0f;

		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_24 = 0.0f;

		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
	}

	inline void Transpose(const Matrix4x4 src)
	{
		_11 = src._11;
		_12 = src._21;
		_13 = src._31;
		_14 = src._41;

		_21 = src._12;
		_22 = src._22;
		_23 = src._32;
		_24 = src._42;

		_31 = src._13;
		_32 = src._23;
		_33 = src._33;
		_34 = src._43;

		_41 = src._14;
		_42 = src._24;
		_43 = src._34;
		_44 = src._44;
	}

	inline void pFloatToMatrix(float* viewMatrix)
	{
		_11 = viewMatrix[0];
		_12 = viewMatrix[1];
		_13 = viewMatrix[2];
		_14 = viewMatrix[3];

		_21 = viewMatrix[4];
		_22 = viewMatrix[5];
		_23 = viewMatrix[6];
		_24 = viewMatrix[7];

		_31 = viewMatrix[8];
		_32 = viewMatrix[9];
		_33 = viewMatrix[10];
		_34 = viewMatrix[11];

		_41 = viewMatrix[12];
		_42 = viewMatrix[13];
		_43 = viewMatrix[14];
		_44 = viewMatrix[15];
	}

	inline void MatrixMultiply(const Matrix4x4 src1, const Matrix4x4 src2)
	{
		_11 = src1._11 * src2._11 + src1._12 * src2._21 + src1._13 * src2._31 + src1._14 * src2._41;
		_12 = src1._11 * src2._12 + src1._12 * src2._22 + src1._13 * src2._32 + src1._14 * src2._42;
		_13 = src1._11 * src2._13 + src1._12 * src2._23 + src1._13 * src2._33 + src1._14 * src2._43;
		_14 = src1._11 * src2._14 + src1._12 * src2._24 + src1._13 * src2._34 + src1._14 * src2._44;
		_21 = src1._21 * src2._11 + src1._22 * src2._21 + src1._23 * src2._31 + src1._24 * src2._41;
		_22 = src1._21 * src2._12 + src1._22 * src2._22 + src1._23 * src2._32 + src1._24 * src2._42;
		_23 = src1._21 * src2._13 + src1._22 * src2._23 + src1._23 * src2._33 + src1._24 * src2._43;
		_24 = src1._21 * src2._14 + src1._22 * src2._24 + src1._23 * src2._34 + src1._24 * src2._44;
		_31 = src1._31 * src2._11 + src1._32 * src2._21 + src1._33 * src2._31 + src1._34 * src2._41;
		_32 = src1._31 * src2._12 + src1._32 * src2._22 + src1._33 * src2._32 + src1._34 * src2._42;
		_33 = src1._31 * src2._13 + src1._32 * src2._23 + src1._33 * src2._33 + src1._34 * src2._43;
		_34 = src1._31 * src2._14 + src1._32 * src2._24 + src1._33 * src2._34 + src1._34 * src2._44;
		_41 = src1._41 * src2._11 + src1._42 * src2._21 + src1._43 * src2._31 + src1._44 * src2._41;
		_42 = src1._41 * src2._12 + src1._42 * src2._22 + src1._43 * src2._32 + src1._44 * src2._42;
		_43 = src1._41 * src2._13 + src1._42 * src2._23 + src1._43 * src2._33 + src1._44 * src2._43;
		_44 = src1._41 * src2._14 + src1._42 * src2._24 + src1._43 * src2._34 + src1._44 * src2._44;
	}


};

template<typename T>
class vec2
{
public:
	T x, y;

	inline vec2()
	{
		x = (T)0;
		y = (T)0;
	}

	inline vec2(const vec2& src)
	{
		x = src.x;
		y = src.y;
	}

	inline vec2(const T& x, const T& y):x(x),y(y) {}

	inline T GetLength()
	{
		return x * x + y * y;
	}

	inline T GetMagnitude()
	{
		return (T)sqrt(GetLength());
	}

	inline vec2 GetNormalize()
	{
		T magnitude = GetMagnitude();
		return vec2(x / magnitude, y / magnitude);
	}

	inline vec2 operator+(const vec2 v)
	{
		return vec2(x + v.x, y + v.y);
	}

	inline vec2 operator-(const vec2 v)
	{
		return vec2(x - v.x, y - v.y);
	}

	inline vec2 operator*(const vec2 v)
	{
		return vec2(x * v.x, y * v.y);
	}

	inline vec2 operator/( const vec2 v)
	{
		return vec2(x / v.x, y / v.y);
	}

};


template<typename T>
class vec3
{
public:
	T x, y,z;

	inline vec3()
	{
		x = (T)0;
		y = (T)0;
		z = (T)0;
	}

	inline vec3(const vec3& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	inline vec3(const T& x, const T& y, const T& z) :x(x), y(y), z(z) {}

	inline T GetDot(const vec3<T>& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
	inline T GetLength()
	{
		return (T)sqrt(GetDot(*this));
	}

	inline vec3 GetNormalize()
	{
		T magnitude = GetLength();
		return vec3(x / magnitude, y / magnitude);
	}

	inline vec3 operator+(const vec3 v)
	{
		return vec3(x + v.x, y + v.y ,z + v.z);
	}

	inline vec3 operator-(const vec3 v)
	{
		return vec3(x - v.x, y - v.y,z - v.z);
	}

	inline vec3 operator*(const vec3 v)
	{
		return vec3(x * v.x, y * v.y,z * v.z);
	}

	inline vec3 operator/(const vec3 v)
	{
		return vec3(x / v.x, y / v.y,z /v.z);
	}
	inline vec3 operator/(float  v)
	{
		return vec3(x / v, y / v, z / v);
	}
	inline vec3 operator*(float  v)
	{
		return vec3(x * v, y * v, z * v);
	}
};


template<typename T>
class vec4
{
public:
	T x, y, z,w;

	inline vec4()
	{
		x = (T)0;
		y = (T)0;
		z = (T)0;
		w = (T)0;
	}

	inline vec4(const vec4& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
	}

	inline vec4(const T& x, const T& y,const T & z,const T & w) :x(x), y(y), z(z),w(w) {}

	inline T GetLength()
	{
		return x * x + y * y + z * z + w*w;
	}

	inline T GetMagnitude()
	{
		return (T)sqrt(GetLength());
	}

	inline vec4 GetNormalize()
	{
		T magnitude = GetMagnitude();
		return vec4(x / magnitude, y / magnitude);
	}

	inline void Transform(const vec3<float>  v, const Matrix4x4  m)
	{
		x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
		y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
		z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
		w = v.x * m._14 + v.y * m._24 + v.z * m._34 + m._44;
	}

	inline vec4 operator+(const vec4 v)
	{
		return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	inline vec4 operator-(const vec4 v)
	{
		return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	inline vec4 operator*(const vec4 v)
	{
		return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	inline vec4 operator/(const vec4 v)
	{
		return vec4(x / v.x, y / v.y, z / v.z ,w / v.w);
	}
};

using fVec2 = vec2<float>;
using fVec3 = vec3<float>;
using fVec4 = vec4<float>;


inline bool WorldToScreen(float * viewMatrix,fVec3 pos,fVec2 &newPos,fVec2 screenSize,bool transpose)
{
	auto TransfromCoord = [](float x, float f)
	{
		return f * (x + 1) * 0.5f;
	};
	fVec4 vec4;
	Matrix4x4 view = viewMatrix;

	if (transpose)
		view.Transpose(view);

	vec4.Transform(pos, view);

	if (vec4.w < 0.5f)
		return false;

	vec4.x /= vec4.w;
	vec4.y /= vec4.w;

	newPos.x = TransfromCoord(vec4.x, screenSize.x);
	newPos.y = TransfromCoord(-vec4.y, screenSize.y);

	return true;
}