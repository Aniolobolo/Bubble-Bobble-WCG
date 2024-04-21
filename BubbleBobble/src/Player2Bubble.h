#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player2.h"
#include "Player2Bubble.h"

#define BUBBLE_PHYSICAL_SIZE	14
#define BUBBLE_FRAME_SIZE		16
#define BUBBLEJUMP				-20
#define SHOOT_RANGE 65

enum class Directions { LEFT, RIGHT };
enum class BubbleAnim { INSHOOT, IDLE, POP, ENEMYIN, NUM_ANIMATIONS };
enum class BubbleState { JUSTSHOT, WANDER, ISONCEILING, POP, ENEMYINSIDE };
class Player2Bubble : public Entity
{
public:
	Player2Bubble(const Point& p, Directions d);
	~Player2Bubble();

	void Update();

	void Movement(Directions d);
	void ClampPos();
	AppStatus Initialise();
	bool isAlive();
	void Release();
	Directions direc;
	int level;
	void DrawDebug(const Color& col) const;
	void Stomp();
	void SetPlayer(Player2* p);

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
	Player2* player2;
};