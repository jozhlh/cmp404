#include "transform.h"
#include <iostream>

namespace gef
{
	Transform::Transform()
	{
		rotation_ = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		translation_ = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		scale_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		eulers_ = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		set_rotation_eulers(eulers_);
	}

	Transform::Transform(const Matrix44& matrix)
	{
		Set(matrix);
	}

	const Matrix44 Transform::GetMatrix() const
	{
		Matrix44 result, scale_matrix, rotation_matrix;
	
		scale_matrix.Scale(scale_);
		rotation_matrix.Rotation(rotation_);
		result = scale_matrix * rotation_matrix;
		result.SetTranslation(translation_);

		return result;
	}

	void Transform::Set(const Matrix44& matrix)
	{
		translation_ = matrix.GetTranslation();
		Quaternion rotation;
		rotation_.SetFromMatrix(matrix);
		rotation_.Normalise();
		scale_ = matrix.GetScale();
	}

	void Transform::Linear2TransformBlend(const gef::Transform& start, const gef::Transform& end, const float time)
	{
		Vector4 scale(1.0f, 1.0f, 1.0f), translation;
		Quaternion rotation;
		scale.Lerp(start.scale(), end.scale(), time);
		translation.Lerp(start.translation(), end.translation(), time);
		rotation.Slerp(start.rotation(), end.rotation(), time);
		set_scale(scale);
		set_rotation(rotation);
		set_translation(translation);
	}

	void Transform::Rotate(const Vector4 eulerRotation)
	{
		//Vector4 intermediate = rotation_.Eulers();
		//std::cout << "current rotation: " << intermediate.y() << std::endl;
		eulers_ += eulerRotation;
		set_rotation_eulers(eulers_);
	}

	void Transform::set_rotation_eulers(const Vector4 & rot)
	{
		eulers_ = rot;
		Quaternion r;
		r.Identity();
		r.SetEulers(rot.x(), rot.y(), rot.z());
		//std::cout << "current rotation: " << r.y() << std::endl;
		rotation_ = r;
	}
}