#include <d3dcompiler.h>
#include <d3dx11.h>
#include <Misc.h>

HRESULT CompileShaderFromFile(WCHAR* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blob)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* errorBlob;

	RETURN_IF_FAILED(D3DX11CompileFromFile(filename, NULL, NULL, entryPoint, shaderModel,
		dwShaderFlags, 0, NULL, blob, &errorBlob, NULL));

	return S_OK;
}