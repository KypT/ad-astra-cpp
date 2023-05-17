#include <d3d11.h>
#include <string>
#include <map>

#include <Shader.h>

using namespace std;

#pragma once

class ShaderCollection
{
private:
	static map<wstring, Shader*>  shaders;

public:
	Shader* GetShader(wstring shaderName);  
	static void release();
	ShaderCollection();
	ShaderCollection(ID3D11Device* device);
	~ShaderCollection();
};

