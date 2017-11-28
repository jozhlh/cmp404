#include "game_object_manager.h"
#include <graphics\mesh.h>

#include <iostream>

GameObjectManager::GameObjectManager()
{
	current_parent_ = 0;
	current_parent_marker_ = NULL;
	overlap_allowance_ = 0.3f;
	overlap_counter_ = 0.0f;
	for (int marker_num = 0; marker_num < NUM_OF_MARKERS; marker_num++)
	{
		marker_transform_matrices_[marker_num].SetIdentity();
		markers_visible_[marker_num] = false;
	}
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::GivePlayerReference(GameObject * new_object)
{
	hovership_ = new_object;
	TrackObject(new_object);
}

void GameObjectManager::UpdateMarkerData()
{
	UpdateObjectsInList(marker_bound_objects_);
	UpdateObjectsInList(marker_specific_objects_);
}

void GameObjectManager::RenderObjects(gef::Renderer3D * renderer)
{
	for (std::list<GameObject*>::iterator tracked_object = marker_bound_objects_.begin(); tracked_object != marker_bound_objects_.end(); ++tracked_object)
	{
		(*tracked_object)->Render(renderer);
	}
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		(*tracked_object)->Render(renderer);
	}
}

void GameObjectManager::UpdateObjectsInList(std::list<GameObject*> target_list)
{
	for (std::list<GameObject*>::iterator tracked_object = target_list.begin(); tracked_object != target_list.end(); ++tracked_object)
	{
		int parentID = (*tracked_object)->GetParentMarker();
		(*tracked_object)->SetMarkerPosition(marker_transform_matrices_[parentID]);
		(*tracked_object)->SetMarkerVisiblity(markers_visible_[parentID]);
	}
}

void GameObjectManager::TransferOwnership(GameObject* new_owner)
{
	current_parent_ = new_owner->GetParentMarker();
	hovership_->SetParentMarker(new_owner->GetParentMarker());
	for (std::list<GameObject*>::iterator tracked_object = marker_bound_objects_.begin(); tracked_object != marker_bound_objects_.end(); ++tracked_object)
	{
		//(*tracked_object)->SetLocalTransformFromMatrix((*tracked_object)->GetLocalTransform()->GetMatrix() * displacement);
		SetNewLocal((*tracked_object), new_owner);
		(*tracked_object)->SetParentMarker(new_owner->GetParentMarker());
	}
}
/*
gef::Matrix44 GameObjectManager::CalculateTransformDisplacement(GameObject * current_parent, GameObject * new_parent)
{
	gef::Matrix44 current_transform = current_parent->GetTransform()->GetMatrix();
	gef::Matrix44 target_transform = new_parent->GetTransform()->GetMatrix();
	gef::Matrix44 inverse_current = current_transform;
	inverse_current.Inverse(current_transform);
	gef::Matrix44 inverse_target = target_transform;
	inverse_target.Inverse(target_transform);
	gef::Matrix44 displacement_transform = inverse_target * current_transform;
	return displacement_transform;
}*/

gef::Matrix44 GameObjectManager::CalculateTransformDisplacement(GameObject * current_object, GameObject * new_parent)
{
	gef::Matrix44 current_transform = current_object->GetTransform()->GetMatrix();
	gef::Matrix44 target_transform = new_parent->GetTransform()->GetMatrix();
	gef::Matrix44 inverse_current = current_transform;
	inverse_current.Inverse(current_transform);
	gef::Matrix44 inverse_target = target_transform;
	inverse_target.Inverse(target_transform);
	gef::Matrix44 displacement_transform = inverse_target * current_transform;
	return displacement_transform;
}

void GameObjectManager::SetNewLocal(GameObject * current_object, GameObject * new_parent)
{
	gef::Transform* object_world = current_object->GetTransform();
	//gef::Transform object_parent;
	//object_parent.Set(current_object->ParentTransform());
	gef::Transform* new_parent_transform = new_parent->GetTransform();

	gef::Matrix44 inv;
	inv.Inverse(new_parent_transform->GetMatrix());

	current_object->SetMarkerPosition(new_parent_transform->GetMatrix());
	current_object->SetLocalTransformFromMatrix(object_world->GetMatrix() * inv);



	//m_transform_->set_rotation_eulers(amount_to_rotate);
	
	//m_local_transform_->Set(m_transform_->GetMatrix() * inv);
	//UpdateMeshTransform();
}

