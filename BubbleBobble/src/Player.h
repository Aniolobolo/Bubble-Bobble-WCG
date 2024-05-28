#pragma once
#include "Entity.h"
#include "TileMap.h"


//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

#define PLAYER_FALLING_SPEED	1
#define PLAYER_ENDJUMPING_SPEED	2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, CLIMBING, DAMAGED, SHOOTING, DEAD };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class PlayerAnim {
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

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitLife();
	void LifeManager();
	int life;
	int getLife();
	bool godMode;
	void Die();
	void ReceiveDamage();
	void isReceivingDamage();
	bool wasHit = false;
	bool isDead = false;
	bool gameOver = false;
	void SetDeathAnim();
	bool Ikilleable = true;
	
	
	void InitScore();
	void IncrScore(int n);
	int GetScore();

	bool IsLookingRight() const;
	bool IsLookingLeft() const;


	bool BubbleIsBeingCreated = false;
	bool IsJumpingOnBubble(const Point& p, int distance);
	bool canJump;
	void SetDir(Point p);
	bool TestUpCol(const AABB& box, int* py);
	void SetState(State state);
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();


private:
	//Entity* Shots[MAX_SHOTS];
	//int idx_shot;

	bool hasTakenDamage;
	
	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicClimbing();
	float eTimeHitted = 0;
	float eTimeDead = 0;
	//void ShootBubble();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	

	
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();
	void StartShooting();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	
	
	
	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;
	int jump_delay;
	int immuneThreshold = 2;

	TileMap *map;

	int score;
	
	float damageTime;
	float pTime;
};

