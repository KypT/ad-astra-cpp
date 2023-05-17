#include <main.h>
#include <ShaderCollection.h>

#pragma once
class SpaceshipGraphics
{
public:

	struct WorldTransform
	{
		matrix world;
		float4 color;
	};

	struct Vertex
	{
		float4 pos;
		float2 tex;
	};

private:
	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* dataBuffer;
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* linearSampler;
	int indices_count;
public:
	void Apply(ID3D11DeviceContext* deviceContext);
	void ApplyTransform(ID3D11DeviceContext* deviceContext, const SpaceshipGraphics::WorldTransform &buf);
	virtual void Render(ID3D11DeviceContext* deviceContext);
	virtual void Load(ID3D11Device* device, ShaderCollection coll);
	virtual void Release();
	SpaceshipGraphics();
	~SpaceshipGraphics();
};

