#include "SceneObjectList.h"
#include <iomanip>
#include <iostream>

#include "Gizmo.h"
#include "Input.h"
#include "Mesh.h"
#include "OpenGL.h"

SceneObjectList::SceneObjectList()
{
	root = nullptr;
}

SceneObjectList::~SceneObjectList()
{
	DeleteList();
}

ListNode* SceneObjectList::AddNode(SceneObject* data)
{
	ListNode* newNode = new ListNode(data);
	ListNode* last = root;

	newNode->data = data;
	newNode->nextNode = nullptr;

	if (root == nullptr)
	{
		root = newNode;
		return newNode;
	}

	while (last->nextNode != nullptr)
	{
		last = last->nextNode;
	}

	last->nextNode = newNode;
	return newNode;
}

ListNode* SceneObjectList::InsertFirst(ListNode** head, SceneObject* data)
{
	ListNode* newNode = new ListNode();

	newNode->data = data;
	newNode->nextNode = *head;
	*head = newNode;

	return newNode;
}

void SceneObjectList::InsertAfter(ListNode* lastNode, SceneObject* data)
{
	if (lastNode == nullptr)
	{
		std::cout << "LastNode cannot be null!\n";
		return;
	}

	ListNode* newNode = new ListNode();
	newNode->data = data;
	newNode->nextNode = lastNode->nextNode;
	lastNode->nextNode = newNode;
}

void SceneObjectList::DeleteList()
{
	ListNode* pTemp = root;
	ListNode* next;

	while (pTemp != nullptr)
	{
		next = pTemp->nextNode;
		delete pTemp;
		pTemp = next;
	}

	delete root;
	root = nullptr;
}

void SceneObjectList::DeleteAfter(ListNode* node)
{
	ListNode* pTemp;

	if (node != nullptr && node->nextNode != nullptr)
	{
		pTemp = node->nextNode;
		node->nextNode = pTemp->nextNode;

		delete pTemp;
	}
}

void SceneObjectList::DeleteNode(ListNode* node)
{
	int index = FindIndex(node);

	if (index != -1)
	{
		ListNode* previousNode = GetNode(index - 1);
		DeleteAfter(previousNode);
		return;
	}

	std::cout << "Failed to delete node" << std::endl;
}

void SceneObjectList::DeleteNodeAt(int pos)
{
	ListNode* previousNode = GetNode(pos - 1);
	DeleteAfter(previousNode);
}

ListNode* SceneObjectList::GetNode(int pos)
{
	ListNode* node = root;
	int count = 0;

	while (node != nullptr)
	{
		if (count == pos)
			return node;

		count++;
		node = node->nextNode;
	}

	return nullptr;
}

int SceneObjectList::FindIndex(ListNode* node)
{
	ListNode* temp = root;
	int count = 0;
	
	while (node != nullptr)
	{
		if (temp == node)
			return count;

		count++;
		temp = temp->nextNode;
	}

	return -1;
}

ListNode* SceneObjectList::Find(SceneObject* data)
{
	ListNode* node = root;
	
	while (node != nullptr)
	{
		if (node->data == data)
			return node;

		node = node->nextNode;
	}

	return nullptr;
}

void SceneObjectList::Render()
{
	ListNode* node = root;
	
	while (node != nullptr)
	{
		node->data->Render();
		node = node->nextNode;
	}
}

void SceneObjectList::AssignColors(SceneObject* selectedObj)
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glEnable(GL_COLOR);
	
	int offset = 1;
	Color color;
	
	if (selectedObj != nullptr)
	{
		OpenGL::GetInst()->GetActiveObject()->RenderToBackBuffer(offset, color);
		
		if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_MESH_VERTEX)
			((Mesh*)selectedObj)->RenderVertsToBackBuffer(offset, color);

		if (((Mesh*)selectedObj)->GetSelectedIndex() != -1 || OpenGL::GetInst()->GetEditMode() == EditMode::MODE_SELECTION)
			Gizmo::GetGizmo()->RenderToBackBuffer(offset, color);
	}
	else
	{
		ListNode* node = root;

		while (node != nullptr)
		{
			node->data->RenderToBackBuffer(offset, color);
			node = node->nextNode;
		}
	}
	
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDisable(GL_COLOR);
}

void SceneObjectList::CheckHit()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int y_new = viewport[3] - Input::GetMousePos().y;

	float data[3];
	glReadPixels(Input::GetMousePos().x, y_new, 1, 1, GL_RGB, GL_FLOAT, data);

	Color color(data[0] * 255.f, data[1] * 255.f, data[2] * 255.f);

	int colorID = color.GetColorID();

	Object* activeObj = OpenGL::GetInst()->GetActiveObject();
	int offset = 1;

	if (activeObj != nullptr)
	{
		if (Gizmo::GetGizmo()->IsInteracting())
		{
			Gizmo::GetGizmo()->MoveGizmo(Gizmo::GetGizmo()->GetMoveAxis());
			return;
		}

		offset++;
		
		if (Color(255, 255, 254).GetColorID() == colorID)
			Gizmo::GetGizmo()->SetInteracting(true, MoveAxis::MOVE_X);
		else if (Color(255, 255, 253).GetColorID() == colorID)
			Gizmo::GetGizmo()->SetInteracting(true, MoveAxis::MOVE_Y);
		else if (Color(255, 255, 252).GetColorID() == colorID)
			Gizmo::GetGizmo()->SetInteracting(true, MoveAxis::MOVE_Z);
		else if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_MESH_VERTEX && (Mesh*)activeObj != nullptr)
			((Mesh*)activeObj)->CheckVertHit(offset, colorID);
		else if (OpenGL::GetInst()->GetEditMode() == EditMode::MODE_MESH_FACE && (Mesh*)activeObj != nullptr)
			((Mesh*)activeObj)->CheckFaceHit(offset, colorID);
		else if (colorID != OpenGL::GetInst()->GetActiveObject()->GetBackBufferID() && OpenGL::GetInst()->GetEditMode() == EditMode::MODE_SELECTION)
			OpenGL::GetInst()->SetActiveObject(nullptr);
	}
	else
	{ 
		ListNode* node = root;

		while (node != nullptr)
		{
			if (colorID == node->data->GetBackBufferID())
			{
				OpenGL::GetInst()->SetActiveObject(node->data);
				Gizmo::GetGizmo()->SetPosition(node->data->GetPosition());
				Gizmo::GetGizmo()->SetVisibility(true);
				return;
			}
			
			node = node->nextNode;
		}
	}
}

void SceneObjectList::Update(float deltaTime)
{
	ListNode* node = root;

	while (node != nullptr)
	{
		node->data->Update(deltaTime);
		node = node->nextNode;
	}
}
