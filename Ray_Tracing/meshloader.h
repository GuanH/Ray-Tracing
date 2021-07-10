#pragma once
#include"structure.h"
#include"object.h"
#include<vector>
#include<sstream>

class object;
class mesh;
struct Material;
class Mesh
{
public:
	Mesh(const std::string& path);
	const std::vector<rt::float3>* GetVertices()const;
	const std::vector<int>* GetIndices()const;
	rt::float3 Sampler(rt::float2 uv, int type);
private:
	void LoadMaterial(std::string Path);
	std::vector<rt::float3> vertices;
	std::vector<rt::float2> vertices_texture;
	std::vector<int> indices;
	std::vector<int> indices_texture;
	std::vector<object*> owner;
	rt::BMP texture;
	rt::BMP normal;
	Material* material;
	std::string path;
	float radius2 = 0;
	friend class mesh;
};