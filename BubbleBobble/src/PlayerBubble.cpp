#include "PlayerBubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound sfx[10];

PlayerBubble::PlayerBubble(const Point& p, Directions d) : Entity(p, BUBBLE_PHYSICAL_SIZE, BUBBLE_PHYSICAL_SIZE, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	dire = d;
	speed = .3;
	stages = 1;
	logPosXL = pos.x - SHOOT_RANGE;
	logPosXR = pos.x + SHOOT_RANGE;
	eTime = 0;
	spawnTime = 0;
	player = nullptr;
	issAlive = GetRandomValue(3, 4);
	Rectangle rc;
	inShoot = true;
	eTimePogo = 0;
	canCollide = true;
	issAlive = true;
	poped = false;
	framecounter = 0;
	fruit = false;

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
	if (poped == false)
	{
		Movement(dire);
		pos += dir;
	}
	else
	{
		dire = BublePop(dire);
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	HandleCollisionLogic();
	if (inCatch) {
		switch (enemytype)
		{
		case 0:
			EnemyCatchSlime();
			break;
		case 1:
			EnemyCatchDrunk();
			break;
		}
	}

}

bool PlayerBubble::isAlive()
{
	if (!inCatch)
	{
		eTime += GetFrameTime();
		if (eTime >= lifeTime)
		{
			return false;
		}
		else {
			return true;
		}
	}

}
void PlayerBubble::ClampPos()
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
Point PlayerBubble::GetPos() const
{
	return pos;
}
void PlayerBubble::EnemyCatchSlime()
{
	if (poped == false)
	{
		switch (bubbleStages)
		{
		case (int)BubbleStages::GREENSTAGE:
			SetAnimation((int)BubbleAnim::GREENZENCHAN);
			bubbleStages++;
			break;
		case (int)BubbleStages::GREENSTAGE_:
			if (eTimeCatch > eTimeCatchGreen) bubbleStages++;
			eTimeCatch += GetFrameTime();
			break;
		case (int)BubbleStages::YELLOWSTAGE:
			eTimeCatch = 0;
			SetAnimation((int)BubbleAnim::YELLOWZENCHAN);
			bubbleStages++;
			break;
		case (int)BubbleStages::YELLOWSTAGE_:
			if (eTimeCatch > eTimeCatchYellow) bubbleStages++;
			eTimeCatch += GetFrameTime();
			break;
		case (int)BubbleStages::REDSTAGE:
			eTimeCatch = 0;
			SetAnimation((int)BubbleAnim::REDZENCHAN);
			bubbleStages++;
			break;
		case (int)BubbleStages::REDSTAGE_:
			if (eTimeCatch > eTimeCatchRed) bubbleStages++;
			eTimeCatch += GetFrameTime();
			break;
		case(int)BubbleStages::ENDED:
			hasEndedFromCatch = true;
			break;
		}
	}
}
void PlayerBubble::EnemyCatchDrunk()
{
	if (poped == false)
	{
		switch (bubbleStages)
		{
		case (int)BubbleStages::GREENSTAGE:
			SetAnimation((int)BubbleAnim::GREENDRUNK);
			bubbleStages++;
			break;
		case (int)BubbleStages::GREENSTAGE_:
			if (eTimeCatch > eTimeCatchGreen) bubbleStages++;
			eTimeCatch += GetFrameTime();
			break;
		case (int)BubbleStages::YELLOWSTAGE:
			eTimeCatch = 0;
			SetAnimation((int)BubbleAnim::YELLOWDRUNK);
			bubbleStages++;
			break;
		case (int)BubbleStages::YELLOWSTAGE_:
			if (eTimeCatch > eTimeCatchYellow) bubbleStages++;
			eTimeCatch += GetFrameTime();
			break;
		case (int)BubbleStages::REDSTAGE:
			eTimeCatch = 0;
			SetAnimation((int)BubbleAnim::REDDRUNK);
			bubbleStages++;
			break;
		case (int)BubbleStages::REDSTAGE_:
			if (eTimeCatch > eTimeCatchRed) bubbleStages++;
			eTimeCatch += GetFrameTime();
			break;
		case(int)BubbleStages::ENDED:
			hasEndedFromCatch = true;
			break;
		}
	}
}
void PlayerBubble::SetTileMap(TileMap* m)
{
	map = m;
}
void PlayerBubble::Movement(Directions d)
{
	ClampPos();

	if (poped == true)
	{
		return;
	}

	if (pos.y > 32)
	{
		if (d == Directions::LEFT)
		{
			switch (stages) {
				SetAnimation((int)BubbleAnim::INSHOOT);
			case 1:
				if (pos.x < 20)
				{
					pos.x++;
					if (!inCatch) SetAnimation((int)BubbleAnim::IDLE);

					stages++;
				}
				inShoot = true;

				dir = { -2, 0 };
				if (pos.x <= logPosXL) {
					if (!inCatch) SetAnimation((int)BubbleAnim::IDLE);

					stages++;
				}
				break;
			case 2:
				canCollide = false;

				inShoot = false;
				dir = { 0, -1 };

				break;


			}
		}
		else if (d == Directions::RIGHT)
		{

			switch (stages) {
				SetAnimation((int)BubbleAnim::INSHOOT);
			case 1:
				if (pos.x > 226)
				{
					pos.x--;
					if (!inCatch) SetAnimation((int)BubbleAnim::IDLE);

					stages++;
				}
				inShoot = true;

				dir = { 2, 0 };
				if (pos.x >= logPosXR) {
					if (!inCatch) SetAnimation((int)BubbleAnim::IDLE);

					stages++;
				}
				break;
			case 2:
				canCollide = false;

				inShoot = false;

				dir = { 0, -1 };

				break;

			}

		}
	}

}
void PlayerBubble::MoveBubbleRightPlayer()
{
	AABB box;
	direction = Directions::RIGHT;
	pos += {1, 0};
	box = GetHitbox();
	if (!map->TestCollisionAir(box)) {
		issAlive = false;
	}
}

void PlayerBubble::MoveBubbleLeftPlayer()
{
	AABB box;
	direction = Directions::LEFT;
	pos += {-1, 0};
	box = GetHitbox();
	if (!map->TestCollisionAir(box)) {
		issAlive = false;
	}
}
void PlayerBubble::HandleCollisionLogic()
{

}
void PlayerBubble::MoveBubbleToRandomNear()
{
	float ranIndexX = GetRandomValue(0, 1);
	float ranIndexY = GetRandomValue(0, 1);

	pos.x -= ranIndexX;
	pos.y -= ranIndexY;
}
void PlayerBubble::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void PlayerBubble::Release()
{

	render->Release();
}
Directions PlayerBubble::BublePop(Directions d)
{

	if (pos.x <= 16)
	{
		d = Directions::RIGHT;

	}
	if (pos.x + 16 >= 240)
	{
		d = Directions::LEFT;

	}


	if (d == Directions::LEFT)
	{
		if ((framecounter / 60) <= 0.5)
		{
			pos.x = pos.x - 1;
			pos.y = pos.y - 2;

			framecounter++;
		}
		else if (0.5 < (framecounter / 60) && (framecounter / 60) <= 1)
		{
			pos.x = pos.x - 1;
			pos.y = pos.y - 1;
			framecounter++;
		}
		else if (1 < (framecounter / 60))
		{
			pos.y = pos.y + 3;
			pos.x = pos.x - 1;

			framecounter++;
			if (map->TestCollisionGround(GetHitbox(), &pos.y) == true)
			{
				fruit = true;
			}
		}
		return Directions::LEFT;
	}
	else if (d == Directions::RIGHT)
	{

		if ((framecounter / 60) <= 0.5)
		{
			pos.x = pos.x + 2;
			pos.y = pos.y - 3;
			framecounter++;
		}
		else if (0.5 < (framecounter / 60) && (framecounter / 60) <= 1)
		{
			pos.x = pos.x + 2;
			pos.y = pos.y - 2;

			framecounter++;
		}
		else if (1 < (framecounter / 60))
		{
			pos.y = pos.y + 4;
			pos.x = pos.x + 2;

			framecounter++;
			if (map->TestCollisionGround(GetHitbox(), &pos.y) == true)
			{
				fruit = true;
			}
		}
		return Directions::RIGHT;

	}
}
