#include <cstdio>
#include <cmath>

#include "OpenGL.h"
#include "SceneObjectList.h"
#include "Mesh.h"
#include "Structs.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Constants.h"
#include "Gizmo.h"
#include "Input.h"
#include "UIMenu.h"

OpenGL* OpenGL::inst = nullptr;

OpenGL::OpenGL(int argc, char* argv[])
{
	inst = this;
	
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition((1920 / 2) - (SCREEN_WIDTH / 2), (1080 / 2) - (SCREEN_HEIGHT / 2));

	glutCreateWindow("OPEN GL");

	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	
	glutKeyboardFunc(GLUTCallbacks::KeyEventDown);
	glutKeyboardUpFunc(GLUTCallbacks::KeyEventUp);
	glutMouseFunc(GLUTCallbacks::MouseKeyEvent);
	glutMotionFunc(GLUTCallbacks::UpdateMousePos);
	glutPassiveMotionFunc(GLUTCallbacks::UpdateMousePos);
	glutMouseWheelFunc(GLUTCallbacks::MouseWheelEvent);
	glutSpecialFunc(GLUTCallbacks::SpecialKeyEventDown);
	glutSpecialUpFunc(GLUTCallbacks::SpecialKeyEventUp);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 800, 800);
	gluPerspective(45, 1, 0.01f, 100.f);
	
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	InitScene();

	glutMainLoop();
}

OpenGL::~OpenGL()
{
	delete SceneObject::GetSceneObjects();
	delete lighting;
}

void OpenGL::Render()
{	
	SceneObjectList* list = SceneObject::GetSceneObjects();

	if (Input::IsMouseKeyDown(LEFT_MOUSE_BUTTON))
	{	
		list->AssignColors(activeObject);
		list->CheckHit();
	}
	else
	{
		Gizmo::GetGizmo()->SetInteracting(false);
	}
	
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawGrid();
	list->Render();
	Gizmo::GetGizmo()->Render();
	
	glFlush();
	glutSwapBuffers();
}

void OpenGL::Update(float deltaTime)
{
	SceneObjectList* list = SceneObject::GetSceneObjects();
	list->Update(deltaTime);

	if (Input::WasKeyDown('r') && (Mesh*)activeObject != nullptr)
		SetEditMode(editMode == MODE_SELECTION ? MODE_MESH_VERTEX : MODE_SELECTION);
	
	if (Input::WasKeyDown('t') && (Mesh*)activeObject != nullptr)
		SetEditMode(editMode == MODE_MESH_VERTEX ? MODE_MESH_FACE : MODE_MESH_VERTEX);

	if (Input::WasKeyDown('q'))
	{
		int index = ((int)renderMode) + 1;

		if (index > 2)
			index = 0;

		renderMode = (RenderMode)index;
	}
	
	glutPostRedisplay();
}

void OpenGL::InitScene()
{
	camera = new Camera(Vector3(0, 3, 3), Rotation(0.f, 45.f, 0.f));
	//Mesh* obj = new Mesh("Data/Meshes/Cube", "", Vector3(0, 0, 0), Rotation());
	//activeMesh = obj;

	lightPos = Vector4(1.f, 1.f, 5.f, 1.f);
	lighting = new Lighting(Vector4(0.5f, 0.5f, 0.5f, 0.5f), Vector4(0.5f, 0.5f, 0.5f, 0.5f), Vector4(1.f, 1.f, 1.f, 1.f));

	glLightfv(GL_LIGHT0, GL_AMBIENT, &lighting->ambient.x);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &lighting->diffuse.x);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &lighting->specular.x);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPos.x);
	
	UIMenu::GetMenu();
}

void OpenGL::SetActiveObject(SceneObject* obj)
{
	if (activeObject != nullptr)
		activeObject->SetSelected(false);
	
	activeObject = obj;

	if (activeObject != nullptr)
	{
		activeObject->SetSelected(true);
		Gizmo::GetGizmo()->SetPosition(obj->GetPosition());
	}
	else
	{
		Gizmo::GetGizmo()->SetVisibility(false);
	}
}

void OpenGL::SetEditMode(EditMode editMode)
{
	this->editMode = editMode;

	if (editMode == EditMode::MODE_SELECTION)
	{
		Gizmo::GetGizmo()->SetPosition(activeObject->GetPosition());
		Gizmo::GetGizmo()->SetVisibility(true);
	}
	else if((editMode == EditMode::MODE_MESH_VERTEX || EditMode::MODE_MESH_FACE) && (Mesh*)activeObject)
	{
		Gizmo::GetGizmo()->SetVisibility(true);
	}
	else
	{
		Gizmo::GetGizmo()->SetVisibility(false);
	}
}

void OpenGL::DrawGrid()
{
	glBegin(GL_LINES);
	glEnable(GL_COLOR);
		for (int i = -4; i <= 4; i++)
		{
			glColor3f(1, 1, 1);
			glVertex3f(i * gridSize, 0, -4 * gridSize);
			glVertex3f(i * gridSize, 0, 4 * gridSize);
			glVertex3f(-4 * gridSize, 0, i * gridSize);
			glVertex3f(4 * gridSize, 0, i * gridSize);
		}
	glDisable(GL_COLOR);
	glEnd();
}