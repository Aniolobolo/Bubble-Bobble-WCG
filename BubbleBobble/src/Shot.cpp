#include "Shot.h"
#include "StaticImage.h"

Shot::Shot()
{

	Rectangle rc = { 2 * TILE_SIZE, 12, SHOT_FRAME_WIDTH, SHOT_FRAME_HEIGHT };
	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_SHOTINVADER), rc);
	SetAlive(false);

}
Shot::~Shot()
{
}
void Shot::Init(const Point& p, const Point& dir)
{
	Set(p, dir, SHOT_PHYSICAL_WIDTH, SHOT_PHYSICAL_HEIGHT, SHOT_FRAME_WIDTH, SHOT_FRAME_HEIGHT);
	LOG("Shot initialized at position: ", pos.x, " ", pos.y, " with direction: ", dir.x, " ", dir.y);
}
bool Shot::IsMovingLeft() const
{
	return dir.x < 0;
}
bool Shot::IsMovingRight() const
{
	return dir.x > 0;
}
void Shot::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
