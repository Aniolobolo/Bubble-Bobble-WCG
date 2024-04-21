#include "Player2Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player2Bubble::Player2Bubble(const Point& p, Directions d) : Entity(p, BUBBLE_PHYSICAL_SIZE, BUBBLE_PHYSICAL_SIZE, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	direc = d;
	speed = .3;
	level = 1;
	logPosXL = pos.x - SHOOT_RANGE;
	logPosXR = pos.x + SHOOT_RANGE;
	bTime = 0;
	spawnTime = 0;
	player2 = nullptr;
	timeAlive = GetRandomValue(3, 4);
	Rectangle rc;
	inShoot = true;
	jumpTime = 0;


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

	sprite->SetNumberAnimations((int)BubbleAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BubbleAnim::IDLE, ANIM_DELAY + 3);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::IDLE, { (float)i * n, n, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::INSHOOT, ANIM_DELAY + 6);
	for (int i = 0; i < 6; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::INSHOOT, { (float)i * n, 0, n, n });

	sprite->SetAnimation((int)BubbleAnim::INSHOOT);
	return AppStatus::OK;

}
void Player2Bubble::Update()
{
	pos += dir;
	Movement(direc);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

}
void Player2Bubble::Shot()
{
	state = BubbleState::JUSTSHOT;
	SetAnimation((int)BubbleAnim::INSHOOT);
}
void Player2Bubble::Wander()
{
	state = BubbleState::WANDER;
	SetAnimation((int)BubbleAnim::IDLE);
}
void Player2Bubble::Ceiling()
{
	state = BubbleState::ISONCEILING;
	SetAnimation((int)BubbleAnim::IDLE);
}
bool Player2Bubble::isAlive()
{
	bTime += GetFrameTime();
	if (bTime >= timeAlive)
	{
		return false;
	}
	else {
		return true;
	}
}
void Player2Bubble::ClampPos()
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
void Player2Bubble::SetPlayer(Player2* p)
{
	player2 = p;
}
void Player2Bubble::Stomp()
{
	if (!inShoot)
	{
		AABB box = GetHitbox();
		/*if (player2 != nullptr && IsKeyDown(KEY_Q))
		{
			if (jumpTime > 1)
			{
				if (player2->TestCollisionFromUp(box, &pos.y))
				{
					player2->SetPos(player2->GetPos() += { 0, BUBBLEJUMP });
					jumpTime = 0;
				}
			}
		}*/
	}
	jumpTime += GetFrameTime();
}

void Player2Bubble::Movement(Directions d)
{
	ClampPos();
	Stomp();
	if (pos.y > 32)
	{
		if (d == Directions::LEFT)
		{

			switch (level) {
				Shot();
			case 1:

				if (pos.x < 20)
				{
					pos.x++;
					level++;
				}
				inShoot = true;

				dir = { -2, 0 };
				if (pos.x <= logPosXL) {
					level++;
				}
				break;
			case 2:
				Wander();

				inShoot = false;
				dir = { 0, -1 };
				break;


			}
		}
		else if (d == Directions::RIGHT)
		{

			switch (level) {
				Shot();
			case 1:

				if (pos.x > 226)
				{
					pos.x--;
					level++;
				}
				inShoot = true;

				dir = { 2, 0 };
				if (pos.x >= logPosXR) {
					level++;
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
