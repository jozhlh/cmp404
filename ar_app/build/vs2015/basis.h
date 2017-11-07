#ifndef BASIS_H_
#define BASIS_H_

#include "matrix.h"
#include "vector.h"

namespace obb
{
	class Basis
	{
	public:
		Matrix R;

	public:
		Basis(const Vector& v0, const Vector& v1, const Vector& v2)
			: R(v0, v1, v2)
		{}

		Basis(const Matrix& m) : R(m) {}

		const Vector& operator [] (long i) const { return R.C[i]; }
		const Vector& x() const { return R.C[0]; }
		const Vector& y() const { return R.C[1]; }
		const Vector& z() const { return R.C[2]; }
		const Matrix& basis() const { return R; }
		void basis(const Vector& v0, const Vector& v1, const Vector& v2)
		{
			this->R[0] = v0;
			this->R[1] = v1;
			this->R[2] = v2;
		}

/*		// Right-Handed Rotations
		void rotateAboutX(const Scalar& a)
		{
			if (0 != a)//don’t rotate by 0
			{
				Vector b1 = this->Y()*cos(a) + this->Z()*sin(a);
				Vector b2 = -this->Y()*sin(a) + this->Z()*cos(a);

				//set basis
				this->M[1] = b1;
				this->M[2] = b2;

				//x is unchanged
			}
		}

		void rotateAboutY(const Scalar& a)
		{
			if (0 != a)//don’t rotate by 0
			{
				Vector b2 = this->Z()*cos(a) + this->X()*sin(a);	//rotate z
				Vector b0 = -this->Z()*sin(a) + this->X()*cos(a);	//rotate x
																	//set basis
				this->M[2] = b2;
				this->M[0] = b0;
				//y is unchanged
			}
		}

		void rotateAboutZ(const Scalar& a)
		{
			if (0 != a)//don’t rotate by 0
			{
				//don’t over-write basis before calculation is done
				Vector b0 = this->X()*cos(a) + this->Y()*sin(a);	//rotate x
				Vector b1 = -this->X()*sin(a) + this->Y()*cos(a);	//rotate y
																	//set basis
				this->M[0] = b0;
				this->M[1] = b1;
				//z is unchanged
			}
		}
		*/
		//rotate the basis about the unit axis u by theta (radians)
		void rotate(const Scalar& theta, const Vector& u);

		//rotate, length of da is theta, unit direction of da is u
		void rotate(const Vector& da);



		// Transformations
		const Vector transformVectorToLocal(const Vector& v) const
		{
			return Vector(R.C[0].dot(v), R.C[1].dot(v), R.C[2].dot(v));
		}

		const POINT transformVectorToParent(const Vector& v) const
		{
			return R.C[0] * v.x + R.C[1] * v.y + R.C[2] * v.z;
		}
	};
}

#endif

