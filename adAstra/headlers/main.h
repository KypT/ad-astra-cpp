#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <vector>
#include <string>

#pragma once

using namespace std;

typedef XMFLOAT2 float2;
typedef XMFLOAT3 float3;
typedef XMFLOAT4 float4;

typedef XMMATRIX matrix;

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH  1280
#define FIELD_WIDTH 6.4f
#define FIELD_HEIGHT 3.6f

#define MIN_RADIUS 0.01f
#define MAX_RADUIS 0.25f
#define MAX_OBJECT_SIZE 0.6f
#define EPS 1e-6f