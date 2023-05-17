#define RETURN_IF_FAILED(action) HRESULT hr = action; if (FAILED(hr)) return hr

#pragma once

HRESULT CompileShaderFromFile(WCHAR* filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blob);