#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	//laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	//if (laser != nullptr)
	//{
	//	laser->Release();
	//	delete laser;
	//	laser = nullptr;
	//}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCK_L1] = { 0,  0, n, n };
	dict_rect[(int)Tile::BLOCK_L2] = { 4 * n,  0, n, n };
	dict_rect[(int)Tile::BLOCK_L3] = { 8 * n,  0, n, n };
	dict_rect[(int)Tile::BLOCK_L4] = { 12 * n,  0, n, n };
	dict_rect[(int)Tile::BLOCK_L5] = { 16 * n,  0, n, n };

	dict_rect[(int)Tile::DECOR_L2_1] = { 6 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L2_2] = { 7 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L2_3] = { 6 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L2_4] = { 7 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L3_1] = { 10 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L3_2] = { 11 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L3_3] = { 10 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L3_4] = { 11 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L4_1] = { 14 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L4_2] = { 15 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L4_3] = { 14 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L4_4] = { 15 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L4_1] = { 14 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L4_2] = { 15 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L4_3] = { 14 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L4_4] = { 15 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L5_1] = { 18 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L5_2] = { 19 * n,  0, n, n };
	dict_rect[(int)Tile::DECOR_L5_3] = { 18 * n,  n, n, n };
	dict_rect[(int)Tile::DECOR_L5_4] = { 19* n,  n, n, n };

	dict_rect[(int)Tile::NOTHING_SOLID] = { 0,  n, n, n };

	dict_rect[(int)Tile::NOTSOLID_L1] = { 0,  0, n, n };
	dict_rect[(int)Tile::NOTSOLID_L2] = { 4 * n,  0, n, n };
	dict_rect[(int)Tile::NOTSOLID_L3] = { 8 * n,  0, n, n };
	dict_rect[(int)Tile::NOTSOLID_L4] = { 12 * n,  0, n, n };
	dict_rect[(int)Tile::NOTSOLID_L5] = { 16 * n,  0, n, n };

	dict_rect[(int)Tile::SHADOW_WALL1] = { 0,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_WALLPLAT1] = { 0,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLAT1] = { n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGE1] = { 2 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGEDOWN1] = { 2 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLATLEFT1] = { 3 * n,  5 * n, n, n };

	dict_rect[(int)Tile::SHADOW_WALL2] = { 4 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_WALLPLAT2] = { 4 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLAT2] = { 5 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGE2] = { 6 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGEDOWN2] = { 6 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLATLEFT2] = { 7 * n,  5 * n, n, n };

	dict_rect[(int)Tile::SHADOW_WALL3] = { 8 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_WALLPLAT3] = { 8 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLAT3] = { 9 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGE3] = { 10 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGEDOWN3] = { 10 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLATLEFT3] = { 11 * n,  5 * n, n, n };

	dict_rect[(int)Tile::SHADOW_WALL4] = { 12 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_WALLPLAT4] = { 12 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLAT4] = { 13 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGE4] = { 14 * n,  4 * n, n, n };
	dict_rect[(int)Tile::SHADOW_EDGEDOWN4] = { 14 * n,  5 * n, n, n };
	dict_rect[(int)Tile::SHADOW_PLATLEFT4] = { 15 * n,  5 * n, n, n };



	//dict_rect[(int)Tile::LADDER_L] = { 2 * n, 2 * n, n, n };
	//dict_rect[(int)Tile::LADDER_R] = { 3 * n, 2 * n, n, n };
	//dict_rect[(int)Tile::LADDER_TOP_L] = { 4 * n, 2 * n, n, n };
	//dict_rect[(int)Tile::LADDER_TOP_R] = { 5 * n, 2 * n, n, n };

	//dict_rect[(int)Tile::LOCK_RED] = { 6 * n, 2 * n, n, n };
	//dict_rect[(int)Tile::LOCK_YELLOW] = { 7 * n, 2 * n, n, n };

	//dict_rect[(int)Tile::LASER_L] = { 0, 6 * n, n, n };
	//dict_rect[(int)Tile::LASER_R] = { 4 * n, 6 * n, n, n };
	//
	//dict_rect[(int)Tile::LASER_FRAME0] = { 1 * n, 6 * n, n, n };
	//dict_rect[(int)Tile::LASER_FRAME1] = { 2 * n, 6 * n, n, n };
	//dict_rect[(int)Tile::LASER_FRAME2] = { 3 * n, 6 * n, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tileset.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	if (data.LoadTexture(Resource::IMG_ITEMS, "images/items.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_items = data.GetTexture(Resource::IMG_ITEMS);

	//laser = new Sprite(img_tiles);
	//if (laser == nullptr)
	//{
	//	LOG("Failed to allocate memory for laser sprite");
	//	return AppStatus::ERROR;
	//}
	//laser->SetNumberAnimations(1);
	//laser->SetAnimationDelay(0, ANIM_DELAY);
	//laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	//laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	//laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	//laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	/*laser->Update();*/
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
//bool TileMap::IsTileLadderTop(Tile tile) const
//{
//	return tile == Tile::LADDER_TOP_L || tile == Tile::LADDER_TOP_R;
//}
//bool TileMap::IsTileLadder(Tile tile) const
//{
//	return tile == Tile::LADDER_L || tile == Tile::LADDER_R;
//}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile)/* || IsTileLadderTop(tile)*/)
			return true;
	}
	return false;
}
//bool TileMap::TestOnLadder(const AABB& box, int* px) const
//{
//	int left, right, bottom;
//	int tx1, tx2, ty;
//	Tile tile1, tile2;
//	
//	//Control points
//	left = box.pos.x;
//	right = box.pos.x + box.width-1;
//	bottom = box.pos.y + box.height-1;
//
//	//Calculate the tile coordinates
//	tx1 = left / TILE_SIZE;
//	tx2 = right / TILE_SIZE;
//	ty = bottom / TILE_SIZE;
//
//	//To be able to climb up or down, both control points must be on ladder
//	tile1 = GetTileIndex(tx1, ty);
//	tile2 = GetTileIndex(tx2, ty);
//	if (IsTileLadder(tile1) && IsTileLadder(tile2))
//	{
//		*px = GetLadderCenterPos(left, bottom) - box.width/2;
//		return true;
//	}
//	return false;
//}
//bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
//{
//	int left, right, bottom;
//	int tx1, tx2, ty;
//	Tile tile1, tile2;
//
//	//Control points
//	left = box.pos.x;
//	right = box.pos.x + box.width - 1;
//	bottom = box.pos.y + box.height - 1;
//
//	//Calculate the tile coordinates
//	tx1 = left / TILE_SIZE;
//	tx2 = right / TILE_SIZE;
//	ty = bottom / TILE_SIZE;
//
//	//To be able to climb up or down, both control points must be on ladder
//	tile1 = GetTileIndex(tx1, ty);
//	tile2 = GetTileIndex(tx2, ty);
//	if (IsTileLadderTop(tile1) && IsTileLadderTop(tile2))
//	{
//		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
//		return true;
//	}
//	return false;
//}
//int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
//{
//	int tx, ty;
//	
//	tx = pixel_x / TILE_SIZE;
//	ty = pixel_y / TILE_SIZE;
//	Tile tile = GetTileIndex(tx, ty);
//
//	if (tile == Tile::LADDER_L || tile == Tile::LADDER_TOP_L)		return tx * TILE_SIZE + TILE_SIZE;
//	else if (tile == Tile::LADDER_R || tile == Tile::LADDER_TOP_R)	return tx * TILE_SIZE;
//	else
//	{
//		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
//		return 0;
//	}
//}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::EMPTY)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	/*laser->Release();*/

	dict_rect.clear();
}