#include "Mighta.h"
#include "Sprite.h"

Mighta::Mighta(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = MightaState::ROAMING;

	current_step = 0;
	current_frames = 0;
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
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_RIGHT, { 0, 3 * n, n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_RIGHT, { n, 3 * n, n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_RIGHT, { n * 2, 3 * n, n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_RIGHT, { n * 3, 3 * n, n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_RIGHT, { n * 4, 3 * n, n, n });
	sprite->SetAnimationDelay((int)MightaAnim::ATTACK_LEFT, MIGHTA_ANIM_DELAY);
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_LEFT, { 0, 3 * n, -n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_LEFT, { n, 3 * n, -n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_LEFT, { n * 2, 3 * n, -n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_LEFT, { n * 3, 3 * n, -n, n });
	sprite->AddKeyFrame((int)MightaAnim::ATTACK_LEFT, { n * 4, 3 * n, -n, n });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)MightaAnim::IDLE_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)MightaAnim::IDLE_RIGHT);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void Mighta::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = MIGHTA_ANIM_DELAY * 3;

	pattern.push_back({ {0, 0}, 2 * n, (int)MightaAnim::IDLE_RIGHT });
	pattern.push_back({ {MIGHTA_SPEED, 0}, n, (int)MightaAnim::WALKING_RIGHT });
	pattern.push_back({ {0, 0}, n, (int)MightaAnim::IDLE_RIGHT });
	pattern.push_back({ {MIGHTA_SPEED, 0}, n, (int)MightaAnim::WALKING_RIGHT });
	pattern.push_back({ {0, 0}, n, (int)MightaAnim::IDLE_RIGHT });

	pattern.push_back({ {0, 0}, 2 * n, (int)MightaAnim::IDLE_LEFT });
	pattern.push_back({ {-MIGHTA_SPEED, 0}, n, (int)MightaAnim::WALKING_LEFT });
	pattern.push_back({ {0, 0}, n, (int)MightaAnim::IDLE_LEFT });
	pattern.push_back({ {-MIGHTA_SPEED, 0}, n, (int)MightaAnim::WALKING_LEFT });
	pattern.push_back({ {0, 0}, n, (int)MightaAnim::IDLE_LEFT });

	current_step = 0;
	current_frames = 0;
}
bool Mighta::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	if (state == MightaState::ROAMING)
	{
		if (IsVisible(box))
		{
			state = MightaState::ATTACK;
			//The attack animation consists of 2 frames, with the second one being when
			//we throw the shot. Wait for a frame before initiating the attack.
			attack_delay = MIGHTA_ANIM_DELAY;

			if (look == Look::LEFT)	sprite->SetAnimation((int)MightaAnim::ATTACK_LEFT);
			else					sprite->SetAnimation((int)MightaAnim::ATTACK_RIGHT);
		}
		else
		{
			pos += pattern[current_step].speed;
			current_frames++;

			if (current_frames == pattern[current_step].frames)
			{
				current_step++;
				current_step %= pattern.size();
				current_frames = 0;

				anim_id = pattern[current_step].anim;
				sprite->SetAnimation(anim_id);
				UpdateLook(anim_id);
			}
		}
	}
	else if (state == MightaState::ATTACK)
	{
		if (!IsVisible(box))
		{
			state = MightaState::ROAMING;

			//Continue with the previous animation pattern before initiating the attack
			anim_id = pattern[current_step].anim;
			sprite->SetAnimation(anim_id);
		}
		else
		{
			attack_delay--;
			if (attack_delay == 0)
			{
				shoot = true;

				//The attack animation consists of 2 frames. Wait for a complete loop
				//before shooting again
				attack_delay = 2 * MIGHTA_ANIM_DELAY;
			}
		}
	}
	sprite->Update();

	return shoot;
}
void Mighta::UpdateLook(int anim_id)
{
	MightaAnim anim = (MightaAnim)anim_id;
	look = (anim == MightaAnim::IDLE_LEFT ||
		anim == MightaAnim::WALKING_LEFT ||
		anim == MightaAnim::ATTACK_LEFT) ? Look::LEFT : Look::RIGHT;
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