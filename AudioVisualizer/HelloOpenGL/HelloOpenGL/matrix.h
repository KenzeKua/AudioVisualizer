#ifndef _MATRIX_H
#define _MATRIX_H

#include "vector.h"

#define M_PI 3.1415926535897932384626433832795f
#define M_2PI 6.283185307179586476925286766559f
#define M_HALF_PI 1.5707963267948966192313216916398f
#define M_DEG_2_RAD (M_PI / 180.0f)

class Matrix
{
protected:
	//! Identity matrix.
	static float sIdentity[4][4];

	void invertMatrixGeneral(const float* m, float* out);
	void invertMatrix(const float* m, float* out);

public:
	//! Matrix array.
	float mVal[4][4];

	//! Constructor.
	Matrix()
	{ }

	//! Constructor.
	Matrix(const Matrix& other)
	{
		memcpy(mVal, other.mVal, sizeof(float) * 16);
	}

	//! Copy assignment operator.
	Matrix& operator=(const Matrix& other)
	{
		memcpy(mVal, other.mVal, sizeof(float) * 16);
		return *this;
	}

	//! Matrix multiply.
	Matrix operator*(const Matrix& other) const
	{
		Matrix r;
		r.mVal[0][0] = other.mVal[0][0] * mVal[0][0] + other.mVal[0][1] * mVal[1][0] + other.mVal[0][2] * mVal[2][0] + other.mVal[0][3] * mVal[3][0];
		r.mVal[0][1] = other.mVal[0][0] * mVal[0][1] + other.mVal[0][1] * mVal[1][1] + other.mVal[0][2] * mVal[2][1] + other.mVal[0][3] * mVal[3][1];
		r.mVal[0][2] = other.mVal[0][0] * mVal[0][2] + other.mVal[0][1] * mVal[1][2] + other.mVal[0][2] * mVal[2][2] + other.mVal[0][3] * mVal[3][2];
		r.mVal[0][3] = other.mVal[0][0] * mVal[0][3] + other.mVal[0][1] * mVal[1][3] + other.mVal[0][2] * mVal[2][3] + other.mVal[0][3] * mVal[3][3];

		r.mVal[1][0] = other.mVal[1][0] * mVal[0][0] + other.mVal[1][1] * mVal[1][0] + other.mVal[1][2] * mVal[2][0] + other.mVal[1][3] * mVal[3][0];
		r.mVal[1][1] = other.mVal[1][0] * mVal[0][1] + other.mVal[1][1] * mVal[1][1] + other.mVal[1][2] * mVal[2][1] + other.mVal[1][3] * mVal[3][1];
		r.mVal[1][2] = other.mVal[1][0] * mVal[0][2] + other.mVal[1][1] * mVal[1][2] + other.mVal[1][2] * mVal[2][2] + other.mVal[1][3] * mVal[3][2];
		r.mVal[1][3] = other.mVal[1][0] * mVal[0][3] + other.mVal[1][1] * mVal[1][3] + other.mVal[1][2] * mVal[2][3] + other.mVal[1][3] * mVal[3][3];

		r.mVal[2][0] = other.mVal[2][0] * mVal[0][0] + other.mVal[2][1] * mVal[1][0] + other.mVal[2][2] * mVal[2][0] + other.mVal[2][3] * mVal[3][0];
		r.mVal[2][1] = other.mVal[2][0] * mVal[0][1] + other.mVal[2][1] * mVal[1][1] + other.mVal[2][2] * mVal[2][1] + other.mVal[2][3] * mVal[3][1];
		r.mVal[2][2] = other.mVal[2][0] * mVal[0][2] + other.mVal[2][1] * mVal[1][2] + other.mVal[2][2] * mVal[2][2] + other.mVal[2][3] * mVal[3][2];
		r.mVal[2][3] = other.mVal[2][0] * mVal[0][3] + other.mVal[2][1] * mVal[1][3] + other.mVal[2][2] * mVal[2][3] + other.mVal[2][3] * mVal[3][3];

		r.mVal[3][0] = other.mVal[3][0] * mVal[0][0] + other.mVal[3][1] * mVal[1][0] + other.mVal[3][2] * mVal[2][0] + other.mVal[3][3] * mVal[3][0];
		r.mVal[3][1] = other.mVal[3][0] * mVal[0][1] + other.mVal[3][1] * mVal[1][1] + other.mVal[3][2] * mVal[2][1] + other.mVal[3][3] * mVal[3][1];
		r.mVal[3][2] = other.mVal[3][0] * mVal[0][2] + other.mVal[3][1] * mVal[1][2] + other.mVal[3][2] * mVal[2][2] + other.mVal[3][3] * mVal[3][2];
		r.mVal[3][3] = other.mVal[3][0] * mVal[0][3] + other.mVal[3][1] * mVal[1][3] + other.mVal[3][2] * mVal[2][3] + other.mVal[3][3] * mVal[3][3];

		return r;
	}

