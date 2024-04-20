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
	PLAYER = 100, PLAYER2, ZENCHAN, INVADER, BUBBLE,
	//id >= 150 items
	ITEM_PEPPER = 150, ITEM_EGGPLANT, ITEM_CARROT, ITEM_ONION, ITEM_BEET, ITEM_WHITEBEET, ITEM_WHITECARROT, ITEM_PICKLE, ITEM_PEAS, ITEM_MUSHROOM, ITEM_CORN, ITEM_FRIEDEGG, ITEM_ACORN, ITEM_PERSIMMON, ITEM_LEMON,
	ITEM_ORANGE, ITEM_PEACH, ITEM_BANANA, ITEM_PEAR, ITEM_WATERMELON, ITEM_REDICECREAM, ITEM_LIME_ICECREAM, ITEM_GREENICECREAM, ITEM_DARKGREENICECREAM, ITEM_DONNUT, ITEM_MCFRIES, ITEM_FLAN, ITEM_BORGER, ITEM_CAKE,
	ITEM_CUPCAKE, ITEM_KFCCHICKEN, ITEM_GREENSUSHI, ITEM_SHRIMPSUSHI, ITEM_TUNASUSHI, ITEM_BROWNSUSHI, ITEM_BEER, ITEM_CHOCOLATEPOPSICLE, ITEM_BLUEPOPSICLE, ITEM_ICECREAM, ITEM_DOUBLEICECREAM, ITEM_HOTDOG, ITEM_COCKTAIL,
	ITEM_BUTTERFLY, ITEM_CRABSHELL, ITEM_RAMEN, ITEM_CURRYRICE, ITEM_PINKJEWEL, ITEM_YELLOWJEWEL, ITEM_REDJEWEL, ITEM_BLUEJEWEL, ITEM_PURPLEJEWEL, ITEM_PINKORE, ITEM_BLUEORE, ITEM_PURPLEDIAMOND, ITEM_BLUEDIAMOND, ITEM_REDDIAMOND,
	ITEM_PURPLECOLLAR, ITEM_BLUECOLLAR, ITEM_REDCOLLAR, ITEM_BRONZECROWN, ITEM_SILVERCROWN, ITEM_GOLDCROWN, ITEM_COOLERBRONZECROWN, ITEM_COOLERSILVERCROWN, ITEM_COOLERGOLDCROWN, ITEM_CHERRY, ITEM_STRAWBERRY, ITEM_TOMATO, ITEM_APPLE,
	ITEM_GRAPES, ITEM_PINEAPPLE, ITEM_PINKCANDY, ITEM_BLUECANDY, ITEM_YELLOWCANDY, ITEM_JORDANS, ITEM_CLOCK, ITEM_BOMB, ITEM_ORANGEUMBRELLA, ITEM_REDUMBRELLA, ITEM_PINKUMBRELLA, ITEM_ORANGEPOTION, ITEM_REDPOTION, ITEM_GREENPOTION,
	ITEM_YELLOWPOTION, ITEM_PINKPOTION, ITEM_BLUERING, ITEM_PINKRING, ITEM_REDRING, ITEM_CROSS, ITEM_LIGHTNINGPOWER, ITEM_E, ITEM_X, ITEM_T, ITEM_E2, ITEM_N, ITEM_D, ITEM_THUNDERBOBBLE, ITEM_WATERBOBBLE, ITEM_FIREBOBBLE, ITEM_REDXBOBBLE,



	//Intervals
	STATIC_FIRST = BLOCK_L1,
	STATIC_LAST = DECOR_L5_4,
	SOLID_FIRST = BLOCK_L1,
	SOLID_LAST = NOTHING_SOLID,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = BUBBLE
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
	const Texture2D *img_items;
};

