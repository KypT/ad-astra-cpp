#include <main.h>
#include <ShaderCollection.h>
#include <AsteroidGraphics.h>
#include <SpaceshipGraphics.h>
#include <Background.h>
#include <Player.h>

using namespace std;

#pragma once
class Universe
{
public:

	struct Asteroid : public Circle
	{
	public:
		float2 pos, vel;
		float r, m;

		Asteroid(float2 position, float2 velocity, float radius, float mass)
		{
			pos = position;
			vel = velocity;
			r = radius;
			m = mass;
		}
		Asteroid(float2 position, float radius)
		{
			pos = position;
			r = radius;
		}
		Asteroid() {}

		virtual float2 Circle::getPosition()
		{
			return pos;
		}
		virtual float2 Circle::getVelocity()
		{
			return vel;
		}
		virtual float Circle::getRadius()
		{
			return r;
		}
		virtual float Circle::getMass()
		{
			return m;
		}
		virtual void Circle::setPosition(float2 position)
		{
			pos = position;
		}
		virtual void Circle::setVelocity(float2 velocity)
		{
			vel = velocity;
		}
		virtual void Circle::setRadius(float radius)
		{
			r = radius;
		}
		virtual void Circle::setMass(float mass)
		{
			m = mass;
		}
	};

private:

	const float entropy = 0.99999f;
	const float energy_loss = 0.8f;

	XMFLOAT2 universeEnd;
	XMFLOAT2 universeDim;
	vector<Asteroid> asteroids;
	vector <Player*> players;

	ShaderCollection shaderCollection;
	Background background;
	AsteroidGraphics asteroidGraphics;
	SpaceshipGraphics spaceshipGraphics;

public:

	Universe();

	void Load(ID3D11Device* device);
	void AddPlayer(Player* player);
	void GenerateAsteroid();
	void Update();
	void Render(ID3D11DeviceContext* deviceContext);
	void Release();

	~Universe();
};

