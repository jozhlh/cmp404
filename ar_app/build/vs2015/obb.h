#pragma once
#include "coordinate_frame.h"
#include "maths\matrix44.h"

#include <iostream>

namespace obb {
	class OBB : public CoordinateFrame
	{
	public:
		Vector E; //extents
		OBB(const Vector& e) : E(e) {}

		void SetCoordinateFrameFromMatrix(gef::Matrix44 transformation_matrix, std::string tag)
		{
			gef::Vector4 gef_origin = transformation_matrix.GetTranslation();
			gef::Vector4 gef_forward = transformation_matrix.GetColumn(2);
			gef::Vector4 gef_up = transformation_matrix.GetColumn(1);
			gef::Vector4 gef_right = transformation_matrix.GetColumn(0);
			obb::Vector origin;
			origin.x = gef_origin.x();
			origin.y = gef_origin.y();
			origin.z = gef_origin.z();
			obb::Vector forward;
			forward.x = gef_forward.x();
			forward.y = gef_forward.y();
			forward.z = gef_forward.z();
			obb::Vector up;
			up.x = gef_up.x();
			up.y = gef_up.y();
			up.z = gef_up.z();
			obb::Vector right;
			right.x = gef_right.x();
			right.y = gef_right.y();
			right.z = gef_right.z();
			SetCoordinateFrame(origin, right, forward, up);
		}
		//check if two oriented bounding boxes overlap
		const bool OBBOverlap
		(
			//A
			Vector&	a,	//extents
			Vector&	Pa,	//position
			Vector*	A,	//orthonormal basis
						//B
			Vector&	b,	//extents
			Vector&	Pb,	//position
			Vector*	B	//orthonormal basis
		)
		{
			//translation, in parent frame
			Vector v = Pb - Pa;
			//translation, in A's frame
			Vector T(v.dot(A[0]), v.dot(A[1]), v.dot(A[2]));

			//B's basis with respect to A's local frame
			Scalar R[3][3];
			float ra, rb, t;
			long i, k;

			//calculate rotation matrix
			for (i = 0; i<3; i++)
				for (k = 0; k<3; k++)
					R[i][k] = A[i].dot(B[k]);
			/*ALGORITHM: Use the separating axis test for all 15 potential
			separating axes. If a separating axis could not be found, the two
			boxes overlap. */

			//A's basis vectors
			for (i = 0; i<3; i++)
			{
				ra = a[i];
				rb = b[0] * fabs(R[i][0]) + b[1] * fabs(R[i][1]) + b[2] * fabs(R[i][2]);
				t = fabs(T[i]);
				if (t > ra + rb)
					return false;
			}

			//B's basis vectors
			for (k = 0; k<3; k++)
			{
				ra = a[0] * fabs(R[0][k]) + a[1] * fabs(R[1][k]) + a[2] * fabs(R[2][k]);
				rb = b[k];
				t = fabs(T[0] * R[0][k] + T[1] * R[1][k] + T[2] * R[2][k]);

				if (t > ra + rb)
					return false;
			}

			//9 cross products
			//L = A0 x B0
			ra = a[1] * fabs(R[2][0]) + a[2] * fabs(R[1][0]);
			rb = b[1] * fabs(R[0][2]) + b[2] * fabs(R[0][1]);
			t = fabs(T[2] * R[1][0] - T[1] * R[2][0]);
			if (t > ra + rb)
				return false;

			//L = A0 x B1
			ra = a[1] * fabs(R[2][1]) + a[2] * fabs(R[1][1]);
			rb = b[0] * fabs(R[0][2]) + b[2] * fabs(R[0][0]);
			t = fabs(T[2] * R[1][1] - T[1] * R[2][1]);
			if (t > ra + rb)
				return false;

			//L = A0 x B2
			ra = a[1] * fabs(R[2][2]) + a[2] * fabs(R[1][2]);
			rb = b[0] * fabs(R[0][1]) + b[1] * fabs(R[0][0]);
			t = fabs(T[2] * R[1][2] - T[1] * R[2][2]);
			if (t > ra + rb)
				return false;

			//L = A1 x B0
			ra = a[0] * fabs(R[2][0]) + a[2] * fabs(R[0][0]);
			rb = b[1] * fabs(R[1][2]) + b[2] * fabs(R[1][1]);
			t = fabs(T[0] * R[2][0] - T[2] * R[0][0]);
			if (t > ra + rb)
				return false;

			//L = A1 x B1
			ra = a[0] * fabs(R[2][1]) + a[2] * fabs(R[0][1]);
			rb = b[0] * fabs(R[1][2]) + b[2] * fabs(R[1][0]);
			t = fabs(T[0] * R[2][1] - T[2] * R[0][1]);
			if (t > ra + rb)
				return false;

			//L = A1 x B2
			ra = a[0] * fabs(R[2][2]) + a[2] * fabs(R[0][2]);
			rb = b[0] * fabs(R[1][1]) + b[1] * fabs(R[1][0]);
			t = fabs(T[0] * R[2][2] - T[2] * R[0][2]);
			if (t > ra + rb)
				return false;

			//L = A2 x B0
			ra = a[0] * fabs(R[1][0]) + a[1] * fabs(R[0][0]);
			rb = b[1] * fabs(R[2][2]) + b[2] * fabs(R[2][1]);
			t = fabs(T[1] * R[0][0] - T[0] * R[1][0]);
			if (t > ra + rb)
				return false;

			//L = A2 x B1
			ra = a[0] * fabs(R[1][1]) + a[1] * fabs(R[0][1]);
			rb = b[0] * fabs(R[2][2]) + b[2] * fabs(R[2][0]);
			t = fabs(T[1] * R[0][1] - T[0] * R[1][1]);
			if (t > ra + rb)
				return false;

			//L = A2 x B2
			ra = a[0] * fabs(R[1][2]) + a[1] * fabs(R[0][2]);
			rb = b[0] * fabs(R[2][1]) + b[1] * fabs(R[2][0]);
			t = fabs(T[1] * R[0][2] - T[0] * R[1][2]);
			if (t > ra + rb)
				return false;

			/*no separating axis found,
			the two boxes overlap */
			return true;
		}
	};

	
}

