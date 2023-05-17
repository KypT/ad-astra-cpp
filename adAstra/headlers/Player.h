#include <main.h>
#include <Abstractions.h>

#pragma once

class Player : public Circle
{
public:
	float2 pos, vel;
	float angle, r, m, engineForce;

	Player();

	void Push();
	void RotateCW();
	void RotateCCW();
	void Move();

	float2 getPosition();
	float2 getVelocity();
	float getRadius();
	float getMass();

	void setPosition(float2 position);
	void setVelocity(float2 velocity);
	void setRadius(float radius);
	void setMass(float mass);

	~Player();
}; 

