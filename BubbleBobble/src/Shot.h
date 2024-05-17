#pragma once
#include "Entity.h"

#define SHOT_PHYSICAL_WIDTH		16
#define SHOT_PHYSICAL_HEIGHT	16
#define SHOT_FRAME_WIDTH		16
#define SHOT_FRAME_HEIGHT		16

class Shot : public Entity
{
public:
	Shot();
	~Shot();

	void Init(const Point& p, const Point& dir);
	bool IsMovingLeft() const;
	bool IsMovingRight() const;
	void DrawDebug(const Color& col) const;
};

