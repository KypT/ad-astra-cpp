#include <Abstractions.h>

#pragma once

class Background : public Renderable
{
private:
	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* dataBuffer;
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* linearSampler;
	int indices_count;

public:

	struct ConstantBuffer
	{
		XMMATRIX world;
	};

	struct Vertex
	{
		XMFLOAT4 pos;
		XMFLOAT2 tex;
	};

	virtual void Render(ID3D11DeviceContext* deviceContext);
	virtual void Load(ID3D11Device* device, ShaderCollection coll);
	virtual void Release();

	Background();
	~Background();
};

