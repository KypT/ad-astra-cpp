#include <Windows.h>
#include <string>
#include <vector>
#include <xnamath.h>

#pragma once

using namespace std;
namespace demo{
	struct Vertex
	{
		XMFLOAT4 pos;
		XMFLOAT4 normal;
		XMFLOAT2 tex;
	};
}


	struct Mesh
	{
	public:
		XMFLOAT4* coords;
		XMFLOAT4* normals;
		XMFLOAT2* uvs;
		WORD* inds;

		UINT verts_count;
		UINT inds_count;

		void Release()
		{
			delete [] coords;
			delete [] normals;
			delete [] uvs;
			delete [] inds;
		}
	};

	Mesh loadMeshFromFile_ply(string fileName);

void createSphere(demo::Vertex* &vertices, WORD* &indices, int slices, int stacks);
