#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Enemy.h"

#define SHOT_PHYSICAL_SIZE	16
#define SHOT_FRAME_SIZE		16

enum class ShotDirections { LEFT, RIGHT };
enum class ShotAnim { SHOT_L, SHOT_R, NUM_ANIMATIONS };
enum class ShotState { MOVING };

class DrunkShot : public Entity
{
public:
	DrunkShot(const Point& p, ShotDirections d);
	~DrunkShot();

	void Update();

	void Move(ShotDirections d);
	AppStatus Initialise();
	bool isAlive();
	void Release();
	ShotDirections direction;
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
	ShotState state;
	TileMap* map;

	
};
