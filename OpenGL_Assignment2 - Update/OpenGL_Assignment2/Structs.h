#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

class Texture2D;

struct Vector2
{
    float x, y;

    Vector2()
    {
        x = 0.f;
        y = 0.f;
    }

    Vector2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2 operator- (const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator+ (const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
};

struct Rotation
{
    float roll, pitch, yaw;

	Rotation()
	{
        roll = 0.f;
        pitch = 0.f;
        yaw = 0.f;
	}

    Rotation(float roll, float pitch, float yaw)
    {
        this->roll = roll;
        this->pitch = pitch;
        this->yaw = yaw;
    }

    Rotation operator- (const Rotation& other) const { return Rotation(roll - other.roll, pitch - other.pitch, yaw - other.yaw); }
    Rotation operator+ (const Rotation& other) const { return Rotation(roll + other.roll, pitch + other.pitch, yaw + other.yaw); }
};

struct Vector3
{
    float x, y, z;

    Vector3()
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }

    Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3 Normalize()
    {
        float length = sqrt((x * x) + (y * y) + (z * z));
        return Vector3(x / length, y / length, z / length);
    }

	Vector3 Cross(Vector3 a)
    {
        return Vector3(a.y * z - a.z * y, a.x * z - a.z * x, a.x * y - a.y * x);
    }

    static float Distance(Vector3 a, Vector3 b)
    {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
    }
	
    double Dot(const Vector3& other) const
    {
	    return x * other.x + y * other.y + z * other.z;
    }

    Vector3 operator- (const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator+ (const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator* (const float& val) const { return Vector3(x * val, y * val, z * val); }
	Vector3 operator/ (const float& val) const { return Vector3(x / val, y / val, z / val); }
    Vector3& operator-= (const Vector3& other) { x -= other.x; y -= other.y;  z -= other.z;  return *this; }
    Vector3& operator+= (const Vector3& other) { x += other.x; y += other.y;  z += other.z;  return *this; }
};

struct Vector4
{
    float x, y, z, w;

    Vector4()
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
        w = 0.f;
    }

    Vector4(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

struct Lighting
{
    Vector4 ambient, diffuse, specular;

    Lighting()
    {
        ambient = Vector4();
        diffuse = Vector4();
        specular = Vector4();
    }

    Lighting(Vector4 ambient, Vector4 diffuse, Vector4 specular)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
    }
};

struct Material
{
    Vector4 ambient, diffuse, specular;
    GLfloat shininess;

    Material()
    {
        ambient = Vector4();
        diffuse = Vector4();
        specular = Vector4();
        shininess = 0.f;
    }

    Material(Vector4 ambient, Vector4 diffuse, Vector4 specular, GLfloat shininess)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }
};

struct Color
{
    int r, g, b;

    Color()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    Color(int r, int g, int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

	int GetColorID()
    {
        return r + (g * 256) + (b * 256 * 256);
    }
};

struct TexCoord
{
    GLfloat u, v;
	
    TexCoord()
    {
        u = 0.f;
        v = 0.f;
    }
	
	TexCoord(GLfloat u, GLfloat v)
	{
        this->u = u;
        this->v = v;
	}
};

struct MeshData
{
    int indexCount, normalCount, texCoordCount, vertexCount;

    std::string name;
	
    Vector3* vertices;
    Vector3* normals;
    TexCoord* texCoords;

    GLushort* vertexIndicies;
    GLushort* normalIndicies;
    GLushort* texCoordIndicies;

    MeshData() = default;

    MeshData(std::string name, int vertexCount, int normalCount, int texCoordCount, int indexCount)
    {
        this->name = name;
        this->normalCount = normalCount;
        this->texCoordCount = texCoordCount;
        this->indexCount = indexCount;
        this->vertexCount = vertexCount;

        vertices = new Vector3[vertexCount];
        normals = new Vector3[normalCount];
        texCoords = new TexCoord[texCoordCount];

        vertexIndicies = new GLushort[indexCount];
        normalIndicies = new GLushort[indexCount];
        texCoordIndicies = new GLushort[indexCount];
    }
	
    ~MeshData()
    {
        delete vertices;
        delete normals;
        delete texCoords;

        delete normalIndicies;
        delete texCoordIndicies;
    }
};