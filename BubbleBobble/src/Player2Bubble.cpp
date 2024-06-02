#include "Player2Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound sfx2[10];

Player2Bubble::Player2Bubble(const Point& p, Bubble2Directions d) : Entity(p, BUBBLE_PHYSICAL_SIZE, BUBBLE_PHYSICAL_SIZE, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	direction = d;
	speed = .3;
	bubbleStage = 1;
	bubblePosLeft = pos.x - SHOOT_RANGE;
	bubblePosRight = pos.x + SHOOT_RANGE;
	bubbleLifetime = 0;
	spawnTime = 0;
	player2 = nullptr;
	particles = nullptr;
	timeAlive = 8;
	Rectangle rc;
	inShoot = true;


	sfx2[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");



}
Player2Bubble::~Player2Bubble()
{
}
void Player2Bubble::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}

AppStatus Player2Bubble::Initialise()
{
	Rectangle rc;
	const int n = ITEM_BUBBLE_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	data.LoadTexture(Resource::IMG_BUBBLE, "images/bubbles.png");

	render = new Sprite(data.GetTexture(Resource::IMG_BUBBLE));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	sprite->SetNumberAnimations((int)Bubble2Anim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)Bubble2Anim::IDLE, ANIM_DELAY + 3);
	for (int i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::IDLE, { (float)i * n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::INSHOOT, ANIM_DELAY + 6);
	for (int i = 6; i < 12; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::INSHOOT, { (float)i * n, 0, n, n });

	//Zenchan bubbles
	sprite->SetAnimationDelay((int)Bubble2Anim::DEADZENCHAN, ANIM_DELAY);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::DEADZENCHAN, { (float)i * n, n * 7, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::GREENZENCHAN, ANIM_DELAY);
	for (int i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::GREENZENCHAN, { (float)i * n, n * 3, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::YELLOWZENCHAN, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::YELLOWZENCHAN, { (float)i * n, n * 3, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::REDZENCHAN, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::REDZENCHAN, { (float)i * n, n * 3, n, n });

	//Invader bubbles
	sprite->SetAnimationDelay((int)Bubble2Anim::DEADINVADER, ANIM_DELAY);
	for (int i = 4; i < 8; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::DEADINVADER, { (float)i * n, n * 7, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::GREENINVADER, ANIM_DELAY);
	for (int i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::GREENINVADER, { (float)i * n, n * 4, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::YELLOWINVADER, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::YELLOWINVADER, { (float)i * n, n * 4, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::REDINVADER, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::REDINVADER, { (float)i * n, n * 4, n, n });

	//Mighta bubbles
	sprite->SetAnimationDelay((int)Bubble2Anim::DEADMIGHTA, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::DEADMIGHTA, { (float)i * n, n * 7, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::GREENMIGHTA, ANIM_DELAY);
	for (int i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::GREENMIGHTA, { (float)i * n, n * 5, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::YELLOWMIGHTA, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::YELLOWMIGHTA, { (float)i * n, n * 5, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::REDMIGHTA, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::REDMIGHTA, { (float)i * n, n * 5, n, n });

	//Drunk bubbles
	sprite->SetAnimationDelay((int)Bubble2Anim::DEADDRUNK, ANIM_DELAY);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::DEADDRUNK, { (float)i * n, n * 8, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::GREENDRUNK, ANIM_DELAY);
	for (int i = 3; i < 6; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::GREENDRUNK, { (float)i * n, n * 6, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::YELLOWDRUNK, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::YELLOWDRUNK, { (float)i * n, n * 6, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::REDDRUNK, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::REDDRUNK, { (float)i * n, n * 6, n, n });

	sprite->SetAnimationDelay((int)Bubble2Anim::POP, ANIM_DELAY);
	for (int i = 6; i < 10; ++i)
		sprite->AddKeyFrame((int)Bubble2Anim::POP, { (float)i * n, n, n, n });



	sprite->SetAnimation((int)Bubble2Anim::IDLE);
	return AppStatus::OK;

}
void Player2Bubble::Update()
{
	pos += dir;
	Move(direction);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	if (sprite->IsAnimationComplete()) {
		Zenchan();
		Mighta();
		Invader();
		Drunk();
	}
	if (!isAlive() ) {
		Point p;
		p.x = pos.x - (TILE_SIZE - BUBBLE_FRAME_SIZE) / 2;
		p.y = pos.y - (TILE_SIZE - BUBBLE_FRAME_SIZE) / 2;
	}
}

void Player2Bubble::Shot()
{
	state = Bubble2State::JUSTSHOT;
	SetAnimation((int)Bubble2Anim::IDLE);
}

void Player2Bubble::Wander()
{
	//if (!isEnemyInside) {
	//	if (bubbleLifetime > 7 || popped) {
	//		state = Bubble2State::POP;
	//		SetAnimation((int)Bubble2Anim::POP);
	//	}
	//}
}


void Player2Bubble::Zenchan()
{
	if (hasZenchan) {
		state = Bubble2State::ZENCHANINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)Bubble2Anim::GREENZENCHAN);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)Bubble2Anim::YELLOWZENCHAN);
		}
		else if (bubbleLifetime > 6 && bubbleLifetime <= 8) {
			SetAnimation((int)Bubble2Anim::REDZENCHAN);
		}
	}
}
void Player2Bubble::Mighta()
{
	if (hasMighta) {
		state = Bubble2State::MIGHTAINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)Bubble2Anim::GREENMIGHTA);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)Bubble2Anim::YELLOWMIGHTA);
		}
		else if (bubbleLifetime > 6 && bubbleLifetime <= 8) {
			SetAnimation((int)Bubble2Anim::REDMIGHTA);
		}
	}
}
void Player2Bubble::Invader()
{
	if (hasInvader) {
		state = Bubble2State::INVADERINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)Bubble2Anim::GREENINVADER);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)Bubble2Anim::YELLOWINVADER);
		}
		else if (bubbleLifetime > 6 && bubbleLifetime <= 8) {
			SetAnimation((int)Bubble2Anim::REDINVADER);
		}
	}
}
void Player2Bubble::Drunk()
{
	if (hasDrunk) {
		state = Bubble2State::DRUNKINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)Bubble2Anim::GREENDRUNK);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)Bubble2Anim::YELLOWDRUNK);
		}
		else if (bubbleLifetime > 6 && bubbleLifetime <= 8) {
			SetAnimation((int)Bubble2Anim::REDDRUNK);
		}
	}
}
bool Player2Bubble::isAlive()
{
	bubbleLifetime += GetFrameTime();
	if (bubbleLifetime >= timeAlive)
	{
		isEnemyInside = false;
		return false;
	}
	else {
		return true;
	}
}
void Player2Bubble::Clamp()
{

	if (pos.y < 36)
	{
		if (pos.x <= WINDOW_WIDTH / 2)
		{
			dir = { 1, 1 };
		}
		else {
			dir = { -1, 1 };
		}
	}
	if (pos.y == 36)
	{
		isOnCeiling = true;
		if (pos.x <= GetRandomValue(110, WINDOW_WIDTH / 2))
		{
			dir = { 1, 0 };
		}
		else if (pos.x > GetRandomValue(WINDOW_WIDTH / 2, 140))
		{
			dir = { -1, 0 };
		}
	}

}
void Player2Bubble::SetPlayer2(Player2* p)
{
	player2 = p;
}
hType Player2Bubble::GetEnemyType()
{
	if (hasZenchan) {
		return hType::ZENCHAN;
	}
	else if (hasMighta) {
		return hType::MIGHTA;
	}
	else if (hasInvader) {
		return hType::INVADER;
	}
	else if (hasDrunk) {
		return hType::DRUNK;
	}
}
bool Player2Bubble::isJustShot()
{
	if (state == Bubble2State::JUSTSHOT) {
		return true;
	}
	else {
		return false;
	}

}
bool Player2Bubble::isInside()
{
	return isEnemyInside;
}
void Player2Bubble::ZenchanInside()
{
	hasZenchan = true;
}
void Player2Bubble::MightaInside()
{
	hasMighta = true;
}
void Player2Bubble::InvaderInside()
{
	hasInvader = true;
}
void Player2Bubble::DrunkInside()
{
	hasDrunk = true;
}
void Player2Bubble::SetParticleManager(ParticleManager* particles)
{
	this->particles = particles;
}
void Player2Bubble::JumpOnBubble()
{
	if (!inShoot)
	{
		AABB box = GetHitbox();
		//if (player != nullptr && IsKeyDown(KEY_Q))
		//{
		//	if (jumpTime > 1)
		//	{
		//		if (player->TestCollisionFromUp(box, &pos.y))
		//		{
		//			player->SetPos(player->GetPos() += { 0, BUBBLEJUMP });
		//			jumpTime = 0;
		//		}
		//	}
		//}
	}
	jumpTime += GetFrameTime();
}

void Player2Bubble::Move(Bubble2Directions d)
{
	Clamp();
	JumpOnBubble();
	if (pos.y > 36)
	{
		if (d == Bubble2Directions::LEFT)
		{

			switch (bubbleStage) {
				Shot();
			case 1:

				if (pos.x < 20)
				{
					pos.x++;
					bubbleStage++;
				}

				inShoot = true;

				dir = { -2, 0 };
				if (pos.x <= bubblePosLeft) {
					bubbleStage++;
				}
				break;
			case 2:
				Wander();

				inShoot = false;
				dir = { 0, -1 };
				break;


			}
		}
		else if (d == Bubble2Directions::RIGHT)
		{

			switch (bubbleStage) {
				Shot();
			case 1:

				if (pos.x > 226)
				{
					pos.x--;
					bubbleStage++;
				}
				inShoot = true;

				dir = { 2, 0 };
				if (pos.x >= bubblePosRight) {
					bubbleStage++;
				}
				break;
			case 2:
				Wander();

				inShoot = false;
				dir = { 0, -1 };

				break;

			}

		}
	}

}
void Player2Bubble::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Player2Bubble::Release()
{

	render->Release();
}
