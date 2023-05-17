#include <Shader.h>
#include <Misc.h>
#include <ShaderCollection.h>

Shader::Shader(wstring fileName)
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	inputLayout = nullptr;
	filePath = fileName;
	name = fileName.substr(fileName.find_first_of('/') + 1, fileName.find_last_of('_') - fileName.find_first_of('/') - 1);
}

Shader::Shader()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	inputLayout = nullptr;
}
Shader::~Shader() {};

HRESULT Shader::Compile(ID3D11Device* device, vector<D3D11_INPUT_ELEMENT_DESC> layout)
{
	ID3DBlob* vertexBlob = nullptr;
	ID3DBlob* pixelBlob = nullptr;

	CompileShaderFromFile((WCHAR *)filePath.data(), "VS", "vs_5_0", &vertexBlob);
	CompileShaderFromFile((WCHAR*) filePath.data(), "PS", "ps_5_0", &pixelBlob);
	device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &vertexShader);
	device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, &pixelShader);
	device->CreateInputLayout(layout.data(), layout.size(), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &inputLayout);

	vertexBlob->Release();
	pixelBlob->Release();
	return S_OK;
}

void Shader::Apply(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
}

wstring Shader::GetName()
{
	return name;
}

void Shader::Release()
{
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
	if (inputLayout) inputLayout->Release();
}