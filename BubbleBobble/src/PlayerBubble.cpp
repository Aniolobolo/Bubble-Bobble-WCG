#include "PlayerBubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound sfx[10];

PlayerBubble::PlayerBubble(const Point& p, BubbleDirections d) : Entity(p, BUBBLE_PHYSICAL_SIZE, BUBBLE_PHYSICAL_SIZE, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	direction = d;
	speed = .3;
	bubbleStage = 1;
	bubblePosLeft = pos.x - SHOOT_RANGE;
	bubblePosRight = pos.x + SHOOT_RANGE;
	bubbleLifetime = 0;
	spawnTime = 0;
	player = nullptr;
	timeAlive = 8;
	Rectangle rc;
	inShoot = true;


	sfx[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");



}
PlayerBubble::~PlayerBubble()
{
}
void PlayerBubble::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}

AppStatus PlayerBubble::Initialise()
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

	sprite->SetNumberAnimations((int)BubbleAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BubbleAnim::IDLE, ANIM_DELAY + 3);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::IDLE, { (float)i * n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::INSHOOT, ANIM_DELAY + 6);
	for (int i = 0; i < 6; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::INSHOOT, { (float)i * n, 0, n, n });

	//Zenchan bubbles
	sprite->SetAnimationDelay((int)BubbleAnim::DEADZENCHAN, ANIM_DELAY);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::DEADZENCHAN, { (float)i * n, n * 7, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::GREENZENCHAN, ANIM_DELAY);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::GREENZENCHAN, { (float)i * n, n * 3, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::YELLOWZENCHAN, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::YELLOWZENCHAN, { (float)i * n, n * 3, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::REDZENCHAN, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::REDZENCHAN, { (float)i * n, n * 3, n, n });

	//Invader bubbles
	sprite->SetAnimationDelay((int)BubbleAnim::DEADINVADER, ANIM_DELAY);
	for (int i = 4; i < 8; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::DEADINVADER, { (float)i * n, n * 7, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::GREENINVADER, ANIM_DELAY);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::GREENINVADER, { (float)i * n, n * 4, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::YELLOWINVADER, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::YELLOWINVADER, { (float)i * n, n * 4, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::REDINVADER, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::REDINVADER, { (float)i * n, n * 4, n, n });

	//Mighta bubbles
	sprite->SetAnimationDelay((int)BubbleAnim::DEADMIGHTA, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::DEADMIGHTA, { (float)i * n, n * 7, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::GREENMIGHTA, ANIM_DELAY);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::GREENMIGHTA, { (float)i * n, n * 5, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::YELLOWMIGHTA, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::YELLOWMIGHTA, { (float)i * n, n * 5, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::REDMIGHTA, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::REDMIGHTA, { (float)i * n, n * 5, n, n });

	//Drunk bubbles
	sprite->SetAnimationDelay((int)BubbleAnim::DEADDRUNK, ANIM_DELAY);
	for (int i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::DEADDRUNK, { (float)i * n, n * 8, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::GREENDRUNK, ANIM_DELAY);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::GREENDRUNK, { (float)i * n, n * 6, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::YELLOWDRUNK, ANIM_DELAY);
	for (int i = 6; i < 9; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::YELLOWDRUNK, { (float)i * n, n * 6, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::REDDRUNK, ANIM_DELAY);
	for (int i = 9; i < 12; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::REDDRUNK, { (float)i * n, n * 6, n, n });


	sprite->SetAnimation((int)BubbleAnim::INSHOOT);
	return AppStatus::OK;

}
void PlayerBubble::Update()
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
}
void PlayerBubble::Shot()
{
	state = BubbleState::JUSTSHOT;
	SetAnimation((int)BubbleAnim::INSHOOT);
}
void PlayerBubble::Wander()
{
	if (hasZenchan) {
		state = BubbleState::ZENCHANINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENZENCHAN);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWZENCHAN);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDZENCHAN);
		}
	}
	else if (hasMighta) {
		state = BubbleState::MIGHTAINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENMIGHTA);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWMIGHTA);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDMIGHTA);
		}
	}
	else if (hasInvader) {
		state = BubbleState::INVADERINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENINVADER);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWINVADER);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDINVADER);
		}
	}
	else if (hasDrunk) {
		state = BubbleState::DRUNKINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENDRUNK);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWDRUNK);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDDRUNK);
		}
	}
	else {
		state = BubbleState::WANDER;
		SetAnimation((int)BubbleAnim::IDLE);
	}
}

void PlayerBubble::Zenchan()
{
	if (hasZenchan) {
		state = BubbleState::ZENCHANINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENZENCHAN);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWZENCHAN);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDZENCHAN);
		}
	}
}
void PlayerBubble::Mighta()
{
	if (hasMighta) {
		state = BubbleState::MIGHTAINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENMIGHTA);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWMIGHTA);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDMIGHTA);
		}
	}
}
void PlayerBubble::Invader()
{
	if (hasInvader) {
		state = BubbleState::INVADERINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENINVADER);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWINVADER);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDINVADER);
		}
	}
}
void PlayerBubble::Drunk()
{
	if (hasDrunk) {
		state = BubbleState::DRUNKINSIDE;
		if (bubbleLifetime <= 4) {
			SetAnimation((int)BubbleAnim::GREENDRUNK);
		}
		else if (bubbleLifetime > 4 && bubbleLifetime <= 6) {
			SetAnimation((int)BubbleAnim::YELLOWDRUNK);
		}
		else if (bubbleLifetime > 6) {
			SetAnimation((int)BubbleAnim::REDDRUNK);
		}
	}
}
bool PlayerBubble::isAlive()
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
void PlayerBubble::Clamp()
{

	if (pos.y < 32)
	{
		if (pos.x <= WINDOW_WIDTH / 2)
		{
			dir = { 1, 1 };
		}
		else {
			dir = { -1, 1 };
		}
	}
	if (pos.y == 32)
	{
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
void PlayerBubble::SetPlayer(Player* p)
{
	player = p;
}
hType PlayerBubble::GetEnemyType()
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
bool PlayerBubble::isJustShot()
{
	if (state == BubbleState::JUSTSHOT) {
		return true;
	}
	else {
		return false;
	}
	
}
bool PlayerBubble::isInside()
{
	return isEnemyInside;
}
void PlayerBubble::ZenchanInside()
{
	hasZenchan = true;
}
void PlayerBubble::MightaInside()
{
	hasMighta = true;
}
void PlayerBubble::InvaderInside()
{
	hasInvader = true;
}
void PlayerBubble::DrunkInside()
{
	hasDrunk = true;
}
void PlayerBubble::JumpOnBubble()
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

void PlayerBubble::Move(BubbleDirections d)
{
	Clamp();
	JumpOnBubble();
	if (pos.y > 32)
	{
		if (d == BubbleDirections::LEFT)
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
		else if (d == BubbleDirections::RIGHT)
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
void PlayerBubble::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void PlayerBubble::Release()
{

	render->Release();
}
