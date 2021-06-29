#pragma once
#include"structure.h"
#include"object.h"
#include<vector>
#include<sstream>
#include<fstream>

class object;
class mesh;

class Mesh
{
public:
	Mesh(std::string Path);
	const std::vector<float3>* GetVertices()const;
	const std::vector<int>* GetIndices()const;
private:
	std::vector<float3> vertices;
	std::vector<int> indices;
	float radius2 = 0;
	friend class mesh;
};