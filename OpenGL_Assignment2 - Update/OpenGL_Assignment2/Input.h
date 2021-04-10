#pragma once
#include "Structs.h"

enum MouseKey
{
	LEFT_MOUSE_BUTTON = 0,
	MIDDLE_MOUSE_BUTTON = 1,
	RIGHT_MOUSE_BUTTON = 2
};

enum KeyState
{
	KEY_UP = 0,
	KEY_DOWN = 1
};

enum Scroll
{
	SCROLL_DOWN = -1,
	SCROLL_NONE = 0,
	SCROLL_UP = 1
};

enum SpecialKey
{
	SKEY_SHIFT = 0,
	SKEY_CRTL = 1,
	SKEY_ALT = 2
};

class Input
{
	Input();
	
	static Input* inst;

	Vector2 mousePos;
	Scroll currentScrollState = SCROLL_NONE;
	
	bool keyboardState[256] { KEY_UP };
	bool prevKeyboardState[256] { KEY_UP };

	bool mouseKeyState[3] { KEY_UP };
	bool prevMouseKeyState[3] { KEY_UP };

	bool isShiftDown = false;

public:
	~Input();

	static Input* GetInst() { return inst != nullptr ? inst : inst = new Input(); }

	static void UpdateMousePos(int x, int y) { GetInst()->mousePos.x = x; GetInst()->mousePos.y = y; }
	static void OnKeyInput(unsigned char key, KeyState state) { GetInst()->prevKeyboardState[key] = GetInst()->keyboardState[key]; GetInst()->keyboardState[key] = state; }
	static void UpdateMouseKey(int button, KeyState state) { GetInst()->prevMouseKeyState[button] = GetInst()->mouseKeyState[button]; GetInst()->mouseKeyState[button] = state; }
	static void UpdateScrollDir(int dir) { GetInst()->currentScrollState = (Scroll)dir; }
	static void SetIsShiftDown(bool isDown) { GetInst()->isShiftDown = isDown; }
	
	static bool IsKeyDown(unsigned char key) { return GetInst()->keyboardState[key]; }
	static bool WasKeyDown(unsigned char key);
	static bool IsShiftDown() { return GetInst()->isShiftDown; }
 
	static bool IsMouseKeyDown(MouseKey key) { return GetInst()->mouseKeyState[key]; }
	static bool WasMouseKeyDown(MouseKey key);

	static Vector2 GetMousePos() { return GetInst()->mousePos; }
	static Scroll GetScrollDir() { return GetInst()->currentScrollState; }
};