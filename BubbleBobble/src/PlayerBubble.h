#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"
#include "PlayerBubble.h"

#define BUBBLE_PHYSICAL_SIZE	14
#define BUBBLE_FRAME_SIZE		16
#define BUBBLEJUMP				-20
#define SHOOT_RANGE 65

enum class Directions { LEFT, RIGHT };
enum class BubbleAnim { INSHOOT, IDLE, POP, GREENZENCHAN, YELLOWZENCHAN, REDZENCHAN, DEADZENCHAN, GREENINVADER, YELLOWINVADER, REDINVADER, DEADINVADER, GREENMIGHTA, YELLOWMIGHTA, REDMIGHTA, DEADMIGHTA, GREENDRUNK, YELLOWDRUNK, REDDRUNK, DEADDRUNK, NUM_ANIMATIONS };
enum class BubbleState { JUSTSHOT, WANDER, ISONCEILING, POP, ZENCHANINSIDE, INVADERINSIDE, MIGHTAINSIDE, DRUNKINSIDE};
enum class BubbleStages { GREENSTAGE, GREENSTAGE_, YELLOWSTAGE, YELLOWSTAGE_, REDSTAGE, REDSTAGE_, ENDED };
class PlayerBubble : public Entity
{
public:
	PlayerBubble(const Point& p, Directions d);
	~PlayerBubble();

	void Update();

	void Movement(Directions d);
	void MoveBubbleRightPlayer();
	void MoveBubbleLeftPlayer();
	void MoveBubbleRightPlayerRandom();
	void MoveBubbleLeftPlayerRandom();
	void ClampPos();
	AppStatus Initialise();
	bool isAlive();
	void Release();
	Directions dire;
	int stages;
	void DrawDebug(const Color& col) const;
	void Stomp();
	void SetTileMap(TileMap* m);
	void SetPlayer(Player* p);
	bool inCatch = false;
	bool hasEndedFromCatch = false;
	bool poped = false;
	bool fruit = false;
	float framecounter;
	bool canCollide;
	Point GetPos() const;
	void EnemyCatchSlime();
	void EnemyCatchDrunk();
	bool issAlive;
	void MoveBubbleToRandomNear();

	Directions BublePop(Directions d);

	bool inShoot;
	bool CollisionWall(const Point& p, int distance) const;
	int enemytype = 0;
private:
	void HandleCollisionLogic();




	//void DrawDebug(const Color& col) const;
	void SetAnimation(int id);
	int bubbleStages = (int)BubbleStages::GREENSTAGE;

	int logPosXL;
	int logPosXR;
	float spawnTime;
	float eTime;
	float eTimePogo;
	float eTimeCatch = 0;
	float eTimeCatchGreen = 1.5;
	float eTimeCatchYellow = 1;
	float eTimeCatchRed = 1;
	float lifeTime;
	float speed;
	Player* player;
	TileMap* map;
	Directions direction;
};