#pragma once

#ifndef ListNode
#define List

#include "Gizmo.h"
#include "SceneObject.h"

class Mesh;

struct ListNode
{
    SceneObject* data;
    ListNode* nextNode;

    ListNode()
    {
        data = nullptr;
        nextNode = nullptr;
    }

    ListNode(SceneObject* obj)
    {
        data = obj;
        nextNode = nullptr;
    }
};

class SceneObjectList : public Object
{
	void DeleteList();

	ListNode* root;

public:
	SceneObjectList();
	~SceneObjectList();

	ListNode* AddNode(SceneObject* data);
	ListNode* InsertFirst(ListNode** head, SceneObject* data);

	ListNode* GetNode(int pos);
	int FindIndex(ListNode* node);
	ListNode* Find(SceneObject* data);
	
	void InsertAfter(ListNode* lastNode, SceneObject* data);
	void DeleteAfter(ListNode* node);
	void DeleteNode(ListNode* node);
	void DeleteNodeAt(int pos);

	void AssignColors(SceneObject* selectedMesh = nullptr);
	void CheckHit();
	
	void Render() override;
	void Update(float deltaTime) override;

	MoveAxis lastMove;
};

#endif