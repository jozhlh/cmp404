#ifndef _GEF_QUATERNION_H
#define _GEF_QUATERNION_H

#include <math.h>

namespace gef
{
	class Matrix44;
	class Vector4;

class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Matrix44& matrix);
	void SetFromMatrix(const class Matrix44& matrix);
	const Quaternion operator * (const Quaternion& quaternion) const;
	const Quaternion operator -() const;
	const Quaternion operator *(float scale) const;
	const Quaternion operator /(float scale) const;
	const Quaternion operator +(const Quaternion &q) const;
	float LengthSquared() const;
	float Length() const;
	void Normalise();
	void Identity();
	void Lerp(const Quaternion& startQ, const Quaternion& endQ, float time);
	void Slerp(const Quaternion& startQ, const Quaternion& endQ, float time);
	void Conjugate(const Quaternion& quaternion);
	
	// Extended Functions
	void SetEulers(float eX, float eY, float eZ);
	gef::Vector4 Eulers();

	float x;
	float y;
	float z;
	float w;

private:
	gef::Vector4 ConvertEulerRadToDeg(gef::Vector4 euler);
};

}

#include "quaternion.inl"


#endif // _GEF_QUATERNION_H