/*
void GameObjectManager::SetNewLocal(GameObject * current_object, GameObject * new_parent)
{
	gef::Vector4 current_rot = current_object->GetTransform()->rotation_eulers();
	gef::Vector4 m1_rot = current_parent_marker_->GetTransform()->rotation_eulers();
	gef::Vector4 m2_rot = new_parent->GetTransform()->rotation_eulers();

	gef::Vector4 amount_to_rot = current_rot;
	//gef::Vector4 amount_to_rot = m1_rot - m2_rot;
	//amount_to_rot.set_x(amount_to_rot.x() * -1);
	//amount_to_rot.set_y(amount_to_rot.y() * -1);
	//amount_to_rot.set_z(amount_to_rot.z() * -1);

	gef::Transform* target = new_parent->GetTransform();
	gef::Transform* current = current_object->GetTransform();
	gef::Transform* ref = current_parent_marker_->GetTransform();
	std::cout << "M1" << std::endl;
	PrintVector(ref->rotation_eulers());
	std::cout << "M2" << std::endl;
	PrintVector(target->rotation_eulers());
	std::cout << "Current" << std::endl;
	PrintVector(current->rotation_eulers());

	gef::Matrix44 current_transform = current_object->GetTransform()->GetMatrix();
	gef::Matrix44 target_transform = new_parent->GetTransform()->GetMatrix();
	gef::Matrix44 inverse_target = target_transform;
	inverse_target.Inverse(target_transform);
	gef::Matrix44 displacement_transform;
	displacement_transform.SetIdentity();
	current_object->SetMarkerPosition(target_transform);
	displacement_transform = displacement_transform * (current_transform * inverse_target);
	gef::Transform trans;
	trans.Set(displacement_transform);
	//trans.set_rotation_eulers(amount_to_rot);
	//trans.Rotate(amount_to_rot);
	std::cout << "Result" << std::endl;
	PrintVector(trans.rotation_eulers());
	current_object->SetLocalTransformFromMatrix(trans.GetMatrix());
	current_object->RotateFromReparent(amount_to_rot);
	//TestMatrices();
}
*/
/*
void GameObjectManager::SetNewLocal(GameObject * current_object, GameObject * new_parent)
{
	gef::Matrix44 current_transform = current_object->GetTransform()->GetMatrix();
	gef::Matrix44 target_transform = new_parent->GetTransform()->GetMatrix();
	gef::Matrix44 inverse_target = target_transform;
	inverse_target.Inverse(target_transform);
	gef::Matrix44 displacement_transform;
	displacement_transform.SetIdentity();
	current_object->SetMarkerPosition(target_transform);
	displacement_transform = displacement_transform * (current_transform * inverse_target);
	current_object->SetLocalTransformFromMatrix(displacement_transform);
	//TestMatrices();
}
*/
/*
void GameObjectManager::SetNewLocal(GameObject * current_object, GameObject * new_parent)
{
	gef::Matrix44 current_transform = current_object->GetTransform()->GetMatrix();
	gef::Matrix44 target_transform = new_parent->GetTransform()->GetMatrix();
	gef::Quaternion ref_rotation = current_parent_marker_->GetTransform()->rotation();
	gef::Quaternion current_rotation = current_object->GetTransform()->rotation();
	gef::Quaternion target_rotation = new_parent->GetTransform()->rotation();
	//gef::Matrix44 inverse_current = current_transform;
	//inverse_current.Inverse(current_transform);
	gef::Matrix44 inverse_target = target_transform;
	inverse_target.Inverse(target_transform);
	gef::Matrix44 displacement_transform;
	displacement_transform.SetIdentity();
	current_object->SetMarkerPosition(target_transform);
	// https://stackoverflow.com/questions/10871793/get-rotation-matrix-between-two-two-transform-matrices-xna
	//q = q1 * (q2) ^ -1; q = q1 * conj(q2);
	target_rotation.Conjugate(target_rotation);
	gef::Quaternion rot = ref_rotation * target_rotation;
	rot.Normalise();
	rot = current_rotation * rot;
	displacement_transform.Rotation(current_rotation);
	displacement_transform = displacement_transform * (current_transform * inverse_target);
	gef::Matrix44 rotation_mat;
	rotation_mat.Rotation(current_rotation);
	gef::Matrix44 scale_mat;
	scale_mat.Scale(displacement_transform.GetScale());
	gef::Vector4 translation = displacement_transform.GetTranslation();
	displacement_transform.SetIdentity();
	displacement_transform = scale_mat * displacement_transform;
	displacement_transform = rotation_mat * displacement_transform;
	displacement_transform.SetTranslation(translation);
	current_object->SetLocalTransformFromMatrix(displacement_transform);
	//TestMatrices();
}*/

