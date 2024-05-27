#include "Mighta.h"
#include "Sprite.h"
#include "EnemyManager.h"

Mighta::Mighta(const Point& p, int width, int height, int frame_width, int frame_height, TileMap* tilemap) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = MightaState::ROAMING;
	map = tilemap;
	current_step = 0;
	current_frames = 0;
	dir = { -1,0 };
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

bool Mighta::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Mighta::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Mighta::IsAscending() const
{
	return dir.y < -MIGHTA_LEVITATING_SPEED;
}
bool Mighta::IsLevitating() const
{
	return abs(dir.y) <= MIGHTA_LEVITATING_SPEED;
}
bool Mighta::IsDescending() const
{
	return dir.y > MIGHTA_LEVITATING_SPEED;
}
bool Mighta::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Mighta::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Mighta::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);

}
MightaAnim Mighta::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (MightaAnim)sprite->GetAnimation();
}
void Mighta::Stop()
{
	dir = { 0,0 };
	state = MightaState::ROAMING;
	if (IsLookingRight())	SetAnimation((int)MightaAnim::IDLE_RIGHT);
	else					SetAnimation((int)MightaAnim::IDLE_LEFT);
}
void Mighta::StartWalkingLeft()
{
	pos.x += -MIGHTA_SPEED;
	state = MightaState::ROAMING;
	look = Look::LEFT;
	SetAnimation((int)MightaAnim::WALKING_LEFT);
}
void Mighta::StartWalkingRight()
{
	pos.x += MIGHTA_SPEED;
	state = MightaState::ROAMING;
	look = Look::RIGHT;
	SetAnimation((int)MightaAnim::WALKING_RIGHT);
}
void Mighta::StartFalling()
{
	dir.y = MIGHTA_FALLING_SPEED;
	state = MightaState::FALLING;
	if (IsLookingRight())	SetAnimation((int)MightaAnim::FALLING_RIGHT);
	else					SetAnimation((int)MightaAnim::FALLING_LEFT);
}
//void Enemy::StartJumping()
//{
//	dir.y = -ENEMY_JUMP_FORCE;
//	state = hState::EJUMPING;
//	if (IsLookingRight())	SetAnimation((int)EnemyAnim::JUMPING_RIGHT);
//	else					SetAnimation((int)EnemyAnim::JUMPING_LEFT);
//	jump_delay = ENEMY_JUMP_DELAY;
//	
//}

void Mighta::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case MightaState::IDLE:	 SetAnimation((int)MightaAnim::IDLE_RIGHT);    break;
	case MightaState::ROAMING: SetAnimation((int)MightaAnim::WALKING_RIGHT); break;
	case MightaState::JUMPING: SetAnimation((int)MightaAnim::JUMPING_RIGHT); break;
	case MightaState::FALLING: SetAnimation((int)MightaAnim::FALLING_RIGHT); break;
	}
}
void Mighta::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case MightaState::IDLE:	 SetAnimation((int)MightaAnim::IDLE_LEFT);    break;
	case MightaState::ROAMING: SetAnimation((int)MightaAnim::WALKING_LEFT); break;
	case MightaState::JUMPING: SetAnimation((int)MightaAnim::JUMPING_LEFT); break;
	case MightaState::FALLING: SetAnimation((int)MightaAnim::FALLING_LEFT); break;
	}
}
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
	pos.x += dir.x;
	box = GetHitbox();

	if (hasStartedWalking == false) {
		StartWalkingLeft();
		state = MightaState::ROAMING;
		hasStartedWalking = true;
	}

	if (hasStartedWalking) {
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			StartWalkingRight();
			dir.x = 1;
		}
		else if (map->TestCollisionWallRight(box)) {
			pos.x = prev_x;
			StartWalkingLeft();
			dir.x = -1;
		}
	}
}
EnemyType Mighta::GetEnemyType() const
{
	return EnemyType::MIGHTA;
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
	look = (anim == MightaAnim::WALKING_LEFT) ? Look::LEFT : Look::RIGHT;
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