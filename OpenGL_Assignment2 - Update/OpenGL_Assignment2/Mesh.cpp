#include "Mesh.h"

#include "Camera.h"
#include "Gizmo.h"
#include "Input.h"
#include "Texture2D.h"
#include "MeshLoader.h"

#include <iostream>

#include "OpenGL.h"

Mesh::Mesh(std::string name, std::string texturePath, Vector3 position, Rotation rotation) : SceneObject(position, rotation)
{
	meshData = MeshLoader::LoadObj(name);
	material = new Material(Vector4(1.f, 1.f, 1.f, 1.f), Vector4(1.f, 1.f, 1.f, 1.f), Vector4(1.f, 1.f, 1.f, 1.f), 0.0f);
	
	texture = new Texture2D();
	texture->LoadTextureTGA(texturePath.c_str());
}

Mesh::~Mesh()
{
	delete meshData;
	meshData = nullptr;

	delete material;
	material = nullptr;

	delete texture;
	texture = nullptr;

	delete this;
}

void Mesh::Render()
{
	if (meshData != nullptr && IsVisible())
	{		
		if (IsSelected() && OpenGL::GetInst()->GetEditMode() == MODE_MESH_VERTEX)
		{
			RenderVerts();
			RenderEdges();
		}

		if (OpenGL::GetInst()->GetRenderMode() == RENDER_COLOR)
			RenderEdges();

		glMatrixMode(GL_MODELVIEW);
		glShadeModel(GL_SMOOTH);
		
		if (OpenGL::GetInst()->GetRenderMode() == RENDER_LIGHTING || OpenGL::GetInst()->GetRenderMode() == RENDER_TEXTURE)
		{		
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);

			if (OpenGL::GetInst()->GetRenderMode() == RENDER_TEXTURE)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture->GetID());
			}
		}
		else if (OpenGL::GetInst()->GetRenderMode() == RENDER_COLOR)
		{
			glEnable(GL_COLOR);
		}
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glTexCoordPointer(2, GL_FLOAT, 0, meshData->texCoords);
		glVertexPointer(3, GL_FLOAT, 0, meshData->vertices);
		glNormalPointer(GL_FLOAT, 0, meshData->normals);
		
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &(material->diffuse.x));
		glMaterialfv(GL_FRONT, GL_AMBIENT, &(material->ambient.x));
		glMaterialfv(GL_FRONT, GL_SPECULAR, &(material->specular.x));
		glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
		
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < meshData->indexCount; i++)
		{
			Vector3 vPos = meshData->vertices[meshData->vertexIndicies[i]];
			meshData->vertices[meshData->vertexIndicies[i]] += GetPosition();
			glPushMatrix();
				glRotated(GetRotation().yaw, 1.f, 1.f, 1.f);
				glColor3f(0.4f, 0.4f, 0.4f);
 				glTexCoord2fv(&meshData->texCoords[meshData->texCoordIndicies[i]].u);
				glNormal3fv(&meshData->normals[meshData->normalIndicies[i]].x);
				glVertex3fv(&meshData->vertices[meshData->vertexIndicies[i]].x);
			glPopMatrix();
		
			meshData->vertices[meshData->vertexIndicies[i]] = vPos;
		}
		glEnd();
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glDisable(GL_TEXTURE_2D);
		
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_COLOR);

		glFlush();
	}
}

void Mesh::Update(float deltaTime)
{

	if (Input::WasKeyDown('e'))
		MeshLoader::ExportMesh(meshData);

	if (Input::WasKeyDown('p'))
	{
		meshData->vertices[meshData->vertexIndicies[1]] += Vector3(0.1f, 0.f, 0.f);
		meshData->vertices[meshData->vertexIndicies[2]] += Vector3(0.1f, 0.f, 0.f);
		meshData->vertices[meshData->vertexIndicies[4]] += Vector3(0.1f, 0.f, 0.f);
		meshData->vertices[meshData->vertexIndicies[5]] += Vector3(0.1f, 0.f, 0.f);
	}
	
	if (Input::WasKeyDown('o'))
	{
		meshData->vertices[meshData->vertexIndicies[1]] -= Vector3(0.1f, 0.f, 0.f);
		meshData->vertices[meshData->vertexIndicies[2]] -= Vector3(0.1f, 0.f, 0.f);
		meshData->vertices[meshData->vertexIndicies[4]] -= Vector3(0.1f, 0.f, 0.f);
		meshData->vertices[meshData->vertexIndicies[5]] -= Vector3(0.1f, 0.f, 0.f);
	}

	SetRotation(GetRotation() + Rotation(1.f, 1.f, 1.f));
}