	//! Transform vector with matrix.
	Vector operator*(const Vector& vec) const
	{
		float prod[4] = {
			vec.mVal[0] * mVal[0][0] + vec.mVal[1] * mVal[1][0] + vec.mVal[2] * mVal[2][0] + mVal[3][0],
			vec.mVal[0] * mVal[0][1] + vec.mVal[1] * mVal[1][1] + vec.mVal[2] * mVal[2][1] + mVal[3][1],
			vec.mVal[0] * mVal[0][2] + vec.mVal[1] * mVal[1][2] + vec.mVal[2] * mVal[2][2] + mVal[3][2],
			mVal[3][3]
		};

		// convert homogenous to cartesian.
		float div = 1.0f / prod[3];

		return Vector(prod[0] * div, prod[1] * div, prod[2] * div);
	}

	void translate(const Vector& offset)
	{
		mVal[3][0] = offset.mVal[0];
		mVal[3][1] = offset.mVal[1];
		mVal[3][2] = offset.mVal[2];
	}

	Matrix transpose()
	{
		Matrix r;
		r.mVal[0][0] = mVal[0][0]; r.mVal[0][1] = mVal[1][0]; r.mVal[0][2] = mVal[2][0]; r.mVal[0][3] = mVal[3][0];
		r.mVal[1][0] = mVal[0][1]; r.mVal[1][1] = mVal[1][1]; r.mVal[1][2] = mVal[2][1]; r.mVal[1][3] = mVal[3][1];
		r.mVal[2][0] = mVal[0][2]; r.mVal[2][1] = mVal[1][2]; r.mVal[2][2] = mVal[2][2]; r.mVal[2][3] = mVal[3][2];
		r.mVal[3][0] = mVal[0][3]; r.mVal[3][1] = mVal[1][3]; r.mVal[3][2] = mVal[2][3]; r.mVal[3][3] = mVal[3][3];
		return r;
	}

	Matrix inverse()
	{
		Matrix r;
		invertMatrix((float*)mVal, (float*)r.mVal);
		return r;
	}

	//! Create identity matrix.
	static Matrix makeIdentityMatrix()
	{
		Matrix matrix;
		memcpy(matrix.mVal, sIdentity, sizeof(float) * 16);
		return matrix;
	}

	//! Create translation matrix.
	inline static Matrix makeTranslationMatrix(const Vector& pos)
	{
		return makeTranslationMatrix(pos.mVal[0], pos.mVal[1], pos.mVal[2]);
	}

	//! Create translation matrix.
	static Matrix makeTranslationMatrix(float x, float y, float z)
	{
		Matrix matrix;

		matrix.mVal[0][0] = 1.0f;
		matrix.mVal[1][0] = 0.0f;
		matrix.mVal[2][0] = 0.0f;
		matrix.mVal[3][0] = x;
 
		matrix.mVal[0][1] = 0.0f;
		matrix.mVal[1][1] = 1.0f;
		matrix.mVal[2][1] = 0.0f;
		matrix.mVal[3][1] = y;
 
		matrix.mVal[0][2] = 0.0f;
		matrix.mVal[1][2] = 0.0f;
		matrix.mVal[2][2] = 1.0f;
		matrix.mVal[3][2] = z;

		matrix.mVal[0][3] = 0.0f;
		matrix.mVal[1][3] = 0.0f;
		matrix.mVal[2][3] = 0.0f;
		matrix.mVal[3][3] = 1.0f;

		return matrix;
	}

