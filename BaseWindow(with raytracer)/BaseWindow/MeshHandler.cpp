#include "MeshHandler.h"


void MeshHandler::LoadMesh(std::string _meshPath)
{

	MeshData meshData;


	std::string err = tinyobj::LoadObj(meshData.shapes, meshData.materials, _meshPath.c_str());
	g_DeviceContext->Flush();

	if (!err.empty())
	{
		MessageBox(NULL, "Failed reading the OBJ-file", _meshPath.c_str(), MB_OK);
	}

	FillMesh(&meshData.shapes, &meshData.materials, &meshData.mesh);
	g_DeviceContext->Flush();



	//////////////////OLD////////////////
	//Load OBJ-file
	//m_mesh.loadObj("Meshi/kub.obj");
	meshData.mesh.setColor(XMFLOAT4(1, 1, 1, 1));
	//m_mesh.scaleMesh(XMFLOAT3(0.10, 0.10, 0.10));
	//m_mesh.scaleMesh(XMFLOAT3(2, 2, 2));
	//m_mesh.scaleMesh(XMFLOAT3(10, 10, 10));
	//m_mesh.rotateMesh(XMFLOAT3(0, PI, 0));
	//m_mesh.rotateMesh(XMFLOAT3(0, PI, 0));

	meshList.push_back(meshData);

	createKdTree(&meshData.mesh);
	g_DeviceContext->Flush();

	m_meshBuffer = computeWrap->CreateBuffer(STRUCTURED_BUFFER,
		sizeof(TriangleMat),
		meshData.mesh.getNrOfFaces(),
		true,
		false,
		false,
		meshData.mesh.getTriangles(),
		false,
		"Structured Buffer: Mesh Texture");

	//from wchat_t to string
	//std::string narrow = converter.to_bytes(wide_utf16_source_string);
	//from string to wchar_t
	std::wstring meshTextureWstring = converter.from_bytes(m_mesh.getTextureString());


	//TEXTURE STUFF
	CreateWICTextureFromFile(g_Device,
		g_DeviceContext,
		meshTextureWstring.c_str(),
		NULL,
		&m_meshTexture);


}

void MeshHandler::FillMesh(std::vector<tinyobj::shape_t>* _shapes, std::vector<tinyobj::material_t>* _materials, Mesh* _mesh)
{
	std::vector<TriangleMat> temp;

	for (int i = 0; i < _shapes->size(); i++)
	{
		std::vector<XMFLOAT4> pos;
		std::vector<XMFLOAT4> normal;
		for (int j = 0; j < _shapes->at(i).mesh.indices.size(); j++)
		{
			XMFLOAT4 tempPos;
			tempPos.x = _shapes->at(i).mesh.positions.at(_shapes->at(i).mesh.indices.at(j) * 3);
			tempPos.y = _shapes->at(i).mesh.positions.at(_shapes->at(i).mesh.indices.at(j) * 3 + 1);
			tempPos.z = _shapes->at(i).mesh.positions.at(_shapes->at(i).mesh.indices.at(j) * 3 + 2);
			tempPos.w = 0;

			pos.push_back(tempPos);

			if (_shapes->at(i).mesh.normals.size() > 0)
			{
				XMFLOAT4 tempNormal;
				//Normals
				tempNormal.x = _shapes->at(i).mesh.normals.at(_shapes->at(i).mesh.indices.at(j) * 3);
				tempNormal.y = _shapes->at(i).mesh.normals.at(_shapes->at(i).mesh.indices.at(j) * 3 + 1);
				tempNormal.z = _shapes->at(i).mesh.normals.at(_shapes->at(i).mesh.indices.at(j) * 3 + 2);
				tempNormal.w = 0;

				normal.push_back(tempNormal);
			}

		}

		for (int j = 0; j < pos.size(); j += 3)
		{
			TriangleMat tempPush;
			tempPush.pos0 = pos.at(j);
			tempPush.pos1 = pos.at(j + 1);
			tempPush.pos2 = pos.at(j + 2);

			tempPush.ID = temp.size();
			tempPush.pad = 0;

			if (_shapes->at(i).mesh.normals.size() > 0)
			{
				tempPush.normal = normal.at(j);
			}

			temp.push_back(tempPush);
		}


		//int k = 0;
		//for (int j = 0; j < _shapes->at(i).mesh.texcoords.size(); j += 6)
		//{
		//	//Textcoordinats
		//	temp.at(k).textureCoordinate0.x = _shapes->at(i).mesh.texcoords.at(j);
		//	temp.at(k).textureCoordinate0.y = _shapes->at(i).mesh.texcoords.at(j + 1);

		//	temp.at(k).textureCoordinate1.x = _shapes->at(i).mesh.texcoords.at(j + 2);
		//	temp.at(k).textureCoordinate1.y = _shapes->at(i).mesh.texcoords.at(j + 3);

		//	temp.at(k).textureCoordinate2.x = _shapes->at(i).mesh.texcoords.at(j + 4);
		//	temp.at(k).textureCoordinate2.y = _shapes->at(i).mesh.texcoords.at(j + 5);
		//	k++;
		//}


	}

	for (int i = 0; i < temp.size(); i++)
	{
		_mesh->m_meshTriangles.push_back(temp.at(i));
	}

	_mesh->m_nrOfFaces = _mesh->m_meshTriangles.size();

}