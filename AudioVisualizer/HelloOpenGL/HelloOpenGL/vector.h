#ifndef _VECTOR_H
#define _VECTOR_H

#include <string.h>
#include <math.h>

class Vector
{
public:
	//! Vector array.
	float mVal[3];

	//! Default constructor.
	Vector()
	{ }

	//! Constructor.
	Vector(float x, float y, float z)
	{
		mVal[0] = x;
		mVal[1] = y;
		mVal[2] = z;
	}

	//! Constructor.
	Vector(const Vector& other)
	{
		memcpy(mVal, other.mVal, sizeof(float) * 3);
	}

	//! Set vector value.
	void set(float x, float y, float z)
	{
		mVal[0] = x;
		mVal[1] = y;
		mVal[2] = z;
	}

	//! Addition operator.
	inline Vector operator+(const Vector& other) const
	{
		return Vector(
			mVal[0] + other.mVal[0],
			mVal[1] + other.mVal[1],
			mVal[2] + other.mVal[2]);
	}

	//! Subtract operator.
	inline Vector operator-(const Vector& other) const
	{
		return Vector(
			mVal[0] - other.mVal[0],
			mVal[1] - other.mVal[1],
			mVal[2] - other.mVal[2]);
	}

	//! Multiply operator.
	inline Vector operator*(float val) const
	{
		return Vector(
			mVal[0] * val,
			mVal[1] * val,
			mVal[2] * val);
	}

	//! Divide operator.
	inline Vector operator/(float val) const
	{
		return Vector(
			mVal[0] / val,
			mVal[1] / val,
			mVal[2] / val);
	}

	//! Addition assignment operator.
	inline Vector& operator+=(const Vector& other)
	{
		mVal[0] += other.mVal[0];
		mVal[1] += other.mVal[1];
		mVal[2] += other.mVal[2];
		return *this;
	}

	//! Subtract assignment operator.
	inline Vector& operator-=(const Vector& other)
	{
		mVal[0] -= other.mVal[0];
		mVal[1] -= other.mVal[1];
		mVal[2] -= other.mVal[2];
		return *this;
	}

	//! Multiply assignment operator.
	inline Vector& operator*=(float val)
	{
		mVal[0] *= val;
		mVal[1] *= val;
		mVal[2] *= val;
		return *this;
	}

	//! Divide assignment operator.
	inline Vector& operator/=(float val)
	{
		mVal[0] /= val;
		mVal[1] /= val;
		mVal[2] /= val;
		return *this;
	}

	//! Dot product.
	inline float dot(const Vector& other) const
	{
		return
			mVal[0] * other.mVal[0] +
			mVal[1] * other.mVal[1] +
			mVal[2] * other.mVal[2];
	}

	//! Cross product.
	inline Vector cross(const Vector& other) const
	{
		return Vector(
			mVal[1] * other.mVal[2] - mVal[2] * other.mVal[1],
			mVal[2] * other.mVal[0] - mVal[0] * other.mVal[2],
			mVal[0] * other.mVal[1] - mVal[1] * other.mVal[0]);
	}

	//! Get squared length
	inline float squaredLength() const
	{
		return
			mVal[0] * mVal[0] +
			mVal[1] * mVal[1] +
			mVal[2] * mVal[2];
	}

	//! Get length
	inline float length() const
	{
		return sqrt(squaredLength());
	}

	//! Normalize.
	inline void normalize()
	{
		float l = length();
		if (l > 0)
		{
			float invL = 1.0f / l;
			mVal[0] *= invL;
			mVal[1] *= invL;
			mVal[2] *= invL;
		}
	}

};

#endif
