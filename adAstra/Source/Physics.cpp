#include "Physics.h"
#include <geometry.h>

Field Physics::field;
map<Circle*, pair<int, int>> Physics::circles;
const float Physics::energy_loss = 0.8f;

pair<int, int> Physics::getFieldIndices(Circle* circle)
{
	return make_pair((int) ((circle->getPosition().x + FIELD_WIDTH / 2) / MAX_OBJECT_SIZE) + 1, (int) ((circle->getPosition().y + FIELD_HEIGHT / 2) / MAX_OBJECT_SIZE) + 1);
}

bool Physics::AreIntersected(Circle* first, Circle* second)
{
	if (first == nullptr) return false;
	if (second == nullptr) return false;
	if (first == second) return false;
	if (dist(first->getPosition(), second->getPosition()) >= first->getRadius() + second->getRadius()) return false;
	return true;
}

void Physics::UpdatePosition(Circle* circle)
{
	pair<int, int> curPos = Physics::getFieldIndices(circle);
	if (circles.count(circle))
	{
		pair<int, int> oldPos = circles[circle];
		field[oldPos.first][oldPos.second].emplace(circle);
		field[curPos.first][curPos.second].insert(circle);
	}
	else
		field[curPos.first][curPos.second].insert(circle);
	circles[circle] = curPos;
}

set<Circle*> Physics::GetCollideNeighbor(Circle* circle)
{
	pair<int, int> pos = getFieldIndices(circle);
	set<Circle*> neighbors;

	for (int i = -1; i <= 1; i++)
	for (int j = -1; j <= 1; j++)
	for each (Circle* another in field[pos.first + i][pos.second + j])
	if (AreIntersected(circle, another))
		neighbors.insert(another);
	return neighbors;
}

void Physics::Collide(Circle* first, Circle* second)
{
  	float twoR = first->getRadius() + second->getRadius();
	float2 penetration = float2(twoR + EPS, twoR + EPS) - abs(first->getPosition() - second->getPosition());
	first->setPosition(first->getPosition() + penetration / 2);
	second->setPosition(second->getPosition() + penetration / 2);


	float2 newVel1 = float2
		(first->getVelocity().x * (first->getMass() - second->getMass()) + 2 * second->getMass() * second->getVelocity().x,
		first->getVelocity().y * (first->getMass() - second->getMass()) + 2 * second->getMass() * second->getVelocity().y);

	float2 newVel2 = float2
		(second->getVelocity().x * (second->getMass() - first->getMass()) + 2 * first->getMass() * first->getVelocity().x,
		second->getVelocity().y * (second->getMass() - first->getMass()) + 2 * first->getMass() * first->getVelocity().y);


	float d = (first->getMass() + second->getMass());
	first->setVelocity(newVel1 / d);
	second->setVelocity(newVel2 / d);


	first->setPosition(first->getPosition() + first->getVelocity());
	second->setPosition(second->getPosition() + second->getVelocity());

	UpdatePosition(first);
	UpdatePosition(second);
}

Physics::Physics()
{
	for (int i = 0; i < field.size(); i++)
		field[i].fill(set<Circle*>());
}

Physics::~Physics()
{
}
