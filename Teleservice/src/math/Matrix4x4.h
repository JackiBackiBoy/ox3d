#ifndef MATRIX_4X4_HEADER
#define MATRIX_4X4_HEADER

#include "math/Vector3D.h"

struct Matrix4x4
{
	Matrix4x4() {};
	Matrix4x4(const float matrixData[4][4]);
	Matrix4x4(const float& a11, const float& a21, const float& a31, const float& a41,
			  const float& a12, const float& a22, const float& a32, const float& a42,
			  const float& a13, const float& a23, const float& a33, const float& a43,
			  const float& a14, const float& a24, const float& a34, const float& a44);

	static void Translate(Matrix4x4& matrix, const Vector3D& translationVector);

	float data[4][4] = { 0 };

	inline float GetElementAtAdress(const int& index) const { return data[index / 4][index % 4]; }
	static Matrix4x4 IdentityMatrix;

	Matrix4x4 operator*(const Matrix4x4& rhs) const;
};
#endif