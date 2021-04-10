#pragma once
#include <string>

#include "SceneObject.h"

class Texture2D;

class Mesh : public SceneObject
{
	Material* material;
	Texture2D* texture;

	MeshData* meshData;
	
	int selectedIndex = -1;
	
public:
	Mesh(std::string name, std::string texturePath, Vector3 position, Rotation rotation);
	~Mesh();
	
	void Render() override;
	void Update(float deltaTime) override;

	void RenderVerts();
	void RenderEdges();
	void RenderVertsToBackBuffer(int& index, Color& color);
	
	bool CheckVertHit(int& offset, int colorID);
	bool CheckFaceHit(int& offset, int colorID);

	void RenderToBackBuffer(int& indexOffset, Color& color) override;
	void MoveSelectedVertex(Vector3 position);
	void MoveSelectedFace(Vector3 position);
	void Export();

	int GetSelectedIndex() { return selectedIndex; }
	Vector3 GetSelectedIndexPos() { return meshData->vertices[selectedIndex]; }
};
