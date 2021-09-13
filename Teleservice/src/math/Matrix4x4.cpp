#include "Matrix4x4.h"
#include <cstring>

Matrix4x4 Matrix4x4::IdentityMatrix = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f);

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
	float multipliedMatrixData[4][4];

	size_t elementCounter;
	for (size_t r = 0; r < 4; r++)
	{
		for (size_t c = 0; c < 4; c++)
		{
			float elementValue = 0;

			for (size_t i = 0; i < 4; i++)
			{
				elementValue += data[i][r] * rhs.data[c][i];
			}

			multipliedMatrixData[r][c] = elementValue;
		}
	}

	return Matrix4x4(multipliedMatrixData);
}

Matrix4x4::Matrix4x4(const float matrixData[4][4])
{
	for (size_t r = 0; r < 4; r++)
	{
		for (size_t c = 0; c < 4; c++)
		{
			data[r][c] = matrixData[c][r];
		}
	}
}

Matrix4x4::Matrix4x4(const float& a11, const float& a21, const float& a31, const float& a41,
					 const float& a12, const float& a22, const float& a32, const float& a42,
					 const float& a13, const float& a23, const float& a33, const float& a43,
					 const float& a14, const float& a24, const float& a34, const float& a44)
{
	// Row 1
	data[0][0] = a11;
	data[0][1] = a12;
	data[0][2] = a13;
	data[0][3] = a14;

	// Row 2
	data[1][0] = a21;
	data[1][1] = a22;
	data[1][2] = a23;
	data[1][3] = a24;

	// Row 3
	data[2][0] = a31;
	data[2][1] = a32;
	data[2][2] = a33;
	data[2][3] = a34;

	// Row 4
	data[3][0] = a41;
	data[3][1] = a42;
	data[3][2] = a43;
	data[3][3] = a44;
}

void Matrix4x4::Translate(Matrix4x4& matrix, const Vector3D& translationVector)
{
	// Alter the fourth column of the matrix (assumes the matrix to already be an identity matrix)
	matrix.data[3][0] = translationVector.x;
	matrix.data[3][1] = translationVector.y;
	matrix.data[3][2] = translationVector.z;
}
