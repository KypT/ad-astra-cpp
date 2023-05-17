#include <d3d11.h>
#include <main.h>
#include <xnamath.h>
#include <ShaderCollection.h>

#pragma once

class Renderable
{
public:
	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Load(ID3D11Device* device, ShaderCollection coll) = 0;
	virtual void Release() = 0;
	 
	Renderable() {};
	virtual ~Renderable() {};
};

struct Circle
{
public :
	virtual float2 getPosition() = 0;
	virtual float2 getVelocity() = 0;
	virtual float  getRadius() = 0;
	virtual float getMass() = 0;

	virtual void setPosition(float2 position) = 0;
	virtual void setVelocity(float2 velocity) = 0;
	virtual void  setRadius(float radius) = 0;
	virtual void setMass(float mass) = 0;
};