	//! Create scale matrix.
	inline static Matrix makeScaleMatrix(const Vector& scale)
	{
		return makeScaleMatrix(scale.mVal[0], scale.mVal[1], scale.mVal[2]);
	}

	//! Create scale matrix.
	static Matrix makeScaleMatrix(float x, float y, float z)
	{
		Matrix matrix;

		matrix.mVal[0][0] = x;
		matrix.mVal[1][0] = 0.0f;
		matrix.mVal[2][0] = 0.0f;
		matrix.mVal[3][0] = 0.0f;
 
		matrix.mVal[0][1] = 0.0f;
		matrix.mVal[1][1] = y;
		matrix.mVal[2][1] = 0.0f;
		matrix.mVal[3][1] = 0.0f;
 
		matrix.mVal[0][2] = 0.0f;
		matrix.mVal[1][2] = 0.0f;
		matrix.mVal[2][2] = z;
		matrix.mVal[3][2] = 0.0f;

		matrix.mVal[0][3] = 0.0f;
		matrix.mVal[1][3] = 0.0f;
		matrix.mVal[2][3] = 0.0f;
		matrix.mVal[3][3] = 1.0f;

		return matrix;
	}

	//! Create rotation matrix with angle axis.
	static Matrix makeRotateMatrix(float angle, const Vector& axis)
	{
		Matrix matrix;

		float s = sin(angle * M_DEG_2_RAD);
		float c = cos(angle * M_DEG_2_RAD);
		float t = 1 - c;

		Vector ax = axis / axis.length();

		float x = ax.mVal[0];
		float y = ax.mVal[1];
		float z = ax.mVal[2];
 
		matrix.mVal[0][0] = t * x * x + c;
		matrix.mVal[1][0] = t * y * x + s * z;
		matrix.mVal[2][0] = t * z * x - s * y;
		matrix.mVal[3][0] = 0.0f;
 
		matrix.mVal[0][1] = t * x * y - s * z;
		matrix.mVal[1][1] = t * y * y + c;
		matrix.mVal[2][1] = t * z * y + s * x;
		matrix.mVal[3][1] = 0.0f;
 
		matrix.mVal[0][2] = t * x * z + s * y;
		matrix.mVal[1][2] = t * y * z - s * x;
		matrix.mVal[2][2] = t * z * z + c;
		matrix.mVal[3][2] = 0.0f;

		matrix.mVal[0][3] = 0.0f;
		matrix.mVal[1][3] = 0.0f;
		matrix.mVal[2][3] = 0.0f;
		matrix.mVal[3][3] = 1.0f;

		return matrix;
	}

	//! Create rotation matrix from orientation axis.
	static Matrix makeOrientMatrix(const Vector& x, const Vector& y, const Vector& z)
	{
		Matrix matrix;

		matrix.mVal[0][0] = x.mVal[0];
		matrix.mVal[0][1] = x.mVal[1];
		matrix.mVal[0][2] = x.mVal[2];
		matrix.mVal[0][3] = 0.0f;

		matrix.mVal[1][0] = y.mVal[0];
		matrix.mVal[1][1] = y.mVal[1];
		matrix.mVal[1][2] = y.mVal[2];
		matrix.mVal[1][3] = 0.0f;

		matrix.mVal[2][0] = z.mVal[0];
		matrix.mVal[2][1] = z.mVal[1];
		matrix.mVal[2][2] = z.mVal[2];
		matrix.mVal[2][3] = 0.0f;

		matrix.mVal[3][0] = 0.0f;
		matrix.mVal[3][1] = 0.0f;
		matrix.mVal[3][2] = 0.0f;
		matrix.mVal[3][3] = 1.0f;

		return matrix;
	}

	//! Create look at matrix.
	static Matrix makeLookAtMatrix(const Vector& dir, const Vector& up)
	{
		// direction is Z axis.
		Vector z = dir;

		// get X axis with cross product.
		Vector x = up.cross(dir);

		// normalize Z and X axis.
		// Done here before getting Y axis for optimization:
		// Reason: cross product of two perpendicular unit vector always gets a unit vector.
		// Result: This way we avoid doing a normalize() on Y axis.
		z.normalize();
		x.normalize();

		// get Y axis.
		Vector y = z.cross(x);

		return makeOrientMatrix(x, y, z);
	}

};

#endif
