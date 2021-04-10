#include "Input.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

Input* Input::inst = nullptr;

Input::Input()
{
	
}

Input::~Input()
{
	delete inst;
	inst = nullptr;
}

bool Input::WasKeyDown(unsigned char key)
{
	Input* inst = GetInst();

	if (inst->prevKeyboardState[key] == KeyState::KEY_DOWN && inst->keyboardState[key] == KeyState::KEY_UP)
	{
		inst->prevKeyboardState[key] = inst->keyboardState[key];
		return true;
	}

	return false;
}

bool Input::WasMouseKeyDown(MouseKey key)
{
	Input* inst = GetInst();

	if (inst->prevMouseKeyState[key] == KeyState::KEY_DOWN && inst->mouseKeyState[key] == KeyState::KEY_UP)
	{
		inst->prevMouseKeyState[key] = inst->mouseKeyState[key];
		return true;
	}

	return false;
}
