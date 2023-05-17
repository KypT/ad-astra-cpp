#include "Universe.h"
#include <geometry.h>
#include <Physics.h>

Universe::Universe()
{
	universeEnd = float2(-FIELD_WIDTH / 2, FIELD_HEIGHT / 2);
	universeDim = float2(FIELD_WIDTH, FIELD_HEIGHT);
}

void Universe::Load(ID3D11Device* device)
{
	shaderCollection = ShaderCollection(device);

	background.Load(device, shaderCollection);
	asteroidGraphics.Load(device, shaderCollection);
	spaceshipGraphics.Load(device, shaderCollection);

	GenerateAsteroid();
	GenerateAsteroid();
	GenerateAsteroid();
	GenerateAsteroid();
	GenerateAsteroid();
}

void Universe::AddPlayer(Player* player)
{
	players.push_back(player);
	Physics::UpdatePosition(player);
}

void Universe::GenerateAsteroid()
{
	int counter = 5;
	float radius;
	float2 position;
	Asteroid asteroid;
	
	while (counter-- > 0)
	{
		radius = random() * (MAX_RADUIS - MIN_RADIUS) + MIN_RADIUS;
		position = float2(radius + universeEnd.x + random() * (universeDim.x - 2 * radius), universeEnd.y - radius - random() * (universeDim.y - 2 * radius));
		asteroid = Asteroid(position, radius);
		if (Physics::GetCollideNeighbor(&asteroid).empty()) break;
	};
	
	if (counter == 0) return;

	float2 velocity = float2(random()* 5e-4, random() * 5e-4);
	asteroid.m = radius * radius * XM_PI;
	asteroid.vel = velocity;
	asteroids.push_back(asteroid);
}

void Universe::Update()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i].vel *= entropy;
		asteroids[i].pos += asteroids[i].vel;

		if (asteroids[i].pos.x - asteroids[i].r < universeEnd.x)
		{
			asteroids[i].pos = float2(universeEnd.x + asteroids[i].getRadius() + EPS, asteroids[i].getPosition().y);
			asteroids[i].vel.x *= -energy_loss;
		}

		if (asteroids[i].pos.x + asteroids[i].r > universeEnd.x + universeDim.x)
		{
			asteroids[i].pos = float2(universeEnd.x + universeDim.x - asteroids[i].getRadius() - EPS, asteroids[i].getPosition().y);
			asteroids[i].vel.x *= -energy_loss;
		}

		if (asteroids[i].pos.y + asteroids[i].r > universeEnd.y)
		{
			asteroids[i].pos = float2(asteroids[i].getPosition().x, universeEnd.y - asteroids[i].getRadius() - EPS);
			asteroids[i].vel.y *= -energy_loss;
		}

		if (asteroids[i].pos.y - asteroids[i].r < universeEnd.y - universeDim.y)
		{
			asteroids[i].pos = float2(asteroids[i].getPosition().x, universeEnd.y - universeDim.y + asteroids[i].getRadius() + EPS);
			asteroids[i].vel.y *= -energy_loss;
		}

		Physics::UpdatePosition(&asteroids[i]);
		set<Circle*> neighbors = Physics::GetCollideNeighbor(&asteroids[i]);
		if (!neighbors.empty())
			Physics::Collide(&asteroids[i], *neighbors.begin());
	}

	for (int i = 0; i < players.size(); i++)
	{
		players[i]->vel *= entropy;
		players[i]->pos += players[i]->vel;

		if (players[i]->pos.x - players[i]->r <= universeEnd.x || players[i]->pos.x + players[i]->r >= universeEnd.x + universeDim.x)
		{
			players[i]->pos -= players[i]->vel;
			players[i]->vel.x *= -energy_loss;
		}
		if (players[i]->pos.y + players[i]->r >= universeEnd.y || players[i]->pos.y - players[i]->r <= universeEnd.y - universeDim.y)
		{
			players[i]->pos -= players[i]->vel;
			players[i]->vel.y *= -energy_loss;
		}

		Physics::UpdatePosition(players[i]);
		set<Circle*> neighbors = Physics::GetCollideNeighbor(&asteroids[i]);
		if (!neighbors.empty())
			Physics::Collide(players[i], *neighbors.begin());
	}
}

void Universe::Render(ID3D11DeviceContext* deviceContext)
{
	background.Render(deviceContext);

	AsteroidGraphics::WorldTransform asteroidBuf;
	asteroidGraphics.Apply(deviceContext);
	for (size_t i = 0; i < asteroids.size(); i++)
	{
		matrix world = XMMatrixScaling(asteroids[i].r, asteroids[i].r, asteroids[i].r) * XMMatrixTranslation(asteroids[i].pos.x, asteroids[i].pos.y, 0.0f);
		asteroidBuf.world = XMMatrixTranspose(world);

		asteroidGraphics.ApplyTransform(deviceContext, asteroidBuf);
		asteroidGraphics.Render(deviceContext);
	}

	SpaceshipGraphics::WorldTransform spaceshipBuf;
	spaceshipGraphics.Apply(deviceContext);
	for (size_t i = 0; i < players.size(); i++)
	{
		matrix world = XMMatrixScaling(players[i]->r, players[i]->r, players[i]->r);
		world *= XMMatrixRotationZ(players[i]->angle);
		world *=  XMMatrixTranslation(players[i]->pos.x, players[i]->pos.y, 0.0f);
		spaceshipBuf.world = XMMatrixTranspose(world);
		spaceshipBuf.color = float4(1.0f, 1.0f, 0.0f, 1.0f);

		spaceshipGraphics.ApplyTransform(deviceContext, spaceshipBuf);
		spaceshipGraphics.Render(deviceContext);
	}
}

void Universe::Release()
{
	background.Release();
	asteroidGraphics.Release();
}

Universe::~Universe()
{
}
