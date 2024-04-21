#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER2_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER2_PHYSICAL_WIDTH	12
#define PLAYER2_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define PLAYER2_SPEED			    1

//When jumping, initial jump speed and maximum falling speed
#define PLAYER2_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define PLAYER2_JUMP_DELAY		2

#define PLAYER2_FALLING_SPEED	1
#define PLAYER2_ENDJUMPING_SPEED	2

//Player is levitating when abs(speed) <= this value
#define PLAYER2_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class eState { EIDLE, EWALKING, EJUMPING, EFALLING, ECLIMBING, EDAMAGED, ESHOOTING, EDEAD };
enum class eLook { ERIGHT, ELEFT };

//Rendering states
enum class Player2Anim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	DAMAGE_LEFT, DAMAGE_RIGHT,
	SHOOTING,
	DIE_LEFT, DIE_RIGHT,
	NUM_ANIMATIONS
};

class Player2 : public Entity
{
public:
	Player2(const Point& p, eState s, eLook view);
	~Player2();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void InitLife();
	void LifeManager();
	int life;
	int getLife();
	void Die();
	void ReceiveDamage();
	bool isReceivingDamage();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:

	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	//void ShootBubble();

	//Animation management
	void SetAnimation(int id);
	Player2Anim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	eState state;
	eLook look;
	int jump_delay;

	TileMap* map;

	int score;
	float damageTime;
	float pTime;
};

