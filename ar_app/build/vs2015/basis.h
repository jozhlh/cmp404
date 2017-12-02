/*
	basis.h
	
	Based on the class provided at this link:
	https://www.gamasutra.com/view/feature/131761/c_data_structures_for_rigidbody_.php?page=4
	C++ Data Structures for Rigid-Body Physics: The BASIS Type
	Author: Miguel Gomez
	02/07/99

	@author	Miguel Gomez, Josh Hale
	Last Edited: 02/12/17
*/

#ifndef _BASIS_H
#define _BASIS_H

#include "matrix.h"
#include "vector.h"

namespace obb
{
	class Basis
	{
	public:
		Matrix R;

	public:
		Basis();
		~Basis();
		Basis(const Vector& v0, const Vector& v1, const Vector& v2)
			: R(v0, v1, v2)
		{}

		Basis(const Matrix& m) : R(m) {}

		const Vector& operator [] (long i) const { return R.C[i]; }
		const Vector& x() const { return R.C[0]; }
		const Vector& y() const { return R.C[1]; }
		const Vector& z() const { return R.C[2]; }
		const Matrix& basis() const { return R; }

		void SetBasis(const Vector& v0, const Vector& v1, const Vector& v2)
		{
			this->R[0] = v0;
			this->R[1] = v1;
			this->R[2] = v2;
		}

		Vector* GetBasis()
		{
			Vector* basis_as_vectors = new Vector[3];
			basis_as_vectors[0] = R[0];
			basis_as_vectors[1] = R[1];
			basis_as_vectors[2] = R[2];
			return basis_as_vectors;
		}

		//rotate the basis about the unit axis u by theta (radians)
		void rotate(const Scalar& theta, const Vector& u);

		//rotate, length of da is theta, unit direction of da is u
		void rotate(const Vector& da);

		// Transformations
		const Vector transformVectorToLocal(const Vector& v) const
		{
			return Vector(R.C[0].dot(v), R.C[1].dot(v), R.C[2].dot(v));
		}

		const Point transformVectorToParent(const Vector& v) const
		{
			return R.C[0] * v.x + R.C[1] * v.y + R.C[2] * v.z;
		}
	};
}

#endif //_BASIS_H

