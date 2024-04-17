#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles
	BLOCK_L1 = 1, 
	BLOCK_L2, DECOR_L2_1, DECOR_L2_2, DECOR_L2_3, DECOR_L2_4, 
	BLOCK_L3, DECOR_L3_1, DECOR_L3_2, DECOR_L3_3, DECOR_L3_4, 
	BLOCK_L4, DECOR_L4_1, DECOR_L4_2, DECOR_L4_3, DECOR_L4_4,
	BLOCK_L5, DECOR_L5_1, DECOR_L5_2, DECOR_L5_3, DECOR_L5_4, NOTHING_SOLID,
	NOTSOLID_L1 = 23, NOTSOLID_L2, NOTSOLID_L3, NOTSOLID_L4, NOTSOLID_L5,
	
	// 50 <= id < 100: shadow tiles
	SHADOW_WALL1 = 50, SHADOW_WALLPLAT1, SHADOW_PLAT1, SHADOW_EDGE1, SHADOW_EDGEDOWN1, SHADOW_PLATLEFT1,
	SHADOW_WALL2, SHADOW_WALLPLAT2, SHADOW_PLAT2, SHADOW_EDGE2, SHADOW_EDGEDOWN2, SHADOW_PLATLEFT2,
	SHADOW_WALL3, SHADOW_WALLPLAT3, SHADOW_PLAT3, SHADOW_EDGE3, SHADOW_EDGEDOWN3, SHADOW_PLATLEFT3,
	SHADOW_WALL4, SHADOW_WALLPLAT4, SHADOW_PLAT4, SHADOW_EDGE4, SHADOW_EDGEDOWN4, SHADOW_PLATLEFT4,

	// id >= 100: entities' initial locations
	PLAYER = 100,

	//Intervals
	STATIC_FIRST = BLOCK_L1,
	STATIC_LAST = DECOR_L5_4,
	SOLID_FIRST = BLOCK_L1,
	SOLID_LAST = NOTHING_SOLID,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	
	//Tile sheet
	const Texture2D *img_tiles;
};

