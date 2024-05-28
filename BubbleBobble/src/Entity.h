
#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "AABB.h"

#define BUBBLE_SPEED    4
#define MAX_SHOTS       5
#define SHOOT_POS_X      18
#define SHOOT_POS_Y      26

class Entity
{
public:
	Entity() { isAlive = false; }
	Entity(const Point& p, int width, int height);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Entity();
	AppStatus Initialise();
	void SetPos(const Point& p);
	void Warp();
	void Update();
	void SetAlive(bool b);
	bool IsAlive() const;
	AABB GetHitbox() const;
	Point GetPos();
	void Set(const Point& p, const Point& d, int w, int h, int framew, int frameh);
	void Init(Vector2 p, int w, int h, int s);
	//Draw representation model
	void Draw() const;
	void DrawTint(const Color& col) const;
	void Release();
	//Draw logical/physical model (hitbox)
	void DrawHitbox(const Color& col) const;
	void DrawHitbox(int x, int y, int w, int h, const Color& col) const;

protected:
	Point GetRenderingPosition() const;
	bool isAlive;
	//Logical/physical model
	Point pos, dir;
	int width, height;				

	//Representation model
	int frame_width, frame_height;

	RenderComponent *render;
};