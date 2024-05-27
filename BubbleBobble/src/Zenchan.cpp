#include "Zenchan.h"
#include "Sprite.h"

Zenchan::Zenchan(const Point& p, int width, int height, int frame_width, int frame_height, TileMap* mapp) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = ZenchanState::ROAMING;
	map = mapp;
	current_step = 0;
	current_frames = 0;
}
Zenchan::~Zenchan()
{
}
AppStatus Zenchan::Initialise(Look look, const AABB& area)
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
	sprite->SetNumberAnimations((int)ZenchanAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZenchanAnim::IDLE_RIGHT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::IDLE_RIGHT, { float(i) * n, 3 * n, n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::IDLE_LEFT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::IDLE_LEFT, { float(i) * n, 3 * n, -n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_RIGHT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 5; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_RIGHT, { (float)i * n, 3 * n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_LEFT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 5; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_LEFT, { (float)i * n, 3 * n, -n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::ATTACK_RIGHT, ZENCHAN_ANIM_DELAY);
	for (i = 5; i < 10; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::ATTACK_RIGHT, { (float)i * n, 3 * n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::ATTACK_LEFT, ZENCHAN_ANIM_DELAY);
	for (i = 5; i < 10; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::ATTACK_LEFT, { (float)i * n, 3 * n, -n, n });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)ZenchanAnim::IDLE_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)ZenchanAnim::IDLE_RIGHT);

	visibility_area = area;
	notSpawningAgain = false;


	return AppStatus::OK;
}
void Zenchan::BubbleSetter(PlayerBubble* bub)
{
	for(PlayerBubble * buble: bubbles) {
		bubbles.push_back(bub);
	}
}
bool Zenchan::Update(const AABB& box)
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

void Zenchan::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
	if (look == Look::RIGHT && state != ZenchanState::FALLING && map->TestCollisionGround(box, &pos.y))
	{
		pos.x += ZENCHAN_SPEED;
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			look = Look::LEFT;
			SetAnimation((int)ZenchanAnim::WALKING_LEFT);
		}
		else if (map->TestCollisionWallLeft(box)) {
			pos.x = prev_x;
			look = Look::LEFT;
			SetAnimation((int)ZenchanAnim::WALKING_RIGHT);


		}
		
	}
	else if (look == Look::LEFT && state != ZenchanState::FALLING && map->TestCollisionGround(box, &pos.y))
	{
		pos.x += -ZENCHAN_SPEED;
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			look = Look::RIGHT;
			SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
		}
		else if (map->TestCollisionWallRight(box)) {
			pos.x = prev_x;
			look = Look::RIGHT;
			SetAnimation((int)ZenchanAnim::WALKING_LEFT);

		}
	}
}
EnemyType Zenchan::GetEnemyType() const
{
	return EnemyType::ZENCHAN;
}
void Zenchan::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Zenchan::StartFalling()
{
	dir.y = 1;
}
void Zenchan::Stop()
{
	dir = { 0,0 };
	state = ZenchanState::ROAMING;
}
void Zenchan::MoveY()
{
	AABB box, prev_box;
	int prev_x = pos.x;
	int prev_y = pos.y;


	if (state != ZenchanState::JUMPING)
	{
		pos.y += 1;
		
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == ZenchanState::FALLING) Stop();
			if (IsKeyPressed(KEY_X))
				dir.y = -1;
		}
		else
		{
			if (state != ZenchanState::FALLING) StartFalling();
		}
		
		
	}
	
	
}

void Zenchan::UpdateLook(int anim_id)
{
	ZenchanAnim anim = (ZenchanAnim)anim_id;
	look = (anim == ZenchanAnim::WALKING_LEFT) ? Look::LEFT : Look::RIGHT;
}
void Zenchan::GetShootingPosDir(Point* p, Point* d) const
{
	if (look == Look::LEFT)
	{
		p->x = pos.x + ZENCHAN_SHOT_OFFSET_X_LEFT;
		*d = { -ZENCHAN_SHOT_SPEED, 0 };
	}
	else
	{
		p->x = pos.x + ZENCHAN_SHOT_OFFSET_X_RIGHT;
		*d = { ZENCHAN_SHOT_SPEED, 0 };
	}
	p->y = pos.y + ZENCHAN_SHOT_OFFSET_Y;
}