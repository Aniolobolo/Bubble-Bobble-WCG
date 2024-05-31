#include "DrunkShot.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>


DrunkShot::DrunkShot(const Point& p, ShotDirections d) : Entity(p, SHOT_PHYSICAL_SIZE, SHOT_PHYSICAL_SIZE, SHOT_FRAME_SIZE, SHOT_FRAME_SIZE)
{
	direction = d;
	speed = .3;
	
	Rectangle rc;
	Initialise();
	SetAnimation((int)ShotAnim::SHOT_L);

}
void DrunkShot::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
DrunkShot::~DrunkShot()
{
}


AppStatus DrunkShot::Initialise()
{
	Rectangle rc;
	const int n = ITEM_BUBBLE_SIZE;
	deleteShot = false;
	ResourceManager& data = ResourceManager::Instance();
	data.LoadTexture(Resource::IMG_SHOTS, "images/shots.png");

	render = new Sprite(data.GetTexture(Resource::IMG_SHOTS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for shots sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	sprite->SetNumberAnimations((int)ShotAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ShotAnim::SHOT_L, ANIM_DELAY + 3);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ShotAnim::SHOT_L, { (float)i * n, n, -n, n });

	sprite->SetAnimationDelay((int)ShotAnim::SHOT_R, ANIM_DELAY + 3);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ShotAnim::SHOT_R, { (float)i * n, n, n, n });
	
	sprite->SetAnimation((int)ShotAnim::SHOT_L);
	return AppStatus::OK;

}

bool DrunkShot::isAlive()
{
	lifetime += GetFrameTime();
	if (lifetime >= 2.25f)
	{
		return false;
	}
	else {
		return true;
	}
}

void DrunkShot::Update()
{
	pos += dir;
	Move(direction);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

}
void DrunkShot::Move(ShotDirections d)
{
	AABB box;
	waitForAnim += GetFrameTime();
	if (waitForAnim > 0.6f) {
		if (d == ShotDirections::LEFT)
		{
			dir = { -2, 0 };
			box = GetHitbox();
			if (map->TestCollisionWallLeft(box))
			{
				LOG("Touched left wall");
				deleteShot = true;
			}
			else {
				deleteShot = false;
			}

		}
		else if (d == ShotDirections::RIGHT)
		{
			dir = { 2, 0 };
			box = GetHitbox();
			if (map->TestCollisionWallRight(box))
			{
				LOG("Touched left wall");
				deleteShot = true;
			}
			else {
				deleteShot = false;
			}
		}
	}
}
void DrunkShot::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void DrunkShot::Release()
{

	render->Release();
}