#include <iostream>
#include "Camera.h"
#include "Input.h"
#include "Utils.h"
#include "Constants.h"

Camera* Camera::activeCam = nullptr;

Camera::Camera(Vector3 position, Rotation rotation) : SceneObject(position, rotation)
{
	activeCam = this;
	//smoothX = rotation.roll;
	//smoothY = rotation.pitch;
}

Camera::~Camera()
{
	delete activeCam;
}

void Camera::Update(float deltaTime)
{
	if (Input::IsMouseKeyDown(MouseKey::RIGHT_MOUSE_BUTTON))
	{
		glutWarpPointer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		glutSetCursor(GLUT_CURSOR_NONE);
		double d = 1 - exp(log(0.5) * 50.f * deltaTime); // smooth cam movement
		smoothX += (Utils::Clamp(Input::GetMousePos().x - (SCREEN_WIDTH / 2), -1.f, 1.f) - smoothX) * d;
		smoothY += (Utils::Clamp(Input::GetMousePos().y - (SCREEN_HEIGHT / 2), -1.f, 1.f) - smoothY) * d;

		Rotation rotation = GetRotation();

		rotation.yaw += smoothX * (sensitivity * deltaTime);
		rotation.pitch -= smoothY * (sensitivity * deltaTime);

		if (rotation.yaw > 360.0f)
			rotation.yaw = rotation.yaw - 360.f;
		if (rotation.yaw < 0.f)
			rotation.yaw = 360.f - rotation.yaw;

		if (rotation.pitch > 179.f)
			rotation.pitch = 179.f;
		if (rotation.pitch < 1.f)
			rotation.pitch = 1.f;

		SetRotation(rotation);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}

	if (Input::GetScrollDir() == Scroll::SCROLL_UP)
		SetPosition(GetPosition() + (GetForward() * (movementSpeed * movementSpeedMultiplier)) * deltaTime);
	else if (Input::GetScrollDir() == Scroll::SCROLL_DOWN)
		SetPosition(GetPosition() - (GetForward() * (movementSpeed * movementSpeedMultiplier)) * deltaTime);

	if (Input::IsKeyDown('w'))
		SetPosition(GetPosition() + (GetForward() * movementSpeed) * deltaTime);
	else if (Input::IsKeyDown('s'))
		SetPosition(GetPosition() - (GetForward() * movementSpeed) * deltaTime);

	if (Input::IsKeyDown('a'))
		SetPosition(GetPosition() - (GetRight() * movementSpeed) * deltaTime);
	else if (Input::IsKeyDown('d'))
		SetPosition(GetPosition() + (GetRight() * movementSpeed) * deltaTime);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (view == PERSPECTIVE)
		gluPerspective(45, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 1000.f);
	else if (view == ORTHOGRAPHIC)
		glOrtho(-1,  1, -1, 1, 0.01f, 1000.f);

	gluLookAt(GetPosition().x, GetPosition().y, GetPosition().z, GetPosition().x + GetForward().x, GetPosition().y + GetForward().y, GetPosition().z + GetForward().z, GetUp().x, GetUp().y, GetUp().z);

	glMatrixMode(GL_MODELVIEW);
	//printf("%f %f %f\n", GetPosition().x, GetPosition().y, GetPosition().z);
}