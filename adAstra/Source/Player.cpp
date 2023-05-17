#include "Player.h"
#include <geometry.h>
#include <math.h>

Player::Player()
{
	pos = float2(0, 0);
	vel = float2(0, 0);
	angle = 0;// -XM_PIDIV2;
	r = 0.4f;
	m = 1;
	engineForce = 0.0000001f;
}

void Player::Push()
{
	vel += float2(cos(angle), sin(angle)) * engineForce;
}

void Player::RotateCW()
{
	angle += 0.001f;
}

void Player::RotateCCW()
{
	angle -= 0.001f;
}

void Player::Move()
{
	pos += vel;
}

float2 Player::getPosition()
{
	return pos;
}

float2 Player::getVelocity()
{
	return vel;
}

float Player::getRadius()
{
	return r;
}

float Player::getMass()
{
	return m;
}

void Player::setPosition(float2 position)
{
	pos = position;
}

void Player::setVelocity(float2 velocity)
{
	vel = velocity;
}

void Player::setRadius(float radius)
{
	r = radius;
}

void Player::setMass(float mass)
{
	m = mass;
}

Player::~Player()
{
}
