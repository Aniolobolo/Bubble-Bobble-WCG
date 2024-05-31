
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
	player = nullptr;
	player2 = nullptr;
	map = nullptr;
	score = 0;
	if (type == hType::DRUNK) {
		dir = { static_cast<int>(-DRUNK_SPEED), 0 };
	}
	else {
		dir = { -1,0 };
	}
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
	if (data.LoadTexture(Resource::IMG_MIGHTA, "images/mighta.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	if (data.LoadTexture(Resource::IMG_DRUNK, "images/drunk.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	switch (type)
	{
	case hType::ZENCHAN:render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN)); break;
	case hType::INVADER:render = new Sprite(data.GetTexture(Resource::IMG_INVADER)); break;
	case hType::MIGHTA:render = new Sprite(data.GetTexture(Resource::IMG_MIGHTA)); break;
	case hType::DRUNK:render = new Sprite(data.GetTexture(Resource::IMG_DRUNK)); break;

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
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i * n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)i * n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_RIGHT, { 2 * n, 6 * n, n, n });
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_RIGHT, { 3 * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_LEFT, { 2 * n, 6 * n, -n, n });
	sprite->AddKeyFrame((int)EnemyAnim::FALLING_LEFT, { 3 * n, 6 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::JUMPING_RIGHT, { 0, 5 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::JUMPING_LEFT, { 0, 5 * n, -n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::LEVITATING_RIGHT, { n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::LEVITATING_LEFT, { n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::SHOOT_RIGHT, ANIM_DELAY + 4);
	for (i = 0; i < 5; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::SHOOT_RIGHT, { (float)i * n, 7 * n, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::SHOOT_LEFT, ANIM_DELAY + 3);
	for (i = 0; i < 5; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::SHOOT_LEFT, { (float)i * n, 7 * n, -n, n });


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
hType Enemy::GetType() const
{
	return type;
}
void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Enemy::SetPlayer(Player* play)
{
	player = play;
}
void Enemy::SetPlayer2(Player2* play2)
{
	player2 = play2;
}
void Enemy::DestroyEnemy(Enemy* enemy)
{
	auto it = std::find(enemies.begin(), enemies.end(), enemy);
	if (it != enemies.end())
	{
		delete* it;
		enemies.erase(it);
	}
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
	if (type != hType::DRUNK) {
		return dir.y < -ENEMY_LEVITATING_SPEED;
	}
	else {
		return dir.y < -DRUNK_LEVITATING_SPEED;
	}
}
bool Enemy::IsLevitating() const
{
	if (type != hType::DRUNK) {
		return abs(dir.y) <= ENEMY_LEVITATING_SPEED;
	}
	else {
		return abs(dir.y) <= DRUNK_LEVITATING_SPEED;
	}
}
bool Enemy::IsDescending() const
{
	if (type != hType::DRUNK) {
		return dir.y > ENEMY_LEVITATING_SPEED;
	}
	else {
		return dir.y > DRUNK_LEVITATING_SPEED;
	}
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
}
void Enemy::StartWalkingLeft()
{
	if (type != hType::DRUNK) {
		pos.x += -ENEMY_SPEED;
	}
	else {
		pos.x += -DRUNK_SPEED;
	}
	state = hState::EWALKING;
	look = hLook::ELEFT;
	SetAnimation((int)EnemyAnim::WALKING_LEFT);
}
void Enemy::StartWalkingRight()
{
	if (type != hType::DRUNK) {
		pos.x += ENEMY_SPEED;
	}
	else {
		pos.x += DRUNK_SPEED;
	}
	state = hState::EWALKING;
	look = hLook::ERIGHT;
	SetAnimation((int)EnemyAnim::WALKING_RIGHT);
}
void Enemy::StartFalling()
{
	if (type == hType::ZENCHAN) {
		dir.y = ENEMY_FALLING_SPEED;
	}
	else if (type == hType::MIGHTA) {
		dir.y = ENEMY_FALLING_SPEED;
	}
	else if (type == hType::DRUNK) {
		dir.y = DRUNK_FALLING_SPEED;
	}
	/*state = hState::EFALLING;*/
	if (IsLookingRight())	SetAnimation((int)EnemyAnim::FALLING_RIGHT);
	else					SetAnimation((int)EnemyAnim::FALLING_LEFT);
}
void Enemy::StartJumping()
{
	if (type == hType::ZENCHAN) {
		dir.y = -ENEMY_JUMP_FORCE;
	}
	else if (type == hType::MIGHTA) {
		dir.y = -ENEMY_JUMP_FORCE;
	}
	else if (type == hType::DRUNK) {
		dir.y = -DRUNK_JUMP_FORCE;
	}
	state = hState::EJUMPING;
	if (IsLookingRight())	SetAnimation((int)EnemyAnim::JUMPING_RIGHT);
	else					SetAnimation((int)EnemyAnim::JUMPING_LEFT);
	jump_delay = ENEMY_JUMP_DELAY;
	
}
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

void Enemy::StartShooting()
{
	if (checkIfCanShoot()) {
		state = hState::ESHOOTING;
		if (IsLookingRight())	SetAnimation((int)EnemyAnim::SHOOT_RIGHT);
		else					SetAnimation((int)EnemyAnim::SHOOT_LEFT);
		drunkCanShoot = false;
		mightaCanShoot = false;
		invaderCanShoot = false;

	}

}
bool Enemy::CanStartShooting()
{
	if (isShotAnimDone) {
		return true;
	}
	else {
		return false;
	}
}

void Enemy::Update()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update(); // Actualizar la animaci�n del sprite
	MoveY();
	Warp();
	if (state != hState::EJUMPING && state != hState::EFALLING && state != hState::ESHOOTING) {
		MoveX();
	}
	if (sprite->IsAnimationComplete() && state == hState::ESHOOTING) {
		isShotAnimDone = true;
		state = hState::EWALKING;
		if (IsLookingRight())	SetAnimation((int)EnemyAnim::WALKING_RIGHT);
		else					SetAnimation((int)EnemyAnim::WALKING_LEFT);
		
		
	}
}




bool Enemy::checkIfCanShoot()
{
	if (drunkCanShoot || mightaCanShoot || invaderCanShoot) {
		return true;
	}
	else {
		return false;
	}
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
				dir.x = ENEMY_SPEED;
			}
			else if (map->TestCollisionWallRight(box)) {
				pos.x = prev_x;
				StartWalkingLeft();
				dir.x = -ENEMY_SPEED;
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
				dir.x = ENEMY_SPEED;
			}
			else if (map->TestCollisionWallRight(box)) {
				pos.x = prev_x;
				StartWalkingLeft();
				dir.x = -ENEMY_SPEED;
			}
		}
	}
	if (type == hType::MIGHTA) {
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
				dir.x = ENEMY_SPEED;
			}
			else if (map->TestCollisionWallRight(box)) {
				pos.x = prev_x;
				StartWalkingLeft();
				dir.x = -ENEMY_SPEED;
			}
		}
	}
	if (type == hType::DRUNK) {
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
				dir.x = DRUNK_SPEED;
			}
			else if (map->TestCollisionWallRight(box)) {
				pos.x = prev_x;
				StartWalkingLeft();
				dir.x = -DRUNK_SPEED;
			}
		}
	}

}
void Enemy::MoveY()
{
	AABB box;

	if (type == hType::ZENCHAN) {
		
		if (state == hState::EJUMPING)
		{
			LogicJumping();
		}
		else //idle, walking, falling
		{
			pos.y += ENEMY_SPEED;
			box = GetHitbox();
			Point playerpos = player->GetPos();
			Point player2pos = player2->GetPos();
			if (map->TestCollisionGround(box, &pos.y))
			{
				state == hState::EWALKING;
				if (playerpos.y < pos.y || player2pos.y < pos.y)
				{
					ZtimerTime += GetFrameTime();
					if (ZtimerTime > 0.5f) {
						StartJumping();
						ZtimerTime = 0;
					}
					
					
				}
			}

		}
	}
	else if (type == hType::INVADER) {
		
		box = GetHitbox();
		invaderCanShoot = true;
		ItimerTime += GetFrameTime();
		if (ItimerTime <= 5.0f)
		{
			state = hState::EWALKING;
		}
		else if (ItimerTime > 5.0f && ItimerTime < 5.1f && state != hState::ESHOOTING)
		{
			state = hState::EFALLING;
			pos.y += ENEMY_FALLING_SPEED;
		}
		else if (ItimerTime > 5.1f)
		{
			ItimerTime = 0;
		}
	}
	if (type == hType::MIGHTA) {
		
		if (state == hState::EJUMPING)
		{
			LogicJumping();
		}
		else //idle, walking, falling
		{
			pos.y += ENEMY_SPEED;
			box = GetHitbox();
			Point playerpos = player->GetPos();
			Point player2pos = player2->GetPos();
			if (map->TestCollisionGround(box, &pos.y))
			{
				state == hState::EWALKING;
				mightaCanShoot = true;
				if (playerpos.y < pos.y || player2pos.y < pos.y)
				{
					MtimerTime += GetFrameTime();
					if (MtimerTime > 0.65f && state != hState::ESHOOTING) {
						StartJumping();
						mightaCanShoot = false;
						MtimerTime = 0;
					}


				}
			}
			else {
				mightaCanShoot = false;
			}
		}
	}
	if (type == hType::DRUNK) {
		
		if (state == hState::EJUMPING)
		{
			LogicJumping();
		}
		else //idle, walking, falling
		{
			pos.y += ENEMY_SPEED;
			box = GetHitbox();
			Point playerpos = player->GetPos();
			Point player2pos = player2->GetPos();
			if (map->TestCollisionGround(box, &pos.y) || (map->TestCollisionGround(box, &pos.y) && state == hState::EFALLING))
			{
				state == hState::EWALKING;
				drunkCanShoot = true;
				if (playerpos.y < pos.y || player2pos.y < pos.y)
				{
					DtimerTime += GetFrameTime();
					if (DtimerTime > 0.25f && state != hState::ESHOOTING) {
						StartJumping();
						DtimerTime = 0;
					}


				}
			}
			else {
				drunkCanShoot = false;
			}
		}
	}
}

void Enemy::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;
	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = ENEMY_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > ENEMY_JUMP_FORCE)
		{
			dir.y = DRUNK_FALLING_SPEED;
			
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)EnemyAnim::JUMPING_RIGHT);
				else					SetAnimation((int)EnemyAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)EnemyAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)EnemyAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)EnemyAnim::FALLING_RIGHT);
				else					SetAnimation((int)EnemyAnim::FALLING_LEFT);
			}

		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				state = hState::EWALKING;
			}
		}
	}
}

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
	if (type == hType::MIGHTA) {
		data.ReleaseTexture(Resource::IMG_MIGHTA);
	}
	if (type == hType::DRUNK) {
		data.ReleaseTexture(Resource::IMG_DRUNK);
	}

	render->Release();
}
