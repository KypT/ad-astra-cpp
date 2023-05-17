#include <main.h>
#include <Abstractions.h>
#include <array>
#include <map>
#include <set>

using namespace std;

typedef array< array < set <Circle*>, (size_t)(FIELD_HEIGHT / MAX_OBJECT_SIZE) + 3>, (size_t) (FIELD_WIDTH / MAX_OBJECT_SIZE) + 3> Field;

#pragma once
class Physics
{
private:
	static Field field;
	static map<Circle*, pair<int, int>> circles;
	static const float energy_loss;

	static pair<int, int> getFieldIndices(Circle* circle);
	static bool AreIntersected(Circle* first, Circle* second);

public:
	static void UpdatePosition(Circle* circle);
	static set<Circle*> GetCollideNeighbor(Circle* circle);
	static void Collide(Circle* first, Circle* second);

	Physics();
	~Physics();
};