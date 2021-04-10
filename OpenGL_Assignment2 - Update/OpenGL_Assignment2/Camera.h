#pragma once
#include "SceneObject.h"

enum CameraView
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class Camera : public SceneObject
{
	static Camera* activeCam;
	
	const float movementSpeed = 1.f, movementSpeedMultiplier = 5.f;
	float sensitivity = 65.f, smoothX, smoothY;

	CameraView view;
	
public:
	Camera(Vector3 position, Rotation rotation);
	~Camera();

	void Update(float deltaTime) override;
	void Render() override {};
	void RenderToBackBuffer(int& indexOffset, Color& color) override {};
	void SetCameraView(CameraView view) { this->view = view; }
	
	static Camera* GetActiveCam() { return activeCam; }
};