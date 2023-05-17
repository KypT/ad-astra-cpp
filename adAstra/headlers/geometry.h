#pragma once
#include <main.h>

static float2 operator+(float2 &v)
{
	return v;
}
static float2 operator-(float2 &v)
{
	return float2(-v.x, -v.y);
}
static float2 operator+(float2 &v, float2 &u)
{
	return float2(v.x + u.x, v.y + u.y);
}
static float2 operator+=(float2 &v, float2 &u)
{
	return v = v + u;
}
static float2 operator-(float2 &v, float2 &u)
{
	return float2(v.x - u.x, v.y - u.y);
}
static float2 operator-=(float2 &v, float2 &u)
{
	return v = v - u;
}
static float2 operator*(float2 &u, float k)
{
	return float2(u.x * k, u.y * k);
}
static float2 operator*=(float2 &u, float k)
{
	return u = u * k;
}
static float2 operator/(float2 &u, float k)
{
	return float2(u.x / k, u.y / k);
}
static float2 operator/=(float2 &u, float k)
{
	return u = u / k;
}
static float dot(float2 &v, float2 &u)
{
	return v.x * u.x + v.y * u.y;
}
static float2 abs(float2 &v)
{
	return float2(abs(v.x), abs(v.y));
}
static float length(float2 &v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}
static float dist(float2 &v, float2 &u)
{
	return length(v - u);
}

static int   random(int n)
{
	return rand() & n;
}
static float random()
{
	return rand() * 1.0f / RAND_MAX;
}

