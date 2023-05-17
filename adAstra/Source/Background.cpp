#include "Background.h"
#include <vector>
#include <Mesh.h>

using namespace std;

Background::Background() :indices_count(0) {};

void Background::Load(ID3D11Device* device, ShaderCollection shaderColl)
{
	shader = shaderColl.GetShader(L"texture");
	
	Mesh mesh = loadMeshFromFile_ply("models/background.ply");
	Vertex* vertices = new Vertex[mesh.verts_count];
	
	for (int i = 0; i < mesh.verts_count; i++)
		vertices[i] = {mesh.coords[i], mesh.uvs[i]};

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));

	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * mesh.verts_count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	InitData.pSysMem = vertices;
	device->CreateBuffer(&bd, &InitData, &vertexBuffer);

	bd.ByteWidth = sizeof(WORD) * mesh.inds_count;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	InitData.pSysMem = mesh.inds;
	device->CreateBuffer(&bd, &InitData, &indexBuffer);

	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0; 
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&bd, NULL, &dataBuffer);

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(device, L"textures/space.jpg", NULL, NULL, &resourceView, NULL);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &linearSampler);

	indices_count = mesh.inds_count;
	mesh.Release();
}
 
void Background::Render(ID3D11DeviceContext* deviceContext)
{
	shader->Apply(deviceContext);

	deviceContext->VSSetConstantBuffers(1, 1, &dataBuffer);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.world = XMMatrixTranspose(XMMatrixIdentity());
	deviceContext->UpdateSubresource(dataBuffer, 0, NULL, &cb, 0, 0);
	deviceContext->PSSetShaderResources(0, 1, &resourceView);
	deviceContext->PSSetSamplers(0, 1, &linearSampler);
	deviceContext->DrawIndexed(indices_count, 0, 0);
}

void Background::Release()
{
	shader->Release();
	
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
	if (dataBuffer) dataBuffer->Release();
	if (linearSampler) linearSampler->Release();
	if (resourceView) resourceView->Release();
}

Background::~Background()
{
}
