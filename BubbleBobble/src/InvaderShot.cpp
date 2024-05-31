#include "InvaderShot.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>


InvaderShot::InvaderShot(const Point& p) : Entity(p, SHOT_PHYSICAL_SIZE, SHOT_PHYSICAL_SIZE, SHOT_FRAME_SIZE, SHOT_FRAME_SIZE)
{
	Rectangle rc;
	Initialise();
	SetAnimation((int)iShotAnim::SHOT_L);

}
void InvaderShot::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
InvaderShot::~InvaderShot()
{
}


AppStatus InvaderShot::Initialise()
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

	sprite->SetNumberAnimations((int)iShotAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)iShotAnim::SHOT_L, ANIM_DELAY + 3);
	for (int i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)iShotAnim::SHOT_L, { (float)i * n, 2 * n, -n, n });

	sprite->SetAnimationDelay((int)iShotAnim::SHOT_R, ANIM_DELAY + 3);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)iShotAnim::SHOT_R, { (float)i * n, n, n, n });

	sprite->SetAnimation((int)iShotAnim::SHOT_L);
	return AppStatus::OK;

}

bool InvaderShot::isAlive()
{
	lifetime += GetFrameTime();
	if (lifetime >= 1.5f)
	{
		return false;
	}
	else {
		return true;
	}
}

void InvaderShot::Update()
{
	pos += dir;
	dir = { 0, 2 };
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void InvaderShot::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void InvaderShot::Release()
{

	render->Release();
}