void Mesh::RenderToBackBuffer(int& indexOffset, Color& color)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, meshData->vertices);
	glNormalPointer(GL_FLOAT, 0, meshData->normals);

	color.r = (indexOffset & 0x000000FF) >> 0;
	color.g = (indexOffset & 0x0000FF00) >> 8;
	color.b = (indexOffset & 0x00FF0000) >> 16;
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < meshData->indexCount; i++)
	{
		Vector3 pos = meshData->vertices[meshData->vertexIndicies[i]];
		meshData->vertices[meshData->vertexIndicies[i]] += GetPosition();
		
		glPushMatrix();
			glColor3f((float)(color.r / 255.f), (float)(color.g / 255.f), (float)(color.b / 255.f));
			glTexCoord2fv(&meshData->texCoords[meshData->texCoordIndicies[i]].u);
			glNormal3fv(&meshData->normals[meshData->normalIndicies[i]].x);
			glVertex3fv(&meshData->vertices[meshData->vertexIndicies[i]].x);
		glPopMatrix();

		meshData->vertices[meshData->vertexIndicies[i]] = pos;

		if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_MESH_FACE)
		{
			indexOffset++;
			color.r = (indexOffset & 0x000000FF) >> 0;
			color.g = (indexOffset & 0x0000FF00) >> 8;
			color.b = (indexOffset & 0x00FF0000) >> 16;
		}
	}
	glEnd();

	SetBackBufferID(color.GetColorID());

	indexOffset += 1;
	color.r = (indexOffset & 0x000000FF) >> 0;
	color.g = (indexOffset & 0x0000FF00) >> 8;
	color.b = (indexOffset & 0x00FF0000) >> 16;
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::MoveSelectedVertex(Vector3 position)
{
	meshData->vertices[selectedIndex] = position - GetPosition();
}

void Mesh::MoveSelectedFace(Vector3 position)
{
	for (int i = 0; i < 4; i++)
	{
		Vector3 posOffset = meshData->vertices[selectedIndex + i] - position;
		meshData->vertices[selectedIndex + i] = (position - posOffset);
	}
}

void Mesh::Export()
{
	MeshLoader::ExportMesh(meshData);
}

void Mesh::RenderVertsToBackBuffer(int& index, Color& color)
{
	for (int i = 0; i < meshData->indexCount; i++)
	{
		Vector3 p = meshData->vertices[meshData->vertexIndicies[i]] + GetPosition();

		color.r = (index & 0x000000FF) >> 0;
		color.g = (index & 0x0000FF00) >> 8;
		color.b = (index & 0x00FF0000) >> 16;
		
		glPushMatrix();
			glColor3f((float)(color.r / 255.f), (float)(color.g / 255.f), (float)(color.b / 255.f));
			glTranslatef(p.x, p.y, p.z);
			glutSolidCube(0.005f);
		glPopMatrix();

		index++;
	}
}

bool Mesh::CheckVertHit(int& offset, int colorID)
{
	Color color;
	
	for (int i = 0; i < meshData->indexCount; i++)
	{
		color.r = (offset & 0x000000FF) >> 0;
		color.g = (offset & 0x0000FF00) >> 8;
		color.b = (offset & 0x00FF0000) >> 16;
		
		if (color.GetColorID() == colorID)
		{
			selectedIndex = meshData->vertexIndicies[i];
			Gizmo::GetGizmo()->SetVisibility(true);
			Gizmo::GetGizmo()->SetPosition(meshData->vertices[meshData->vertexIndicies[i]] + GetPosition());
			return true;
		}
		offset++;
	}

	selectedIndex = -1;
	Gizmo::GetGizmo()->SetVisibility(false);
	return false;
}

bool Mesh::CheckFaceHit(int& offset, int colorID)
{
	Color color;

	for (int i = 0; i < meshData->indexCount; i++)
	{
		color.r = (offset & 0x000000FF) >> 0;
		color.g = (offset & 0x0000FF00) >> 8;
		color.b = (offset & 0x00FF0000) >> 16;

		if (color.GetColorID() == colorID)
		{
			selectedIndex = meshData->vertexIndicies[i];
		
			Vector3 gizmoPos;
			
			for (int i = 0; i < 4; i++)
			{
				gizmoPos += meshData->vertices[meshData->vertexIndicies[selectedIndex + i]];
			}
			
			Gizmo::GetGizmo()->SetVisibility(true);
			Gizmo::GetGizmo()->SetPosition(gizmoPos / 4);
			return true;
		}

		offset++;
	}

	return false;
}

void Mesh::RenderVerts()
{
	glEnable(GL_COLOR);

	for (int i = 0; i < meshData->indexCount; i++)
	{
		Vector3 p = meshData->vertices[meshData->vertexIndicies[i]] + GetPosition();

		glPushMatrix();
			if (meshData->vertexIndicies[i] == selectedIndex)
				glColor3f(0.f, 1.f, 0.f);
			else
				glColor3f(1.f, 0.f, 0.f);
			glTranslatef(p.x, p.y, p.z);
			glutSolidCube(0.005f);
		glPopMatrix();
	}

	glDisable(GL_COLOR);
}

void Mesh::RenderEdges()
{
	glEnable(GL_COLOR);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < meshData->indexCount; i++)
	{
		Vector3 vPos = meshData->vertices[meshData->vertexIndicies[i]];
		meshData->vertices[meshData->vertexIndicies[i]] += GetPosition();
	
		glPushMatrix();
			if (IsSelected())
				glColor3f(0.f, 1.f, 0.f);
			else
				glColor3f(1.f, 1.f, 1.f);
	
			glVertex3fv(&meshData->vertices[meshData->vertexIndicies[i]].x);
		glPopMatrix();
	
		meshData->vertices[meshData->vertexIndicies[i]] = vPos;
	}
	glEnd();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_COLOR);
}
