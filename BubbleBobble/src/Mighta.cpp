#include "Mighta.h"
#include "Sprite.h"

Mighta::Mighta(const Point& p, int width, int height, int frame_width, int frame_height, TileMap* mapp) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = MightaState::ROAMING;
	map = mapp;
	current_step = 0;
	current_frames = 0;
}
Mighta::~Mighta()
{
}
AppStatus Mighta::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = MIGHTA_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for slime sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)MightaAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)MightaAnim::IDLE_RIGHT, MIGHTA_ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)MightaAnim::IDLE_RIGHT, { float(i) * n, 3 * n, n, n });

	sprite->SetAnimationDelay((int)MightaAnim::IDLE_LEFT, MIGHTA_ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)MightaAnim::IDLE_LEFT, { float(i) * n, 3 * n, -n, n });

	sprite->SetAnimationDelay((int)MightaAnim::WALKING_RIGHT, MIGHTA_ANIM_DELAY);
	for (i = 0; i < 5; ++i)
		sprite->AddKeyFrame((int)MightaAnim::WALKING_RIGHT, { (float)i * n, 3 * n, n, n });
	sprite->SetAnimationDelay((int)MightaAnim::WALKING_LEFT, MIGHTA_ANIM_DELAY);
	for (i = 0; i < 5; ++i)
		sprite->AddKeyFrame((int)MightaAnim::WALKING_LEFT, { (float)i * n, 3 * n, -n, n });

	sprite->SetAnimationDelay((int)MightaAnim::ATTACK_RIGHT, MIGHTA_ANIM_DELAY);
	for (i = 5; i < 10; ++i)
		sprite->AddKeyFrame((int)MightaAnim::ATTACK_RIGHT, { (float)i * n, 3 * n, n, n });
	sprite->SetAnimationDelay((int)MightaAnim::ATTACK_LEFT, MIGHTA_ANIM_DELAY);
	for (i = 5; i < 10; ++i)
		sprite->AddKeyFrame((int)MightaAnim::ATTACK_LEFT, { (float)i * n, 3 * n, -n, n });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)MightaAnim::IDLE_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)MightaAnim::IDLE_RIGHT);

	visibility_area = area;

	return AppStatus::OK;

}
//void Mighta::BubbleSetter(PlayerBubble* bub)
//{
//	for (PlayerBubbl* buble : bubbles) {
//		bubbles.push_back(bub);
//	}
//}
bool Mighta::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	MoveX();
	MoveY();
	StartFalling();
	Warp();
	sprite->Update();

	return shoot;
}

void Mighta::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
	if (look == Look::RIGHT && state != MightaState::FALLING && map->TestCollisionGround(box, &pos.y))
	{
		pos.x += MIGHTA_SPEED;
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			look = Look::LEFT;
			SetAnimation((int)MightaAnim::WALKING_LEFT);
		}
		else if (map->TestCollisionWallLeft(box)) {
			pos.x = prev_x;
			look = Look::LEFT;
			SetAnimation((int)MightaAnim::WALKING_RIGHT);


		}

	}
	else if (look == Look::LEFT && state != MightaState::FALLING && map->TestCollisionGround(box, &pos.y))
	{
		pos.x += -MIGHTA_SPEED;
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			look = Look::RIGHT;
			SetAnimation((int)MightaAnim::WALKING_RIGHT);
		}
		else if (map->TestCollisionWallRight(box)) {
			pos.x = prev_x;
			look = Look::RIGHT;
			SetAnimation((int)MightaAnim::WALKING_RIGHT);

		}
	}
}
EnemyType Mighta::GetEnemyType() const
{
	return EnemyType::MIGHTA;
}
void Mighta::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Mighta::StartFalling()
{
	dir.y = 1;
}
void Mighta::Stop()
{
	dir = { 0,0 };
	state = MightaState::ROAMING;
}
void Mighta::MoveY()
{
	AABB box, prev_box;
	int prev_x = pos.x;
	int prev_y = pos.y;


	if (state != MightaState::JUMPING)
	{
		pos.y += 1;

		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == MightaState::FALLING) Stop();
			if (IsKeyPressed(KEY_X))
				dir.y = -1;
		}
		else
		{
			if (state != MightaState::FALLING) StartFalling();
		}


	}


}

void Mighta::UpdateLook(int anim_id)
{
	MightaAnim anim = (MightaAnim)anim_id;
	look = (anim == MightaAnim::WALKING_LEFT || anim == MightaAnim::WALKING_LEFT || anim == MightaAnim::WALKING_LEFT) ? Look::LEFT : Look::RIGHT;
}
void Mighta::GetShootingPosDir(Point* p, Point* d) const
{
	if (look == Look::LEFT)
	{
		p->x = pos.x + MIGHTA_SHOT_OFFSET_X_LEFT;
		*d = { -MIGHTA_SHOT_SPEED, 0 };
	}
	else
	{
		p->x = pos.x + MIGHTA_SHOT_OFFSET_X_RIGHT;
		*d = { MIGHTA_SHOT_SPEED, 0 };
	}
	p->y = pos.y + MIGHTA_SHOT_OFFSET_Y;
}