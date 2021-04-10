#include "SceneObject.h"
#include "SceneObjectList.h"
#include "OpenGL.h"
#include "Utils.h"

SceneObjectList* SceneObject::sceneObjects = nullptr;

SceneObject::SceneObject(Vector3 position, Rotation rotation)
{
	node = GetSceneObjects()->AddNode(this);
	
	SetPosition(position);
	SetRotation(rotation);
}

SceneObject::~SceneObject()
{
	GetSceneObjects()->DeleteNode(node);
	node = nullptr;
}

void SceneObject::SetPosition(Vector3 position)
{
	this->pos = position;
}

void SceneObject::SetRotation(Rotation rotation)
{
	this->rot = rotation;

	forward.x = cos(Utils::Radians(rotation.yaw)) * cos(Utils::Radians(rotation.pitch));
	forward.y = sin(Utils::Radians(rotation.pitch));
	forward.z = sin(Utils::Radians(rotation.yaw)) * cos(Utils::Radians(rotation.pitch));
	forward = forward.Normalize();

	right = up.Cross(forward).Normalize();
	//up = right.Cross(forward).Normalize();
}

SceneObjectList* SceneObject::GetSceneObjects()
{
	return sceneObjects != nullptr ? sceneObjects : sceneObjects = new SceneObjectList();
}