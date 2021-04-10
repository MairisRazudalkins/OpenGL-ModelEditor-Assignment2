#pragma once

#ifndef SceneObject
#define SceneObj

#include "Object.h"
#include "Structs.h"

class ListNode;
class SceneObjectList;

class SceneObject : public Object
{
	static SceneObjectList* sceneObjects;
	
	Vector3 pos, up = Vector3(0, 1, 0), forward = Vector3(0, 0, 1), right = Vector3(1, 0, 0);
	Rotation rot;

	ListNode* node;
	
	bool isVisible = true;
	bool isSelected = false;

	int backBufferColorID = -1;
	
public:
	SceneObject(Vector3 position, Rotation rotation);
	~SceneObject();
	
	void Render() override = 0;
	void Update(float deltaTime) override = 0;

	virtual void RenderToBackBuffer(int& indexOffset, Color& color) = 0;

	Vector3 GetPosition() { return pos; }
	Vector3 GetForward() { return forward; }
	Vector3 GetRight() { return right; }
	Vector3 GetUp() { return up; }
	
	Rotation GetRotation() { return rot; }

	void SetPosition(Vector3 position);
	void SetRotation(Rotation rotation);

	bool IsSelected() { return isSelected; }
	void SetSelected(bool isSelected) { this->isSelected = isSelected; }

	int GetBackBufferID() { return backBufferColorID; }
	void SetBackBufferID(int id) { backBufferColorID = id; }

	void SetVisibility(bool isVisible) { this->isVisible = isVisible; }
	bool IsVisible() { return isVisible; }

	static SceneObjectList* GetSceneObjects();
};

#endif