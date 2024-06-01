#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player2.h"
#include "Enemy.h"
#include "Player2Bubble.h"

#define BUBBLE_PHYSICAL_SIZE	14
#define BUBBLE_FRAME_SIZE		16
#define BUBBLEJUMP				-20
#define SHOOT_RANGE 65

enum class Bubble2Directions { LEFT, RIGHT };
enum class Bubble2Anim { INSHOOT, IDLE, POP, GREENZENCHAN, YELLOWZENCHAN, REDZENCHAN, DEADZENCHAN, GREENINVADER, YELLOWINVADER, REDINVADER, DEADINVADER, GREENMIGHTA, YELLOWMIGHTA, REDMIGHTA, DEADMIGHTA, GREENDRUNK, YELLOWDRUNK, REDDRUNK, DEADDRUNK, NUM_ANIMATIONS };
enum class Bubble2State { JUSTSHOT, WANDER, ISONCEILING, POP, ZENCHANINSIDE, INVADERINSIDE, MIGHTAINSIDE, DRUNKINSIDE };
class Player2Bubble : public Entity
{
public:
	Player2Bubble(const Point& p, Bubble2Directions d);
	~Player2Bubble();

	void Update();

	void Move(Bubble2Directions d);
	void Clamp();
	AppStatus Initialise();
	bool isAlive();
	void Release();
	Bubble2Directions direction;
	int bubbleStage;
	void DrawDebug(const Color& col) const;
	void JumpOnBubble();
	void SetPlayer2(Player2* p);
	hType GetEnemyType();
	bool isJustShot();
	bool isEnemyInside = false;
	bool isInside();

	void ZenchanInside();
	void MightaInside();
	void InvaderInside();
	void DrunkInside();

	bool hasZenchan = false;
	bool hasMighta = false;
	bool hasInvader = false;
	bool hasDrunk = false;
	bool popped = false;
	bool isOnCeiling = false;
	float bubbleLifetime;
	Bubble2State state;

private:
	//void DrawDebug(const Color& col) const;
	void SetAnimation(int id);

	void Shot();
	void Wander();
	void Zenchan();
	void Mighta();
	void Invader();
	void Drunk();

	

	

	bool inShoot;
	int bubblePosLeft;
	int bubblePosRight;
	float spawnTime;
	
	float jumpTime;
	float timeAlive;
	float speed;
	Player2* player2;
};