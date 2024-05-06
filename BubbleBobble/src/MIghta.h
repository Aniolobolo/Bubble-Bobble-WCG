#pragma once
#include "Enemy.h"

#define MIGHTA_SPEED			1
#define MIGHTA_ANIM_DELAY	(4*ANIM_DELAY)

#define MIGHTA_SHOT_OFFSET_X_LEFT	-14
#define MIGHTA_SHOT_OFFSET_X_RIGHT	 26
#define MIGHTA_SHOT_OFFSET_Y			-22

#define MIGHTA_SHOT_SPEED	4

enum class MightaState { ROAMING, ATTACK };
enum class MightaAnim {
	IDLE_LEFT, IDLE_RIGHT, WALKING_LEFT, WALKING_RIGHT,
	ATTACK_LEFT, ATTACK_RIGHT, NUM_ANIMATIONS
};

struct Step
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Mighta : public Enemy
{
public:
	Mighta(const Point& p, int width, int height, int frame_width, int frame_height);
	~Mighta();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

	//Retrieve the position and direction of the shot to be thrown
	void GetShootingPosDir(Point* pos, Point* dir) const override;

private:
	//Create the pattern behaviour
	void InitPattern();

	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	int attack_delay;	//delay between attacks
	MightaState state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<Step> pattern;
};

