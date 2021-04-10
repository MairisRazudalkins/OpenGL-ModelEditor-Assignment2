#include "Gizmo.h"

#include <cstdio>

#include "Camera.h"
#include "Input.h"
#include "OpenGL.h"
#include "Mesh.h"

Gizmo* Gizmo::inst = nullptr;

Gizmo::Gizmo() : SceneObject(Vector3(), Rotation())
{
	SetVisibility(false);
}

Gizmo::~Gizmo()
{
	delete inst;
	inst = nullptr;
}

void Gizmo::Render()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	
	if (IsVisible())
	{
		Vector3 camPos = Camera::GetActiveCam()->GetPosition();
		float distance = Vector3::Distance(GetPosition(), camPos);

		if (distance < 2.f)
			distance = 2.f;

		float size = distance;
		
		if (gizmoMode == GIZMO_MOVEMENT)
		{
			DrawArrow(GetPosition(), Vector3(0.2f, 0.f, 0.f), Rotation(0.f, 90.f, 0.f), Color(255, 0, 0), size);
			DrawArrow(GetPosition(), Vector3(0.f, 0.2f, 0.f), Rotation(-90.f, 0.f, 0.f), Color(0, 255, 0), size);
			DrawArrow(GetPosition(), Vector3(0.f, 0.f, 0.2f), Rotation(0.f, 0.f, 90.f), Color(0, 0, 255), size);
		}
	}
}

void Gizmo::Update(float deltaTime)
{
}

void Gizmo::RenderToBackBuffer(int& indexOffset, Color& color)
{
	if (!IsVisible())
		return;

	glClear(GL_DEPTH_BUFFER_BIT);
	
	Vector3 camPos = Camera::GetActiveCam()->GetPosition();
	float distance = Vector3::Distance(GetPosition(), camPos);

	if (distance < 2.f)
		distance = 2.f;

	float size = distance;

	DrawArrow(GetPosition(), Vector3(0.2f, 0.f, 0.f), Rotation(0.f, 90.f, 0.f), Color(255, 255, 254), size);
	DrawArrow(GetPosition(), Vector3(0.f, 0.2f, 0.f), Rotation(-90.f, 0.f, 0.f), Color(255, 255, 253), size);
	DrawArrow(GetPosition(), Vector3(0.f, 0.f, 0.2f), Rotation(0.f, 0.f, 90.f), Color(255, 255, 252), size);
}

void Gizmo::MoveGizmo(MoveAxis axis)
{
	SetInteracting(true);
	moveAxis = axis;
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLfloat winX = Input::GetMousePos().x, winY = (float)viewport[3] - Input::GetMousePos().y, winZ;
	GLdouble posX, posY, posZ;

	float data[3];
	glReadPixels(Input::GetMousePos().x, winY, 1, 1, GL_RGB, GL_FLOAT, data);

	Color color(int(data[0] * 255.f), int(data[1] * 255.f), int(data[2] * 255.f));
	Color gizmoColor = axis == MOVE_X ? Color(255, 255, 254) : axis == MOVE_Y ? Color(255, 255, 253) : Color(255, 255, 252);

	if (gizmoColor.b == color.b)
	{	
		glReadPixels(Input::GetMousePos().x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		Vector3 targetPos(posX, posY, posZ);
		SetPosition(Vector3(axis == MOVE_X ? targetPos.x - 0.1f : GetPosition().x, axis == MOVE_Y ? targetPos.y - 0.1f : GetPosition().y, axis == MOVE_Z ? targetPos.z - 0.1f : GetPosition().z));
		
		if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_MESH_VERTEX) // should replace this with much better implementation so anything can be moved without needing to check what is being moved;
			((Mesh*)OpenGL::GetInst()->GetActiveObject())->MoveSelectedVertex(GetPosition());
		else if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_SELECTION)
			OpenGL::GetInst()->GetActiveObject()->SetPosition(GetPosition());
		if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_MESH_FACE)
			((Mesh*)OpenGL::GetInst()->GetActiveObject())->MoveSelectedFace(GetPosition());
	}
}

void Gizmo::SetInteracting(bool interacting, MoveAxis axis)
{
	this->interacting = interacting;

	if (interacting)
		moveAxis = axis;
}

void Gizmo::DrawArrow(Vector3 position, Vector3 offset, Rotation rotation, Color color, float size)
{
	position += offset * (0.001f + (size / 4.f));

	glEnable(GL_COLOR);
	
	glPushMatrix();
		glColor3f((float)(color.r / 255.f), (float)(color.g / 255.f), (float)(color.b / 255.f));
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation.roll, 1, 0, 0);
		glRotatef(rotation.pitch, 0, 1, 0);
		glRotatef(rotation.yaw, 0, 0, 1);
		glScalef(size * 3, size * 3, size * 25);
		glutSolidCube(0.002f);
	glPopMatrix();

	position += offset * (0.002f + (size / 8.f));
	
	glPushMatrix(); // cone
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation.roll, 1, 0, 0);
		glRotatef(rotation.pitch, 0, 1, 0);
		glRotatef(rotation.yaw, 0, 0, 1);
		glScalef(size, size, size);
		glutSolidCone(0.01f, 0.03f, 10, 1);
	glPopMatrix();

	glDisable(GL_COLOR);
}
