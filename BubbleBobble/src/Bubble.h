#pragma once
#include "Entity.h"
#include "TileMap.h"

#define BUBBLE_PHYSICAL_SIZE	14
#define BUBBLE_FRAME_SIZE		16

enum class Direction { LEFT, RIGHT };

class Bubble : public Entity
{
public:
	Bubble(const Point& p, Direction d);
	~Bubble();

	void Update();
	Direction direc;
	void LeftMovement(Direction d);
	bool isAlive();
	void Release();
	
	void DrawDebug(const Color& col) const;
	
private:
	float spawnTime;
	float eTime;
	float lifeTime;
	float speed;
	int stages;
};