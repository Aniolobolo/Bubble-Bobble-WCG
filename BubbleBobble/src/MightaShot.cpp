#include "MightaShot.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>


MightaShot::MightaShot(const Point& p, mShotDirections d) : Entity(p, SHOT_PHYSICAL_SIZE, SHOT_PHYSICAL_SIZE, SHOT_FRAME_SIZE, SHOT_FRAME_SIZE)
{
	direction = d;
	speed = .3;

	Rectangle rc;
	Initialise();
	SetAnimation((int)mShotAnim::SHOT_L);

}
void MightaShot::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
MightaShot::~MightaShot()
{
}


AppStatus MightaShot::Initialise()
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

	sprite->SetNumberAnimations((int)mShotAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)mShotAnim::SHOT_L, ANIM_DELAY + 3);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)mShotAnim::SHOT_L, { (float)i * n, 0, -n, n });

	sprite->SetAnimationDelay((int)mShotAnim::SHOT_R, ANIM_DELAY + 3);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)mShotAnim::SHOT_R, { (float)i * n, 0, n, n });

	sprite->SetAnimation((int)mShotAnim::SHOT_L);
	return AppStatus::OK;

}

bool MightaShot::isAlive()
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

void MightaShot::Update()
{
	pos += dir;
	Move(direction);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

}
void MightaShot::Move(mShotDirections d)
{
	AABB box;
	if (d == mShotDirections::LEFT)
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
	else if (d == mShotDirections::RIGHT)
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
void MightaShot::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void MightaShot::Release()
{

	render->Release();
}