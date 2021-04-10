#include "UIMenu.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Camera.h"
#include "GL/freeglut.h"
#include "GLUTCallbacks.h"
#include "Mesh.h"
#include "OpenGL.h"

UIMenu* UIMenu::inst = nullptr;

UIMenu::UIMenu()
{
	int shapeSubMenu = glutCreateMenu(GLUTCallbacks::OpenMenu);
	glutAddMenuEntry("Cube", ADD_SHAPE_CUBE);
	glutAddMenuEntry("Donut", ADD_SHAPE_DONUT);
	glutAddMenuEntry("Sword", ADD_SHAPE_SWORD);
	glutAddMenuEntry("Character", ADD_SHAPE_CHARACTER);
	glutAddMenuEntry("Room", ADD_SHAPE_ROOM);

	int viewSubMenu = glutCreateMenu(GLUTCallbacks::OpenMenu);
	glutAddMenuEntry("Perspective", VIEW_PERSPECTIVE);
	glutAddMenuEntry("Orthographic", VIEW_ORTHOGRAPHIC);

	int renderSubMode = glutCreateMenu(GLUTCallbacks::OpenMenu);
	glutAddMenuEntry("Lighting", SCENE_RENDER_LIGHTING);
	glutAddMenuEntry("Color", SCENE_RENDER_COLOR);
	glutAddMenuEntry("Textured", SCENE_RENDER_TEXURED);

	int meshEditMode = glutCreateMenu(GLUTCallbacks::OpenMenu);
	glutAddMenuEntry("Selection", MESH_EDIT_MODE_SELECTION);
	glutAddMenuEntry("Vertex Edit", MESH_EDIT_MODE_VERTEX);
	glutAddMenuEntry("Face Edit (BROKEN!)", MESH_EDIT_MODE_FACE);

	int menuID = glutCreateMenu(GLUTCallbacks::OpenMenu);
	glutAddSubMenu("Add Shape", shapeSubMenu);
	glutAddSubMenu("View", viewSubMenu);
	glutAddSubMenu("Render", renderSubMode);
	glutAddSubMenu("Mesh Edit Mode", meshEditMode);
	glutAddMenuEntry("Export Selected", MESH_EXPORT);
	glutAddMenuEntry("Delete Selected", DELETE_SELECTED_MESH);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

UIMenu::~UIMenu()
{
}

void UIMenu::HandleClick(int index)
{
	Mesh* newMesh = nullptr;
	
	switch (index)
	{
	case ADD_SHAPE_CUBE:
		newMesh = new Mesh("Cube", "", Vector3(), Rotation());
		break;
	case ADD_SHAPE_DONUT:
		newMesh = new Mesh("Donut", "", Vector3(), Rotation());
		break;
	case ADD_SHAPE_SWORD:
		newMesh = new Mesh("Sword", "Data/Textures/TextureWeapon1.tga", Vector3(), Rotation());
		break;
	case ADD_SHAPE_CHARACTER:
		newMesh = new Mesh("Character", "Data/Textures/blacksmith.tga", Vector3(), Rotation());
		break;
	case ADD_SHAPE_ROOM:
		newMesh = new Mesh("Room", "Data/Textures/RoomTexture.tga", Vector3(), Rotation());
		break;
	case VIEW_PERSPECTIVE:
		Camera::GetActiveCam()->SetCameraView(CameraView::PERSPECTIVE);
		break;
	case VIEW_ORTHOGRAPHIC:
		Camera::GetActiveCam()->SetCameraView(CameraView::ORTHOGRAPHIC);
		break;
	case SCENE_RENDER_LIGHTING:
		OpenGL::GetInst()->SetRenderMode(RenderMode::RENDER_LIGHTING);
		break;
	case SCENE_RENDER_COLOR:
		OpenGL::GetInst()->SetRenderMode(RenderMode::RENDER_COLOR);
		break;
	case SCENE_RENDER_TEXURED:
		OpenGL::GetInst()->SetRenderMode(RenderMode::RENDER_TEXTURE);
		break;
	case MESH_EDIT_MODE_SELECTION:
		OpenGL::GetInst()->SetEditMode(EditMode::MODE_SELECTION);
		break;
	case MESH_EDIT_MODE_VERTEX:
		OpenGL::GetInst()->SetEditMode(EditMode::MODE_MESH_VERTEX);
		break;
	case MESH_EDIT_MODE_FACE:
		OpenGL::GetInst()->SetEditMode(EditMode::MODE_MESH_FACE);
		break;
	case DELETE_SELECTED_MESH:
		if (OpenGL::GetInst()->GetActiveObject() != nullptr)
		{
			delete OpenGL::GetInst()->GetActiveObject();
			OpenGL::GetInst()->SetActiveObject(nullptr);
		}
		break;
	case MESH_EXPORT:
		if ((Mesh*)OpenGL::GetInst()->GetActiveObject() != nullptr)
			((Mesh*)OpenGL::GetInst()->GetActiveObject())->Export();
		break;
	}
}