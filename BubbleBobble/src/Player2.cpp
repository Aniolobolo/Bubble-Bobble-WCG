
#include "Player2.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "Game.h"
#include <raymath.h>

Sound player2SoundEffects[30];

Player2::Player2(const Point& p, eState s, eLook view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	score = 0;
	life = 3;
	hasTakenDamage = false;
	pTime = 0;
	damageTime = 2;
	playTime = 2;

	isDead = false;
}
Player2::~Player2()
{
}
AppStatus Player2::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

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
		sprite->AddKeyFrame((int)Player2Anim::WALKING_RIGHT, { (float)i * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)Player2Anim::WALKING_LEFT, { (float)i * n, 4 * n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::FALLING_RIGHT, { 2 * n, 5 * n, n, n });
	sprite->AddKeyFrame((int)Player2Anim::FALLING_RIGHT, { 3 * n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::FALLING_LEFT, { 2 * n, 5 * n, -n, n });
	sprite->AddKeyFrame((int)Player2Anim::FALLING_LEFT, { 3 * n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::JUMPING_RIGHT, { 0, 5 * n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::JUMPING_LEFT, { 0, 5 * n, -n, n });
	sprite->SetAnimationDelay((int)Player2Anim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_RIGHT, { n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_LEFT, { n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::CLIMBING, ANIM_LADDER_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)Player2Anim::CLIMBING, { (float)i * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::CLIMBING_PRE_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::CLIMBING_PRE_TOP, { 4 * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)Player2Anim::CLIMBING_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::CLIMBING_TOP, { 5 * n, 6 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::DAMAGE_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::DAMAGE_LEFT, { (float)i * n, n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::DAMAGE_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::DAMAGE_RIGHT, { (float)i * n, n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::SHOOTING_LEFT, ANIM_DELAY);
	for (i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Player2Anim::SHOOTING_LEFT, { (float)i * n, 7 * n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::SHOOTING_RIGHT, ANIM_DELAY);
	for (i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Player2Anim::SHOOTING_RIGHT, { (float)i * n, 7 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::DIE_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)Player2Anim::DIE_LEFT, { (float)i * n, 2 * n, -n, n });
	
	sprite->SetAnimationDelay((int)Player2Anim::DIE_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)Player2Anim::DIE_RIGHT, { (float)i * n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::INSERTCOIN, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::INSERTCOIN, { (float)i * n, 3 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::THANK, 2 * ANIM_DELAY);
	for (i = 2; i < 4; ++i)
		sprite->AddKeyFrame((int)Player2Anim::THANK, { (float)i * n, 3 * n, n, n });


	sprite->SetAnimation((int)Player2Anim::IDLE_RIGHT);
	sprite->SetAutomaticMode();
	return AppStatus::OK;
}

void Player2::InitLife() {
	state = eState::WAITFORPLAYER;
	life = 3;
}

void Player2::LifeManager() {
	if (!godMode && !wasHit && state != eState::DEAD && state != eState::WAITFORPLAYER) {  // No reducir vida si está muerto
		if (life > 0) {
			life--;
			ReceiveDamage();
		}
		else {
			isDead = true;
			Die();

		}
	}
}

int Player2::getLife()
{
	return life;
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
bool Player2::IsJumpingOnBubble(const Point& p, int distance)
{
	AABB playerHitbox = GetHitbox();

	int displacement = 50;
	if (p.y <= playerHitbox.pos.y + playerHitbox.height &&
		p.y >= playerHitbox.pos.y - displacement &&
		p.x + distance >= playerHitbox.pos.x &&
		p.x <= playerHitbox.pos.x + playerHitbox.width

		)
	{
		return true;
	}
	return false;
}
void Player2::SetDir(Point p)
{
	dir += p;
}
bool Player2::TestUpCol(const AABB& box, int* py)
{
	Point p(box.pos.x, *py);
	int tile_y;

	if (pos.y < p.y && IsJumpingOnBubble(p, box.width) /*&& pos.y +30 > p.y*/)
	{
		tile_y = (p.y + TILE_SIZE) / TILE_SIZE;

		*py -= 10;
		return true;
	}
	else {
		return false;
	}
}
void Player2::SetState(eState state)
{
	this->state = state;
}
void Player2::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player2::IsLookingRight() const
{
	return look == eLook::RIGHT;
}
bool Player2::IsLookingLeft() const
{
	return look == eLook::LEFT;
}
bool Player2::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player2::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player2::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player2::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player2::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Player2::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (state != eState::DAMAGED || id == (int)Player2Anim::DAMAGE_RIGHT || id == (int)Player2Anim::DAMAGE_LEFT) {
		sprite->SetAnimation(id);
	}

}
Player2Anim Player2::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (Player2Anim)sprite->GetAnimation();
}
void Player2::Stop()
{
	dir = { 0,0 };
	state = eState::IDLE;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::IDLE_RIGHT);
	else					SetAnimation((int)Player2Anim::IDLE_LEFT);
}

void Player2::ReceiveDamage()
{
	if (state == eState::DEAD) return;  // No recibir daño si está muerto


	eTimeHitted += GetFrameTime();

	if (!wasHit) {
		if (life <= 0) {
			Die();
		}
		else {
			state = eState::DAMAGED;
			if (IsLookingRight()) {
				SetAnimation((int)Player2Anim::DAMAGE_RIGHT);
			}
			else {
				SetAnimation((int)Player2Anim::DAMAGE_LEFT);
			}
			player2SoundEffects[3] = LoadSound("sound/SoundEffects/Characters/DeathFX.wav");
			PlaySound(player2SoundEffects[3]);

			if (getLife() <= 0) {
				Die();
			}
			hasTakenDamage = true;
			wasHit = true;
			godMode = true;
		}
	}
	if (eTimeHitted >= 0.625f) {

		Point pos;
		pos.x = WINDOW_WIDTH - 4 * TILE_SIZE;
		pos.y = 25 * TILE_SIZE + 7;
		SetPos(pos);
	}
	if (eTimeHitted >= 2) {
		wasHit = false;
		godMode = false;
		eTimeHitted = 0;
		Stop();
		state = eState::IDLE;  // Regresar al estado IDLE después del daño
	}
}

void Player2::Die()
{
	if (state == eState::DEAD) return;  // No hacer nada si ya está muerto

	isDead = true;
	state = eState::DEAD;
	eTimeDead = 0;  // Reiniciar el temporizador de muerto

	// Establecer la animación de muerte
	if (IsLookingRight()) {
		SetAnimation((int)Player2Anim::DIE_RIGHT);
	}
	else {
		SetAnimation((int)Player2Anim::DIE_LEFT);
	}
	player2SoundEffects[3] = LoadSound("sound/SoundEffects/Characters/DeathFX.wav");
	PlaySound(player2SoundEffects[3]);
}

void Player2::StartWalkingLeft()
{
	state = eState::WALKING;
	look = eLook::LEFT;
	SetAnimation((int)Player2Anim::WALKING_LEFT);
}
void Player2::StartWalkingRight()
{
	state = eState::WALKING;
	look = eLook::RIGHT;
	SetAnimation((int)Player2Anim::WALKING_RIGHT);
}
void Player2::StartFalling()
{
	dir.y = PLAYER_FALLING_SPEED;
	state = eState::FALLING;
	isJumping = false;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::FALLING_RIGHT);
	else					SetAnimation((int)Player2Anim::FALLING_LEFT);
}
void Player2::StartJumping()
{
	player2SoundEffects[0] = LoadSound("sound/SoundEffects/Characters/JumpFX.wav");
	PlaySound(player2SoundEffects[0]);
	dir.y = -PLAYER_JUMP_FORCE;
	state = eState::JUMPING;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::JUMPING_RIGHT);
	else					SetAnimation((int)Player2Anim::JUMPING_LEFT);
	isJumping = true;
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player2::StartClimbingUp()
{
	state = eState::CLIMBING;
	SetAnimation((int)Player2Anim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player2::StartClimbingDown()
{
	state = eState::CLIMBING;
	SetAnimation((int)Player2Anim::CLIMBING_TOP);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player2::StartWaiting()
{
	state = eState::WAITFORPLAYER;
	SetAnimation((int)Player2Anim::INSERTCOIN);
	eTimePlay = 0; // Reiniciar el temporizador
}
void Player2::StartPlaying()
{
	player2SoundEffects[6] = LoadSound("sound/SoundEffects/Intro/CoinInsertedFX.wav");
	PlaySound(player2SoundEffects[6]);
	state = eState::PLAYING;
	SetAnimation((int)Player2Anim::THANK);
	
	eTimePlay = 0; // Reiniciar el temporizador
}
void Player2::StartShooting()
{
	if (CanStartShooting()) {
		state = eState::SHOOTING;
		if (IsLookingRight())	SetAnimation((int)Player2Anim::SHOOTING_RIGHT);
		else					SetAnimation((int)Player2Anim::SHOOTING_LEFT);
	}
}
bool Player2::CanStartShooting()
{
	if (state == eState::DAMAGED) {
		return false;
	}
	else if (state == eState::DEAD) {
		return false;
	}
	else {
		return true;
	}
}
void Player2::ChangeAnimRight()
{
	look = eLook::RIGHT;
	switch (state)
	{
	case eState::IDLE:	 SetAnimation((int)Player2Anim::IDLE_RIGHT);    break;
	case eState::WALKING: SetAnimation((int)Player2Anim::WALKING_RIGHT); break;
	case eState::JUMPING: SetAnimation((int)Player2Anim::JUMPING_RIGHT); break;
	case eState::FALLING: SetAnimation((int)Player2Anim::FALLING_RIGHT); break;
	case eState::DAMAGED: SetAnimation((int)Player2Anim::DAMAGE_RIGHT); break;
	case eState::DEAD: SetAnimation((int)Player2Anim::DIE_RIGHT); break;
	}
}
void Player2::ChangeAnimLeft()
{
	look = eLook::LEFT;
	switch (state)
	{
	case eState::IDLE:	 SetAnimation((int)Player2Anim::IDLE_LEFT);    break;
	case eState::WALKING: SetAnimation((int)Player2Anim::WALKING_LEFT); break;
	case eState::JUMPING: SetAnimation((int)Player2Anim::JUMPING_LEFT); break;
	case eState::FALLING: SetAnimation((int)Player2Anim::FALLING_LEFT); break;
	case eState::DAMAGED: SetAnimation((int)Player2Anim::DAMAGE_LEFT); break;
	case eState::DEAD: SetAnimation((int)Player2Anim::DIE_LEFT); break;
	}
}
void Player2::isReceivingDamage()
{
	wasHit = true;
}
void Player2::SetDeathAnim()
{
	if (IsLookingLeft()) {
		SetAnimation((int)Player2Anim::DIE_LEFT);
	}
	else {
		SetAnimation((int)Player2Anim::DIE_RIGHT);
	}

}
void Player2::Update()
{
	// Actualizar sprite
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	if (state != eState::WAITFORPLAYER) {
		canPlay = true;
		if (state == eState::DEAD) {
			eTimeDead += GetFrameTime();
			if (eTimeDead >= 1.25f) {
				gameOver = true;
			}
			return;
		}

		if (wasHit) {
			ReceiveDamage();
		}

		if (life <= 0 && state != eState::DEAD) {
			Die();
		}

		if (state != eState::DAMAGED && state != eState::PLAYING) {
			MoveX();
			MoveY();
			Warp();

			if (IsKeyPressed(KEY_F2)) {
				godMode = !godMode;
			}

			if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) {
				state = eState::IDLE;
				SetAnimation((int)Player2Anim::IDLE_LEFT);
			}

			if (sprite->IsAnimationComplete() && !wasHit) {
				Stop();
			}
		}
	}

	if (state == eState::WAITFORPLAYER) {
		// Wait for player to press 'I' to start playing
		if (IsKeyPressed(KEY_I)) {
			StartPlaying();
		}
		else {
			StartWaiting();
		}
		
	}
	else if (state == eState::PLAYING) {
		// Update play time and transition back to IDLE after a certain duration
		eTimePlay += GetFrameTime();
		
		if (eTimePlay > 2.0f) {
			state = eState::IDLE;
			look = eLook::LEFT;
			eTimePlay = 0; // Reset timer
		}

	}
}

void Player2::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	if (state == eState::CLIMBING)	return;

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER_SPEED;
		if (state == eState::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == eState::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER_SPEED;
		if (state == eState::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == eState::WALKING) Stop();
		}
	}
	else
	{
		if (state == eState::WALKING) Stop();
	}
}
void Player2::MoveY()
{
	AABB box;

	if (state == eState::JUMPING)
	{
		LogicJumping();
	}
	else if (state == eState::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_FALLING_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == eState::FALLING) Stop();

			//if (IsKeyDown(KEY_UP))
			//{
			//	box = GetHitbox();
			//	if (map->TestOnLadder(box, &pos.x))
			//		StartClimbingUp();
			//}
			if (IsKeyPressed(KEY_UP) && state != eState::DEAD)
			{
				StartJumping();
			}
		}
		else
		{
			if (state != eState::FALLING) StartFalling();

		}
	}
}
//void Player2::ShootBubble() {
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
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE - 6)
		{
			dir.y = PLAYER_ENDJUMPING_SPEED;
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
void Player2::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP))
	{
		pos.y -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_LADDER_SPEED;
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();
	//if (map->TestOnLadderTop(box, &tmp))
	//{
	//	if (IsInSecondHalfTile())		SetAnimation((int)PlayerAnim::CLIMBING_PRE_TOP);
	//	else if (IsInFirstHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
	//	else					LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
	//}
	if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	//else if (!map->TestOnLadder(box, &tmp))
	//{
	//	//Case leaving the ladder ascending.
	//	//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
	//	//ther ladder ascending.
	//	Stop();
	//	sprite->SetAutomaticMode();
	//}
	else
	{
		if (GetAnimation() != Player2Anim::CLIMBING)	SetAnimation((int)Player2Anim::CLIMBING);
	}
}
void Player2::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player2::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER2);

	render->Release();
}