#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define ENEMY_FRAME_SIZE		32

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	12
#define ENEMY_PHYSICAL_HEIGHT	12

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED			    1

//When jumping, initial jump speed and maximum falling speed
#define ENEMY_JUMP_FORCE		5

//Frame delay for updating the jump velocity
#define ENEMY_JUMP_DELAY		2

#define ENEMY_FALLING_SPEED	1
#define ENEMY_ENDJUMPING_SPEED	1

//Player is levitating when abs(speed) <= this value
#define ENEMY_LEVITATING_SPEED	1

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class hState { EIDLE, EWALKING, EJUMPING, EFALLING, ECLIMBING, EDEAD };
enum class hLook { ERIGHT, ELEFT };
enum class hType { ZENCHAN, INVADER};
//Rendering states
enum class EnemyAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, hState s, hLook view, hType t);
	~Enemy();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

protected:
	bool hasStartedWalking;
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	//void ShootBubble();

	//Animation management
	void SetAnimation(int id);
	EnemyAnim GetAnimation();
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

	hState state;
	hLook look;
	hType type;
	int direction;

	TileMap* map;

	int score;
};