void GameObjectManager::PrintMatrix(gef::Matrix44 matrix)
{
	std::cout << matrix.GetRow(0).x() << " " << matrix.GetRow(0).y() << " " << matrix.GetRow(0).z() << " " << matrix.GetRow(0).w() << std::endl;
	std::cout << matrix.GetRow(1).x() << " " << matrix.GetRow(1).y() << " " << matrix.GetRow(1).z() << " " << matrix.GetRow(1).w() << std::endl;
	std::cout << matrix.GetRow(2).x() << " " << matrix.GetRow(2).y() << " " << matrix.GetRow(2).z() << " " << matrix.GetRow(2).w() << std::endl;
	std::cout << matrix.GetRow(3).x() << " " << matrix.GetRow(3).y() << " " << matrix.GetRow(3).z() << " " << matrix.GetRow(3).w() << std::endl << std::endl;
}

void GameObjectManager::PrintVector(gef::Vector4 vec)
{
	std::cout << vec.x() << " " << vec.y() << " " << vec.z() << " " << vec.w() << std::endl << std::endl;
}

void GameObjectManager::TestMatrices()
{
	gef::Matrix44 M1, M2, Mr;
	M1.SetIdentity();
	M2.SetIdentity();

	gef::Quaternion m2Rot;
	m2Rot.SetEulers(0.0f, 0.0f, 30.f);
	m2Rot.Normalise();
	M2.Rotation(m2Rot);
	std::cout << "M2 with rotation" << std::endl;
	PrintMatrix(M2);

	M2.SetTranslation(gef::Vector4(2.0f, 2.0f, 0.0f));
	std::cout << "M2 translation" << std::endl;
	PrintMatrix(M2);
	
	M2.Inverse(M2);
	std::cout << "M2 Inversed" << std::endl;
	PrintMatrix(M2);

	Mr = M1 * M2;
	std::cout << "M result" << std::endl;
	PrintMatrix(Mr);

}

// Check Player Wall Collisions

// Check Player Road Collisions

bool GameObjectManager::PlayerRoadCollision(float dt)
{
	overlap_counter_ += dt;
	// if player collision with parent road collider
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		if ((*tracked_object)->GetParentMarker() == hovership_->GetParentMarker())
		{
			current_parent_marker_ = *tracked_object;
			//if (CollisionAABB((*tracked_object)->Collider(), hovership_->Collider()))
			if (CollisionOOBB((*tracked_object)->Obb(), hovership_->Obb(), false))
			{
				return true;
			}
		}
		else
		{
			//if (CollisionAABB((*tracked_object)->Collider(), hovership_->Collider()))
			if (CollisionOOBB((*tracked_object)->Obb(), hovership_->Obb(), true))
			{
				if (overlap_counter_ > overlap_allowance_)
				{
					overlap_counter_ = 0.0f;
					TransferOwnership(*tracked_object);
					current_parent_marker_ = *tracked_object;
				}
				return true;
			}
		}
			
			//else
			//{
			//	break;
			//}
		//}
	}
	/*	
	// else if player collision with any other road collider
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		//if (CollisionSpherical((*tracked_object)->Collider(), hovership_->Collider()))
		if (CollisionOOBB((*tracked_object)->Obb(), hovership_->Obb(), false))
		{
			// transfer all ownership to new parent
			if (current_parent != NULL)
			{
				TransferOwnership((*tracked_object)->GetParentMarker(), CalculateTransformDisplacement(current_parent, *tracked_object));
			}
			else
			{
				// Currently no parent
			}
			return true;
		}
	}
	*/
	// player is dead
	return false;
}

bool GameObjectManager::PlayerWallCollision(gef::Vector4 * collision_vector)
{
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		// for each box collider
		bool oldCol = false;
		if (oldCol)
		{
			std::list<gef::MeshInstance*> walls = (*tracked_object)->GetWallCubes();
			for (std::list<gef::MeshInstance*>::iterator wall = walls.begin(); wall != walls.end(); ++wall)
			{
				if (CollisionSpherical(hovership_->Collider(), *wall))
				{
					GetCollisionVector(hovership_->Collider(), *wall, collision_vector);
					return true;
				}
			}
		}
		else
		{
			std::list<obb::OBB*> walls = (*tracked_object)->GetWallObbs();
			std::list<gef::MeshInstance*> wall_meshes = (*tracked_object)->GetWallCubes();
			int it = 0;
			for (std::list<obb::OBB*>::iterator wall = walls.begin(); wall != walls.end(); ++wall)
			{
				if (CollisionOOBB(hovership_->Obb(), *wall, false))
				{
					std::list<gef::MeshInstance*>::iterator wall_mesh_it = wall_meshes.begin();
					std::advance(wall_mesh_it, it);

					GetCollisionVector(hovership_->Collider(), *wall_mesh_it, collision_vector);
					return true;
				}
				it++;
			}
		}
	}

	return false;
}

