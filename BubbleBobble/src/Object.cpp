#include "Object.h"
#include "StaticImage.h"

Sound soundObjects[30];

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = ITEM_BUBBLE_SIZE;
	switch (type)
	{
		/*case ObjectType::ITEM_PINKCANDY: rc = { 0,2 * n, n, n };
		case ObjectType::ITEM_BLUECANDY: rc = { n * 1,2 * n, n, n };
		case ObjectType::ITEM_YELLOWCANDY: rc = { n * 2,2 * n , n, n };
		case ObjectType::ITEM_JORDANS: rc = { n * 3,2 * n, n, n };
		case ObjectType::ITEM_CLOCK: rc = { n * 4,2 * n, n, n };
		case ObjectType::ITEM_BOMB: rc = { n * 5,2 * n, n, n };
		case ObjectType::ITEM_ORANGEUMBRELLA: rc = { n * 6,2 * n, n, n };
		case ObjectType::ITEM_REDUMBRELLA: rc = { n * 7,2 * n, n, n };
		case ObjectType::ITEM_PINKUMBRELLA: rc = { n * 8,2 * n, n, n };*/
		case ObjectType::ITEM_CHERRY: rc = { n * 9, 2 * n, n, n };
		case ObjectType::ITEM_BEER: rc = { n * 10, 2 * n,n, n };
		case ObjectType::ITEM_BANANA: rc = { n * 11, 2 * n,n, n };
		case ObjectType::ITEM_BORGER: rc = { n * 12, 2 * n,n, n };
		case ObjectType::ITEM_WATERMELON: rc = { n * 13, 2 * n,n, n };
		/*case ObjectType::ITEM_BLUERING: rc = { n * 14,2 * n,n, n };
		case ObjectType::ITEM_PINKRING: rc = { n * 15,2 * n,n, n };
		case ObjectType::ITEM_REDRING: rc = { n * 16,2 * n,n, n };
		case ObjectType::ITEM_CROSS: rc = { n * 17,2 * n,n, n };
		case ObjectType::ITEM_LIGHTNINGPOWER: rc = { n * 18,2 * n,n, n };
		case ObjectType::ITEM_E: rc = { n * 19,2 * n,n, n };
		case ObjectType::ITEM_X: rc = { n * 20,2 * n,n, n };
		case ObjectType::ITEM_T: rc = { n * 21,2 * n,n, n };
		case ObjectType::ITEM_E2: rc = { n * 22,2 * n,n, n };
		case ObjectType::ITEM_N: rc = { n * 23,2 * n,n, n };
		case ObjectType::ITEM_D: rc = { n * 24,2 * n,n, n };
		case ObjectType::ITEM_THUNDERBOBBLE: rc = { n * 25, 2 * n,n, n };
		case ObjectType::ITEM_WATERBOBBLE: rc = { n * 26, 2 * n,n, n };
		case ObjectType::ITEM_FIREBOBBLE: rc = { n * 27, 2 * n,n, n };
		case ObjectType::ITEM_REDXBOBBLE: rc = { n * 28, 2 * n,n, n };*/

		default: LOG("Internal error: object creation of invalid type");
	}
	soundObjects[0] = LoadSound("sound/SoundEffects/Items/GrabFruitFX.wav");
	ResourceManager& data = ResourceManager::Instance();

	render = new StaticImage(data.GetTexture(Resource::IMG_ITEMS), rc);


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
	if (type == ObjectType::ITEM_CHERRY) {
		PlaySound(soundObjects[0]);
		return POINTS_200;
	}
	else if (type == ObjectType::ITEM_BEER) {
		PlaySound(soundObjects[0]);
		return POINTS_1000;
	}
	else if (type == ObjectType::ITEM_BANANA) {
		PlaySound(soundObjects[0]);
		return POINTS_350;
	}
	else if (type == ObjectType::ITEM_BORGER) {
		PlaySound(soundObjects[0]);
		return POINTS_550;
	}
	else if (type == ObjectType::ITEM_WATERMELON) {
		PlaySound(soundObjects[0]);
		return POINTS_300;
	}
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
