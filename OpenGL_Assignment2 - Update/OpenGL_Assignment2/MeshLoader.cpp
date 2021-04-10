#include "MeshLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Mesh.h"

using namespace std;

namespace MeshLoader
{
	//void LoadVertices(ifstream& inFile, MeshData& mesh);
	//void LoadNormals(ifstream& inFile, MeshData& mesh);
	//void LoadIndices(ifstream& inFile, MeshData& mesh);
	//void LoadCoords(ifstream& inFile, MeshData& mesh);
	//
	//void LoadVertices(ifstream& inFile, MeshData& mesh)
	//{
	//	inFile >> mesh.vertexCount;
	//
	//	if (mesh.vertexCount > 0)
	//	{
	//		mesh.vertices = new Vector3[mesh.vertexCount];
	//
	//		for (int i = 0; i < mesh.vertexCount; i++)
	//		{
	//			inFile >> mesh.vertices[i].x;
	//			inFile >> mesh.vertices[i].y;
	//			inFile >> mesh.vertices[i].z;
	//		}
	//	}
	//}
	//
	//void LoadNormals(ifstream& inFile, MeshData& mesh)
	//{
	//	inFile >> mesh.normalCount;
	//	mesh.normals = new Vector3[mesh.normalCount];
	//
	//	if (mesh.normalCount > 0)
	//	{
	//		mesh.normals = new Vector3[mesh.normalCount];
	//
	//		for (int i = 0; i < mesh.normalCount; i++)
	//		{
	//			inFile >> mesh.normals[i].x;
	//			inFile >> mesh.normals[i].y;
	//			inFile >> mesh.normals[i].z;
	//		}
	//	}
	//}
	//
	//void LoadIndices(ifstream& inFile, MeshData& mesh)
	//{
	//	inFile >> mesh.indexCount;
	//
	//	if (mesh.indexCount > 0)
	//	{
	//		mesh.vertexIndicies = new GLushort[mesh.indexCount];
	//
	//		for (int i = 0; i < mesh.indexCount; i++)
	//		{
	//			inFile >> mesh.vertexIndicies[i];
	//		}
	//	}
	//}
	//
	//void LoadCoords(ifstream& inFile, MeshData& mesh)
	//{
	//	inFile >> mesh.texCoordCount;
	//
	//	if (mesh.texCoordCount > 0)
	//	{
	//		mesh.texCoords = new TexCoord[mesh.texCoordCount];
	//
	//		for (int i = 0; i < mesh.texCoordCount; i++)
	//		{
	//			inFile >> mesh.texCoords[i].u;
	//			inFile >> mesh.texCoords[i].v;
	//		}
	//	}
	//}
	//
	//MeshData* MeshLoader::Load(char* path)
	//{
	//	MeshData* mesh = new MeshData();
	//
	//	ifstream inFile;
	//
	//	inFile.open(path);
	//
	//	if (!inFile.good())
	//	{
	//		cerr << "Can't open mesh file " << path << endl;
	//		return nullptr;
	//	}
	//
	//	LoadVertices(inFile, *mesh);
	//	LoadCoords(inFile, *mesh);
	//	LoadNormals(inFile, *mesh);
	//	LoadIndices(inFile, *mesh);
	//
	//	return mesh;
	//}

