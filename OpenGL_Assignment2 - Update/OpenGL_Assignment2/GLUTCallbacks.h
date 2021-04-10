#pragma once
class OpenGL;

namespace GLUTCallbacks
{
	void Init(OpenGL* gl);
	void Display();
	void Timer(int prefferedRefresh);

	void OpenMenu(int index);

	void KeyEventDown(unsigned char key, int x, int y);
	void KeyEventUp(unsigned char key, int x, int y);
	
	void SpecialKeyEventDown(int key, int x, int y);
	void SpecialKeyEventUp(int key, int x, int y);
	void UpdateMousePos(int x, int y);

	void MouseKeyEvent(int button, int buttonState, int x, int y);
	void MouseWheelEvent(int button, int dir, int x, int y);
}