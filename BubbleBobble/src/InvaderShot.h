#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Enemy.h"

#define SHOT_PHYSICAL_SIZE	8
#define SHOT_FRAME_SIZE		16
#define SHOT_SPEED			2

enum class iShotAnim { SHOT_L, SHOT_R, NUM_ANIMATIONS };
enum class iShotState { MOVING };

class InvaderShot : public Entity
{
public:
	InvaderShot(const Point& p);
	~InvaderShot();

	void Update();

	void Move();
	AppStatus Initialise();
	bool isAlive();
	void Release();
	void DrawDebug(const Color& col) const;
	void SetEnemy(Enemy* e);
	bool isJustShot();

	bool deleteShot = false;

private:
	//void DrawDebug(const Color& col) const;
	void SetAnimation(int id);

	void Shot();
	float speed;
	float lifetime;
	float timeAlive;
	iShotState state;
	TileMap* map;


};
