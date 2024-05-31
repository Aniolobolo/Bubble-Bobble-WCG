#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Enemy.h"

#define SHOT_PHYSICAL_SIZE	16
#define SHOT_FRAME_SIZE		16

enum class mShotDirections { LEFT, RIGHT };
enum class mShotAnim { SHOT_L, SHOT_R, NUM_ANIMATIONS };
enum class mShotState { MOVING };

class MightaShot : public Entity
{
public:
	MightaShot(const Point& p, mShotDirections d);
	~MightaShot();

	void Update();

	void Move(mShotDirections d);
	AppStatus Initialise();
	bool isAlive();
	void Release();
	mShotDirections direction;
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
	float waitForAnim = 0;
	mShotState state;
	TileMap* map;


};
