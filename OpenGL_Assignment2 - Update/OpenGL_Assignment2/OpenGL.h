#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "Object.h"
#include "SceneObject.h"

class Mesh;
class Camera;
class SceneObjectList;
struct Vector3;

enum EditMode
{
	MODE_SELECTION,
	MODE_MESH_VERTEX,
	MODE_MESH_FACE
};

enum RenderMode
{
	RENDER_LIGHTING,
	RENDER_COLOR,
	RENDER_TEXTURE
};

class OpenGL : public Object
{
	static OpenGL* inst;
	const float gridSize = 0.2f;
	
	Camera* camera;
	SceneObject* activeObject;
	EditMode editMode;
	RenderMode renderMode = RENDER_COLOR;

public:
	OpenGL(int argc, char* argv[]);
	~OpenGL();

	static OpenGL* GetInst() { return inst; }
	
	void Render() override;
	void Update(float deltaTime) override;	
	void InitScene();

	SceneObject* GetActiveObject() { return activeObject; }
	void SetActiveObject(SceneObject* obj);

	EditMode GetEditMode() { return editMode; }
	void SetEditMode(EditMode editMode);
	void DrawGrid();

	RenderMode GetRenderMode() { return renderMode; }
	void SetRenderMode(RenderMode renderMode) { this->renderMode = renderMode; }

	Vector4 lightPos; // should have put it into a class
	Lighting* lighting;
};