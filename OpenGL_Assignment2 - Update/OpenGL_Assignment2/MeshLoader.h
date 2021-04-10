#pragma once
#include "Structs.h"

#ifndef MeshLoader
#define Loader

namespace MeshLoader
{
	MeshData* Load(char* path);

	MeshData* LoadObj(std::string name);
	void ExportMesh(MeshData* meshData);
};

#endif