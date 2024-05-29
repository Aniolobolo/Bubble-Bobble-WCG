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
class PlayerBubble : public Entity
{
public:
	PlayerBubble(const Point& p, Directions d);
	~PlayerBubble();

	void Update();

	void Move(Directions d);
	void Clamp();
	AppStatus Initialise();
	bool isAlive();
	void Release();
	Directions direc;
	int level;
	void DrawDebug(const Color& col) const;
	void JumpOnBubble();
	void SetPlayer(Player* p);
	bool isJustShot();

private:
	//void DrawDebug(const Color& col) const;
	void SetAnimation(int id);

	void Shot();
	void Wander();
	void Ceiling();

	BubbleState state;

	bool inShoot;
	int logPosXL;
	int logPosXR;
	float spawnTime;
	float bTime;
	float jumpTime;
	float timeAlive;
	float speed;
	Player* player;
};