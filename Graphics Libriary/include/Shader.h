#include <d3dx11.h>
#include <string>
#include <vector>

#pragma once

using namespace std;

class Shader
{
	public:
	ID3D11VertexShader*               vertexShader;
	ID3D11PixelShader*                pixelShader;
	ID3D11InputLayout*				  inputLayout;
	wstring							  filePath;
	wstring							  name;
	
public:
	Shader(wstring fileName);
	Shader();
	~Shader();

	HRESULT Compile(ID3D11Device* device, vector<D3D11_INPUT_ELEMENT_DESC> inputLayout);
	void Apply(ID3D11DeviceContext* deviceContext);
	wstring GetName();
	void Release();
};