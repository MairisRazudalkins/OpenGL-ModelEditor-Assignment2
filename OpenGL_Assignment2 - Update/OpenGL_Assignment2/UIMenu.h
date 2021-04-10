#pragma once

enum MenuType
{
	ADD_SHAPE,
	ADD_SHAPE_CUBE,
	ADD_SHAPE_DONUT,
	ADD_SHAPE_SWORD,
	ADD_SHAPE_CHARACTER,
	ADD_SHAPE_ROOM,

	MESH_EDIT_MODE_SELECTION,
	MESH_EDIT_MODE_VERTEX,
	MESH_EDIT_MODE_FACE,
	
	VIEW,
	VIEW_PERSPECTIVE,
	VIEW_ORTHOGRAPHIC,

	SCENE_RENDER_LIGHTING,
	SCENE_RENDER_COLOR,
	SCENE_RENDER_TEXURED,

	DELETE_SELECTED_MESH,

	MESH_EXPORT
};

class UIMenu
{
	static UIMenu* inst;
	
public:
	UIMenu();
	~UIMenu();

	void HandleClick(int index);

	static UIMenu* GetMenu() { return inst != nullptr ? inst : inst = new UIMenu(); }
};