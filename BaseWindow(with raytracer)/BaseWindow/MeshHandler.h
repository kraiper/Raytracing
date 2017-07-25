#pragma once
#include "Mesh.h"
#include <string>
#include "stdafx.h"
#include "tinyobjloader\tiny_obj_loader.h"
#include "ComputeHelp.h"

extern ID3D11DeviceContext* g_DeviceContext;

struct MeshData
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	Mesh mesh;
};

class MeshHandler
{
	public:
		static MeshHandler& getInstance()
		{
			static MeshHandler meshHandler;

			return meshHandler;
		}

		void LoadMesh(std::string _meshPath);
	private:
		MeshHandler() {}

		void FillMesh(std::vector<tinyobj::shape_t>* _shapes, std::vector<tinyobj::material_t>* _materials, Mesh* _mesh);

		std::vector<MeshData> meshList;

		ComputeWrap *computeWrap;
};