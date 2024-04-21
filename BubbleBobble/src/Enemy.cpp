
#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Enemy::Enemy(const Point& p, hState s, hLook view, hType t):
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	type = t;
	hasStartedWalking = false;
	state = s;
	look = view;
	map = nullptr;
	score = 0;
	dir = { -1,0 };
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	int i;
	const int n = ENEMY_FRAME_SIZE;


	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZENCHAN, "images/zenchan.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadTexture(Resource::IMG_INVADER, "images/invader.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	switch (type)
	{
	case hType::ZENCHAN:render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN)); break;
	case hType::INVADER:render = new Sprite(data.GetTexture(Resource::IMG_INVADER)); break;

	default: LOG("Internal error: enemy creation of invalid type");
	}
	//???
	//if (type == hType::ZENCHAN) {
	//	if (data.LoadTexture(Resource::IMG_ZENCHAN, "images/zenchan.png") != AppStatus::OK)
	//	{
	//		return AppStatus::ERROR;
	//	}

	//	render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN));
	//	if (render == nullptr)
	//	{
	//		LOG("Failed to allocate memory for player sprite");
	//		return AppStatus::ERROR;
	//	}
	//}
	//else if (type == hType::INVADER) {
	//	if (data.LoadTexture(Resource::IMG_INVADER, "images/invader.png") != AppStatus::OK)
	//	{
	//		return AppStatus::ERROR;
	//	}

	//	render = new Sprite(data.GetTexture(Resource::IMG_INVADER));
	//	if (render == nullptr)
	//	{
	//		LOG("Failed to allocate memory for player sprite");
	//		return AppStatus::ERROR;
	//	
	//}


	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)i * n, 4 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_RIGHT, { 2 * n, 5 * n, n, n });
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_RIGHT, { 3 * n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_LEFT, { 2 * n, 5 * n, -n, n });
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_LEFT, { 3 * n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::JUMPING_RIGHT, { 0, 5 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::JUMPING_LEFT, { 0, 5 * n, -n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::LEVITATING_RIGHT, { n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::LEVITATING_LEFT, { n, 5 * n, -n, n });

	sprite->SetAnimation((int)EnemyAnim::IDLE_RIGHT);

	return AppStatus::OK;
}

void Enemy::InitScore()
{
	score = 0;
}
void Enemy::IncrScore(int n)
{
	score += n;
}
int Enemy::GetScore()
{
	return score;
}
void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Enemy::IsLookingRight() const
{
	return look == hLook::ERIGHT;
}
bool Enemy::IsLookingLeft() const
{
	return look == hLook::ELEFT;
}
bool Enemy::IsAscending() const
{
	return dir.y < -ENEMY_LEVITATING_SPEED;
}
bool Enemy::IsLevitating() const
{
	return abs(dir.y) <= ENEMY_LEVITATING_SPEED;
}
bool Enemy::IsDescending() const
{
	return dir.y > ENEMY_LEVITATING_SPEED;
}
bool Enemy::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Enemy::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
EnemyAnim Enemy::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (EnemyAnim)sprite->GetAnimation();
}
void Enemy::Stop()
{
	dir = { 0,0 };
	state = hState::EIDLE;
	if (IsLookingRight())	SetAnimation((int)EnemyAnim::IDLE_RIGHT);
	else					SetAnimation((int)EnemyAnim::IDLE_LEFT);
}
void Enemy::StartWalkingLeft()
{
	pos.x += -ENEMY_SPEED;
	state = hState::EWALKING;
	look = hLook::ELEFT;
	SetAnimation((int)EnemyAnim::WALKING_LEFT);
}
void Enemy::StartWalkingRight()
{
	pos.x += ENEMY_SPEED;
	state = hState::EWALKING;
	look = hLook::ERIGHT;
	SetAnimation((int)EnemyAnim::WALKING_RIGHT);
}
void Enemy::StartFalling()
{
	dir.y = ENEMY_FALLING_SPEED;
	state = hState::EFALLING;
	if (IsLookingRight())	SetAnimation((int)EnemyAnim::FALLING_RIGHT);
	else					SetAnimation((int)EnemyAnim::FALLING_LEFT);
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
void Enemy::StartClimbingUp()
{
	state = hState::ECLIMBING;
	SetAnimation((int)EnemyAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Enemy::StartClimbingDown()
{
	state = hState::ECLIMBING;
	SetAnimation((int)EnemyAnim::CLIMBING_TOP);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Enemy::ChangeAnimRight()
{
	look = hLook::ERIGHT;
	switch (state)
	{
	case hState::EIDLE:	 SetAnimation((int)EnemyAnim::IDLE_RIGHT);    break;
	case hState::EWALKING: SetAnimation((int)EnemyAnim::WALKING_RIGHT); break;
	case hState::EJUMPING: SetAnimation((int)EnemyAnim::JUMPING_RIGHT); break;
	case hState::EFALLING: SetAnimation((int)EnemyAnim::FALLING_RIGHT); break;
	}
}
void Enemy::ChangeAnimLeft()
{
	look = hLook::ELEFT;
	switch (state)
	{
	case hState::EIDLE:	 SetAnimation((int)EnemyAnim::IDLE_LEFT);    break;
	case hState::EWALKING: SetAnimation((int)EnemyAnim::WALKING_LEFT); break;
	case hState::EJUMPING: SetAnimation((int)EnemyAnim::JUMPING_LEFT); break;
	case hState::EFALLING: SetAnimation((int)EnemyAnim::FALLING_LEFT); break;
	}
}
void Enemy::Update()
{

	MoveX();
	MoveY();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	Warp();

}
void Enemy::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	
	//Enemy walking
	
	if (type == hType::ZENCHAN) {
		pos.x += dir.x;
		box = GetHitbox();

		if (hasStartedWalking == false) {
			StartWalkingLeft();
			state = hState::EWALKING;
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
	else if (type == hType::INVADER) {
		pos.x += dir.x;
		box = GetHitbox();

		if (hasStartedWalking == false) {
			StartWalkingLeft();
			state = hState::EWALKING;
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
	
	
	
}
void Enemy::MoveY()
{
	AABB box;

	pos.y -= dir.y;
	if (type == hType::ZENCHAN) {
		pos.y += ENEMY_FALLING_SPEED;
		box = GetHitbox();
		if (!map->TestCollisionGround(box, &pos.y))
		{
			state = hState::EFALLING;
		}

	}
	else if (type == hType::INVADER) {
		box = GetHitbox();
				
	}



}

//void Enemy::LogicJumping()
//{
//	AABB box, prev_box;
//	int prev_y;
//
//	jump_delay--;
//	if (jump_delay == 0)
//	{
//		prev_y = pos.y;
//		prev_box = GetHitbox();
//
//		pos.y += dir.y;
//		dir.y += GRAVITY_FORCE;
//		jump_delay = ENEMY_JUMP_DELAY;
//
//		//Is the jump finished?
//		if (dir.y > ENEMY_JUMP_FORCE - 6)
//		{
//			dir.y = ENEMY_ENDJUMPING_SPEED;
//			StartFalling();
//		}
//		else
//		{
//			//Jumping is represented with 3 different states
//			if (IsAscending())
//			{
//				if (IsLookingRight())	SetAnimation((int)EnemyAnim::JUMPING_RIGHT);
//				else					SetAnimation((int)EnemyAnim::JUMPING_LEFT);
//			}
//			else if (IsLevitating())
//			{
//				if (IsLookingRight())	SetAnimation((int)EnemyAnim::LEVITATING_RIGHT);
//				else					SetAnimation((int)EnemyAnim::LEVITATING_LEFT);
//			}
//			else if (IsDescending())
//			{
//				if (IsLookingRight())	SetAnimation((int)EnemyAnim::FALLING_RIGHT);
//				else					SetAnimation((int)EnemyAnim::FALLING_LEFT);
//			}
//		}
//		//We check ground collision when jumping down
//		if (dir.y >= 0)
//		{
//			box = GetHitbox();
//
//			//A ground collision occurs if we were not in a collision state previously.
//			//This prevents scenarios where, after levitating due to a previous jump, we found
//			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
//			//crossing it.
//			if (!map->TestCollisionGround(prev_box, &prev_y) &&
//				map->TestCollisionGround(box, &pos.y))
//			{
//				Stop();
//
//			}
//		}
//	}
//}

void Enemy::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	if (type == hType::ZENCHAN) {
		data.ReleaseTexture(Resource::IMG_ZENCHAN);
	}
	if (type == hType::INVADER) {
		data.ReleaseTexture(Resource::IMG_INVADER);
	}

	render->Release();
}