#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

#define POINTS_APPLE	10
#define POINTS_CHILI	20
#define POINTS_POK1	15
#define POINTS_POK2	30
#define POINTS_POK3	40

enum class ObjectType { PEPPER, CHILI, WOOPER, GULPIN, BAGON };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;

private:
	ObjectType type;
};