void GameObjectManager::GetCollisionVector(gef::MeshInstance * collider_mesh_1_, gef::MeshInstance * collider_mesh_2_, gef::Vector4* collision_vector)
{
	collision_vector->set_x(0.0f);
	collision_vector->set_y(0.0f);
	collision_vector->set_z(0.0f);

	// Calculate Collision Vector
	// spherical_normal = player_mesh_transform - box_mesh_transform
	gef::Vector4 spherical_normal = collider_mesh_2_->transform().GetTranslation() - collider_mesh_1_->transform().GetTranslation();
	// spherical_normal.y = 0
	spherical_normal.set_z(0.0f);
	// spherical_normal.normalise
	spherical_normal.Normalise();

	if (abs(spherical_normal.x()) < abs(spherical_normal.y()))
	{
		// collision normal is z axis
		if (spherical_normal.y() > 0.0f)
		{
			collision_vector->set_y(1.0f);
			return;
		}
		else
		{
			collision_vector->set_y(-1.0f);
			return;
		}
	}
	else
	{
		// collision normal is x axis
		if (spherical_normal.x() > 0.0f)
		{
			collision_vector->set_x(1.0f);
			return;
		}
		else
		{
			collision_vector->set_x(-1.0f);
			return;
		}
	}
}

bool GameObjectManager::CollisionAABB(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_)
{
	// Get the transformed min and max vertices
	gef::Vector4 min_1 = collider_mesh_1_->mesh()->aabb().min_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 max_1 = collider_mesh_1_->mesh()->aabb().max_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 min_2 = collider_mesh_2_->mesh()->aabb().min_vtx().Transform(collider_mesh_2_->transform());
	gef::Vector4 max_2 = collider_mesh_2_->mesh()->aabb().max_vtx().Transform(collider_mesh_2_->transform());

	return(max_1.x() > min_2.x()
		&& min_1.x() < max_2.x()
		&& max_1.y() > min_2.y()
		&& min_1.y() < max_2.y()
		&& max_1.z() > min_2.z()
		&& min_1.z() < max_2.z());
}

bool GameObjectManager::CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_)
{
	// Calculate the squared distance between the centers of both spheres
	gef::Vector4 collider_1_pos = collider_mesh_1_->mesh()->bounding_sphere().position().Transform(collider_mesh_1_->transform());
	gef::Vector4 collider_2_pos = collider_mesh_2_->mesh()->bounding_sphere().position().Transform(collider_mesh_2_->transform());
	float distance_squared = gef::Vector4(collider_1_pos - collider_2_pos).LengthSqr();

	// Calculate the squared sum of both radii
	float radii_sum_squared = collider_mesh_1_->mesh()->bounding_sphere().radius() + collider_mesh_2_->mesh()->bounding_sphere().radius();
	radii_sum_squared *= radii_sum_squared;

	// if the the distance squared is less than or equal to the square sum of the radii, then there is a collision
	if (distance_squared <= radii_sum_squared)
	{
		return true;
	}

	// if the distance squared is greater than the sum of the radii squared there is no collision
	return false;
}

bool GameObjectManager::CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_, bool debug)
{
	if (collider_obb_1_->OBBOverlap(
		collider_obb_1_->E,
		collider_obb_1_->O,
		collider_obb_1_->GetBasis(),

		collider_obb_2_->E,
		collider_obb_2_->O,
		collider_obb_2_->GetBasis()))
	{
		if (debug)
		{
			std::cout << collider_obb_1_->O.x << " " << collider_obb_1_->O.y << " " << collider_obb_1_->O.z << " size " << collider_obb_1_->E.x << " " << collider_obb_1_->E.y << " " << collider_obb_1_->E.z << std::endl;
			std::cout << collider_obb_2_->O.x << " " << collider_obb_2_->O.y << " " << collider_obb_2_->O.z << " size " << collider_obb_2_->E.x << " " << collider_obb_2_->E.y << " " << collider_obb_2_->E.z << std::endl << std::endl;
		}
		 
		return true;
	}
	return false;
}


