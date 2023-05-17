#include <ShaderCollection.h>
#include <comdef.h>
#include <string>

map <wstring, Shader*> ShaderCollection::shaders;

Shader* ShaderCollection::GetShader(wstring shaderName)
{
	return shaders[shaderName];
}

ShaderCollection::ShaderCollection(){}

ShaderCollection::ShaderCollection(ID3D11Device* device)
{
	vector<D3D11_INPUT_ELEMENT_DESC> input_Pos = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	vector<D3D11_INPUT_ELEMENT_DESC> input_PosTex = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Shader *solid = new Shader(L"shaders/solid_shader.fx");
	Shader *texture = new Shader(L"shaders/texture_shader.fx");

	solid->Compile(device, input_Pos);
	texture->Compile(device, input_PosTex);

	shaders[solid->GetName()] = solid;
	shaders[texture->GetName()] = texture;
}

ShaderCollection::~ShaderCollection()
{

}

void ShaderCollection::release()
{
	for (map<wstring, Shader*>::reverse_iterator it = shaders.rbegin(); it != shaders.rend(); it++)
		it->second->Release();
}
