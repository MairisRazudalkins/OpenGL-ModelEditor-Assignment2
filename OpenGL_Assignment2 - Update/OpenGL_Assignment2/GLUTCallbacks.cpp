#include "GLUTCallbacks.h"

#include <cstdio>
#include <iostream>


#include "Input.h"
#include "OpenGL.h"
#include "UIMenu.h"

namespace GLUTCallbacks
{
	namespace
	{
		OpenGL* openGL = nullptr;
		float lastUpdateTime;
	}

	void Init(OpenGL* gl)
	{
		openGL = gl;
	}

	void Display()
	{
		if (openGL != nullptr)
		{
			openGL->Render();
		}
	}

	void Timer(int prefferedRefresh)
	{
		if (openGL != nullptr)
		{
			int updateTime = glutGet(GLUT_ELAPSED_TIME);

			openGL->Update((float)(updateTime - lastUpdateTime) / 1000.f);
			lastUpdateTime = updateTime;
			updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;

			glutTimerFunc(prefferedRefresh - updateTime, GLUTCallbacks::Timer, prefferedRefresh);
		}

		Input::UpdateScrollDir(SCROLL_NONE);
	}

	void OpenMenu(int index)
	{
		UIMenu::GetMenu()->HandleClick(index);
		glutPostRedisplay();
	}

	void KeyEventDown(unsigned char key, int x, int y)
	{
		Input::OnKeyInput(key, KeyState::KEY_DOWN);
	}

	void KeyEventUp(unsigned char key, int x, int y)
	{
		Input::OnKeyInput(key, KeyState::KEY_UP);
	}

	void SpecialKeyEventDown(int key, int x, int y)
	{
		if (key == GLUT_KEY_SHIFT_L)
			Input::SetIsShiftDown(true);
	}

	void SpecialKeyEventUp(int key, int x, int y)
	{
		if (key == GLUT_KEY_SHIFT_L)
			Input::SetIsShiftDown(false);
	}

	void UpdateMousePos(int x, int y)
	{
		Input::UpdateMousePos(x, y);
	}

	void MouseKeyEvent(int button, int buttonState, int x, int y)
	{
		Input::UpdateMousePos(x, y);
		Input::UpdateMouseKey(button, (KeyState)!buttonState);
	}

	void MouseWheelEvent(int button, int dir, int x, int y)
	{
		Input::UpdateScrollDir((Scroll)dir);
	}
}