	MeshData* LoadObj(std::string name)
	{
		ifstream inFile;
		std::string path;
		inFile.open(path = (char*)("Data/Meshes/" + name + ".obj").c_str());

		if (!inFile.good())
		{
			cerr << "Can't open mesh file " << path << endl;
			return nullptr;
		}

		std::vector<Vector3> tempVertices;
		std::vector<TexCoord> tempTexCoords;
		std::vector<Vector3> tempNormals;
		
		std::vector<GLushort> tempVertexIndicies;
		std::vector<GLushort> tempNormalIndicies;
		std::vector<GLushort> tempTexCoordIndicies;
		
		string line;
		float a, b, c;
		
		while (std::getline(inFile, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				sscanf_s(line.c_str(), "v %f %f %f", &a, &b, &c);
				tempVertices.push_back(Vector3(a, b, c));
			}
			else if (line.substr(0, 2) == "vt")
			{
				sscanf_s(line.c_str(), "vt %f %f", &a, &b);
				tempTexCoords.push_back(TexCoord(a, b));
			}
			else if (line.substr(0, 2) == "vn")
			{
				sscanf_s(line.c_str(), "vn %f %f %f", &a, &b, &c);
				tempNormals.push_back(Vector3(a, b, c));
			}
			else if (line.substr(0, 2) == "f ")
			{
				float vIndcies[4], nIndicies[4], tIndicies[4];
				
				sscanf_s(line.c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f", &vIndcies[0], &tIndicies[0], &nIndicies[0], &vIndcies[1], &tIndicies[1], &nIndicies[1], &vIndcies[2], &tIndicies[2], &nIndicies[2] /*,&vIndcies[3], &tIndicies[3], &nIndicies[3]*/);
				tempVertexIndicies.push_back(vIndcies[0] - 1); tempVertexIndicies.push_back(vIndcies[1] - 1); tempVertexIndicies.push_back(vIndcies[2] - 1); //tempVertexIndicies.push_back(vIndcies[3] - 1);
				tempNormalIndicies.push_back(nIndicies[0] - 1); tempNormalIndicies.push_back(nIndicies[1] - 1); tempNormalIndicies.push_back(nIndicies[2] - 1); //tempNormalIndicies.push_back(nIndicies[3] - 1);
				tempTexCoordIndicies.push_back(tIndicies[0] - 1); tempTexCoordIndicies.push_back(tIndicies[1] - 1); tempTexCoordIndicies.push_back(tIndicies[2] - 1); //tempTexCoordIndicies.push_back(tIndicies[3] - 1);
			}
		}

		inFile.close();

		MeshData* meshData = new MeshData(name,tempVertices.size(), tempNormals.size(), tempTexCoords.size(), tempVertexIndicies.size());
		
		std::copy(tempVertices.begin(), tempVertices.end(), meshData->vertices);
		std::copy(tempNormals.begin(), tempNormals.end(), meshData->normals);
		std::copy(tempTexCoords.begin(), tempTexCoords.end(), meshData->texCoords);
		
		std::copy(tempVertexIndicies.begin(), tempVertexIndicies.end(), meshData->vertexIndicies);
		std::copy(tempTexCoordIndicies.begin(), tempTexCoordIndicies.end(), meshData->texCoordIndicies);
		std::copy(tempNormalIndicies.begin(), tempNormalIndicies.end(), meshData->normalIndicies);

		return meshData;
	}

	void ExportMesh(MeshData* meshData)
	{
		ofstream outFile;

		outFile.open("Data/Meshes/ExportedMesh.obj");

		if (!outFile.is_open())
		{
			std::cout << "Failed to open export file!\n";
			return;
		}

		outFile << "# Mairis's OpenGL Export File\no" << meshData->name << std::endl;
		
		for (int i = 0; i < meshData->vertexCount; i++)
		{
			Vector3 vert = meshData->vertices[i];
			outFile << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
		}

		for (int i = 0; i < meshData->texCoordCount; i++)
		{
			TexCoord coord = meshData->texCoords[i];
			outFile << "vt " << coord.u << " " << coord.v << std::endl;
		}

		for (int i = 0; i < meshData->normalCount; i++)
		{
			Vector3 norm = meshData->normals[i];
			outFile << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
		}

		for (int i = 0; i < meshData->indexCount; i += 3)
		{
			outFile << "f " << meshData->vertexIndicies[i] + 1 << "/" << meshData->texCoordIndicies[i] + 1 << "/" << meshData->normalIndicies[i] + 1 << " "
				<< meshData->vertexIndicies[i + 1] + 1 << "/" << meshData->texCoordIndicies[i + 1] + 1 << "/" << meshData->normalIndicies[i + 1] + 1 << " "
				<< meshData->vertexIndicies[i + 2] + 1 << "/" << meshData->texCoordIndicies[i + 2] + 1 << "/" << meshData->normalIndicies[i + 2] + 1 << std::endl;
				//<< meshData->vertexIndicies[i + 3] + 1 << "/" << meshData->texCoordIndicies[i + 3] + 1 << "/" << meshData->normalIndicies[i + 3] + 1 << std::endl;
		}

		std::cout << "Exported Mesh : " << meshData->name << std::endl;
	}
}