#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
		case ObjectType::APPLE: rc = {4*n, 3*n, n, n}; break;
		case ObjectType::CHILI: rc = {5*n, 3*n, n, n}; break;
		case ObjectType::WOOPER: rc = { 4 * n, 4 * n, n, n }; break;
		case ObjectType::GULPIN: rc = { 5 * n, 4 * n, n, n }; break;
		case ObjectType::BAGON: rc = { 6 * n, 4 * n, n, n }; break;

		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::APPLE)		return POINTS_APPLE;
	else if (type == ObjectType::CHILI)	return POINTS_CHILI;
	else if (type == ObjectType::WOOPER)	return POINTS_POK1;
	else if (type == ObjectType::GULPIN)	return POINTS_POK2;
	else if (type == ObjectType::BAGON)	return POINTS_POK3;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}