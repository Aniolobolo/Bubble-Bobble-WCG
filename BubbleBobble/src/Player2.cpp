
#include "Player2.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound player2SoundEffects[30];

Player2::Player2(const Point& p, eState s, eLook view) :
	Entity(p, PLAYER2_PHYSICAL_WIDTH, PLAYER2_PHYSICAL_HEIGHT, PLAYER2_FRAME_SIZE, PLAYER2_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER2_JUMP_DELAY;
	map = nullptr;
	score = 0;
}
Player2::~Player2()
{
}
AppStatus Player2::Initialise()
{
	int i;
	const int n = PLAYER2_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER2, "images/player2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER2));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}


	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)Player2Anim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)Player2Anim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::IDLE_LEFT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)Player2Anim::WALKING_RIGHT, { (float)i*n, 4*n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)Player2Anim::WALKING_LEFT, { (float)i*n, 4*n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::FALLING_RIGHT, { 2*n, 5*n, n, n });
	sprite->AddKeyFrame((int)Player2Anim::FALLING_RIGHT, { 3*n, 5*n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::FALLING_LEFT, { 2*n, 5*n, -n, n });
	sprite->AddKeyFrame((int)Player2Anim::FALLING_LEFT, { 3*n, 5*n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::JUMPING_RIGHT, { 0, 5*n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::JUMPING_LEFT, { 0, 5*n, -n, n });
	sprite->SetAnimationDelay((int)Player2Anim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_RIGHT, { n, 5*n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_LEFT, { n, 5*n, -n, n });

	sprite->SetAnimation((int)Player2Anim::IDLE_LEFT);

	return AppStatus::OK;
}

void Player2::InitScore()
{
	score = 0;
}
void Player2::IncrScore(int n)
{
	score += n;
}
int Player2::GetScore()
{
	return score;
}
void Player2::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player2::IsLookingRight() const
{
	return look == eLook::ERIGHT;
}
bool Player2::IsLookingLeft() const
{
	return look == eLook::ELEFT;
}
bool Player2::IsAscending() const
{
	return dir.y < -PLAYER2_LEVITATING_SPEED;
}
bool Player2::IsLevitating() const
{
	return abs(dir.y) <= PLAYER2_LEVITATING_SPEED;
}
bool Player2::IsDescending() const
{
	return dir.y > PLAYER2_LEVITATING_SPEED;
}
bool Player2::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player2::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE/2;
}
void Player2::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
Player2Anim Player2::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (Player2Anim)sprite->GetAnimation();
}
void Player2::Stop()
{
	dir = { 0,0 };
	state = eState::EIDLE;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::IDLE_RIGHT);
	else					SetAnimation((int)Player2Anim::IDLE_LEFT);
}
void Player2::StartWalkingLeft()
{
	state = eState::EWALKING;
	look = eLook::ELEFT;
	SetAnimation((int)Player2Anim::WALKING_LEFT);
}
void Player2::StartWalkingRight()
{
	state = eState::EWALKING;
	look = eLook::ERIGHT;
	SetAnimation((int)Player2Anim::WALKING_RIGHT);
}
void Player2::StartFalling()
{
	dir.y = PLAYER2_FALLING_SPEED;
	state = eState::EFALLING;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::FALLING_RIGHT);
	else					SetAnimation((int)Player2Anim::FALLING_LEFT);
}
void Player2::StartJumping()
{
	player2SoundEffects[0] = LoadSound("sound/SoundEffects/Characters/JumpFX.wav");
	PlaySound(player2SoundEffects[0]);
	dir.y = -PLAYER2_JUMP_FORCE;
	state = eState::EJUMPING;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::JUMPING_RIGHT);
	else					SetAnimation((int)Player2Anim::JUMPING_LEFT);
	jump_delay = PLAYER2_JUMP_DELAY;
}
void Player2::StartClimbingUp()
{
	state = eState::ECLIMBING;
	SetAnimation((int)Player2Anim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player2::StartClimbingDown()
{
	state = eState::ECLIMBING;
	SetAnimation((int)Player2Anim::CLIMBING_TOP);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player2::ChangeAnimRight()
{
	look = eLook::ERIGHT;
	switch (state)
	{
		case eState::EIDLE:	 SetAnimation((int)Player2Anim::IDLE_RIGHT);    break;
		case eState::EWALKING: SetAnimation((int)Player2Anim::WALKING_RIGHT); break;
		case eState::EJUMPING: SetAnimation((int)Player2Anim::JUMPING_RIGHT); break;
		case eState::EFALLING: SetAnimation((int)Player2Anim::FALLING_RIGHT); break;
	}
}
void Player2::ChangeAnimLeft()
{
	look = eLook::ELEFT;
	switch (state)
	{
		case eState::EIDLE:	 SetAnimation((int)Player2Anim::IDLE_LEFT);    break;
		case eState::EWALKING: SetAnimation((int)Player2Anim::WALKING_LEFT); break;
		case eState::EJUMPING: SetAnimation((int)Player2Anim::JUMPING_LEFT); break;
		case eState::EFALLING: SetAnimation((int)Player2Anim::FALLING_LEFT); break;
	}
}

void Player2::InitLife() {

	life = 3;
}

void Player2::LifeManager() {
	if (life <= 0) {
		Die();

	}
	life--;
}

int Player2::getLife()
{
	return life;
}

void Player2::ReceiveDamage()
{
	state = eState::EDAMAGED;
	isReceivingDamage();
	if (IsLookingRight())	SetAnimation((int)Player2Anim::DAMAGE_RIGHT);
	else					SetAnimation((int)Player2Anim::DAMAGE_LEFT);
	if (life <= 0) {
		Die();
	}

}
void Player2::Die()
{
	state = eState::EDEAD;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::DIE_RIGHT);
	else                    SetAnimation((int)Player2Anim::DIE_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
}

bool Player2::isReceivingDamage()
{
	if (state == eState::EDAMAGED) {
		return true;
	}
	else {
		return false;
	}
}

void Player2::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	if (state != eState::EDEAD) {
		if (getLife() <= 0) {
			Die();
		}
		MoveX();
		MoveY();
		Warp();
	}
	if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) {
		state = eState::EIDLE;
		SetAnimation((int)Player2Anim::IDLE_LEFT);
	}

}
void Player2::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	if (state == eState::ECLIMBING)	return;

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER2_SPEED;
		if (state == eState::EIDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == eState::EWALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER2_SPEED;
		if (state == eState::EIDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == eState::EWALKING) Stop();
		}
	}
	else
	{
		if (state == eState::EWALKING) Stop();
	}
}
void Player2::MoveY()
{
	AABB box;

	if (state == eState::EJUMPING)
	{
		LogicJumping();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER2_FALLING_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == eState::EFALLING) Stop();

			//if (IsKeyDown(KEY_UP))
			//{
			//	box = GetHitbox();
			//	if (map->TestOnLadder(box, &pos.x))
			//		StartClimbingUp();
			//}
			else if (IsKeyDown(KEY_DOWN))
			{
				//To climb up the ladder, we need to check the control point (x, y)
				//To climb down the ladder, we need to check pixel below (x, y+1) instead
				box = GetHitbox();
				box.pos.y++;
				//if (map->TestOnLadderTop(box, &pos.x))
				//{
				//	StartClimbingDown();
				//	pos.y += PLAYER_LADDER_SPEED;
				//}
					
			}
			else if (IsKeyPressed(KEY_RIGHT_CONTROL))
			{
				StartJumping();
			}
		}
		else
		{
			if (state != eState::EFALLING) StartFalling();

		}
	}
}
//void Player::ShootBubble() {
//	Vector2 p;
//	
//	Shots[idx_shot]->Init({ (float)pos.x + SHOOT_POS_X, (float)pos.y + SHOOT_POS_Y }, 8, 8, BUBBLE_SPEED);
//	idx_shot++;
//	idx_shot %= MAX_SHOTS;
//
//}
void Player2::LogicJumping()
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
		jump_delay = PLAYER2_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER2_JUMP_FORCE - 6)
		{
			dir.y = PLAYER2_ENDJUMPING_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)Player2Anim::JUMPING_RIGHT);
				else					SetAnimation((int)Player2Anim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)Player2Anim::LEVITATING_RIGHT);
				else					SetAnimation((int)Player2Anim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)Player2Anim::FALLING_RIGHT);
				else					SetAnimation((int)Player2Anim::FALLING_LEFT);
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
				Stop();
			}
		}
	}
}

void Player2::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player2::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER2);

	render->Release();
}