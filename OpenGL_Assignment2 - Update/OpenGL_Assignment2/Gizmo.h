#pragma once
#include "SceneObject.h"

enum GizmoMode
{
	GIZMO_MOVEMENT,
	GIZMO_ROTATION,
	GIZMO_SCALE
};

enum MoveAxis
{
	MOVE_X,
	MOVE_Y,
	MOVE_Z,
	MOVE_ALL
};

class Gizmo : public SceneObject
{
	Gizmo();

	static Gizmo* inst;
	void DrawToBackBuffer();

	bool interacting = false;

	GizmoMode gizmoMode = GIZMO_MOVEMENT;
	MoveAxis moveAxis = MOVE_X;
	
	void DrawArrow(Vector3 position, Vector3 offset, Rotation rotation, Color color, float size);

public:
	~Gizmo();
	void Render() override;
	void Update(float deltaTime) override;
	void RenderToBackBuffer(int& indexOffset, Color& color) override;

	void MoveGizmo(MoveAxis axis);
	MoveAxis GetMoveAxis() { return moveAxis; }

	bool IsInteracting() { return interacting; }
	void SetInteracting(bool interacting, MoveAxis axis = MOVE_X);
	
	static Gizmo* GetGizmo() { return inst != nullptr ? inst : inst = new Gizmo(); }
};