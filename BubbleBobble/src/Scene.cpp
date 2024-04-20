#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
	player2 = nullptr;
	level = nullptr;
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom
	eBubblingTime = 0;
	eTimeSpawnX = GetRandomValue(-1, 1);
	eTimeSpawnY = GetRandomValue(-1, 1);
	debug = DebugMode::OFF;

	actualLevel = 1;
	goal_score[0] = 100;
	goal_score[1] = 150;
	goal_score[2] = 200;
}
Scene::~Scene()
{
	// Release and delete player objects
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (player2 != nullptr)
	{
		player2->Release();
		delete player2;
		player2 = nullptr;
	}
	if (pBubble != nullptr)
	{
		pBubble->Release();
		delete pBubble;
		pBubble = nullptr;
	}

	// Release and delete level
	if (level != nullptr)
	{
		level->Release();
		delete level;
		level = nullptr;
	}

	for (Entity* enemy : enemies)
	{
		delete enemy;
	}

	for (Entity* obj : objects)
	{
		delete obj;
	}
	enemies.clear();

	for (Entity* bubbles : playerBubbles)
	{
		delete bubbles;
	}
	playerBubbles.clear();

	// Delete objects from the vector efficiently
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		delete* it; // Dereference the iterator to get the object and delete it
	}
	objects.clear(); // Clear the vector after deleting elements


}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create player
	player2 = new Player2({ 0,0 }, eState::EIDLE, eLook::ERIGHT);
	if (player2 == nullptr)
	{
		LOG("Failed to allocate memory for Player2");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player2->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player2");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	for (Enemy* enemy : enemies)
	{
		if (enemy != nullptr) {
			enemy->SetTileMap(level);

		}
	}

	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	player2->SetTileMap(level);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	Point ePos;
	int *map = nullptr;
	Object *obj;
	Enemy *e;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			22,   22,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 22, 22,
			1,   1,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,   23,	23,  23,  23,  23, 23, 23, 23, 23, 23, 1, 1,
			1,   1,   51,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 52, 52, 52, 52, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   102,   0,   0,   0,   0,   0,   0,   0,   102,   0,   0,   0,   0,   0,	  0,   0,   102,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   53,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1, 53, 0, 0, 1, 1, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   53,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1, 53, 0, 0, 1, 1, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   53,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1, 53, 0, 0, 1, 1, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,  0,   0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 101, 0, 1, 1,
			1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,  1, 1, 1, 1, 1, 1, 1, 1


		};
		player->InitScore();
		player->InitLife();

		player2->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			22,   22,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 22, 22,
			2,   2,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   59,	  0,   0,   0,  24, 24, 24, 24, 24, 24, 24, 2, 2,
			2,   2,   57,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58, 58,  58,  60,  0,  0,  0,  61, 58,  58, 58, 58,  58, 58, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   59,	  0,   0,   2,   59, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   56,	  0,   0,   2,   2,  2, 2, 2, 59, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   57,   58,   58,   58,   60,	  0,   0,   61,  58, 58, 58, 2, 56, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   59,	  0,   0,   2,   2,  2, 2, 2, 56, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   61,   58,   58,   58,   2,   56,	  0,   0,   2,   57, 58, 58, 58, 60, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   56,	  0,   0,   2,   2, 59, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,  0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   57,   60,	  0,   0,   61,   2, 56, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   59,	  0,   0,   2,   2, 56, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   61,   2,   56,	  0,   0,   2,   57, 60, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   56,	  0,   0,   2,   2, 59, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   102,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   57,   60,	  0,   0,   61,   2, 2, 59, 0, 0, 0, 2, 2,
			2,   2,   56,  0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   57,   60,   0,	  0,   0,   0,   61, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   2,   2,   57,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,  60,   0,   0,	  0,   0,   0,   0, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   2,   57,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   57,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   2, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   56,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,	  2,   2,   2,   2, 57, 60, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   56,   0,   0,   2,   2,   57,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,	  58,   58,   58,   58, 60, 0, 0, 0, 0, 2, 2,
			2,   2,   56,  0,   2,   56,   0,   0,   2,   57,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,  0,   2,   56,   0,   0,   2,   56,   0,   0,   0,   0,   0,   0, 0,  0,   0,  0,   0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   61,   60,   0,   0,   61,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 101, 0, 2, 2,
			2,   2,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 59, 103, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		};
	}
	else if (stage == 3)
	{
		map = new int[size] {
			22,   22,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 22, 22,
			8,   9,   25,   25,   25,   25,   25,   25,   25,   62,   0,   0,   0,   25,   25,   25,   25,   25,   25,   62,	  0,   0,   0,  25, 25, 25, 25, 25, 25, 25, 8, 9,
			10,   11,   63,  64,  64,  64,  64,  64,  64,  66,  0,  0,  0,  67,  64,  64,  64, 64,  64,  66,  0,  0,  0,  67, 64,  64, 64, 64,  64, 64, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   7,   7,   7,   7,   65,   0,   0,   0,   0,   0,   7,   7,   65,   0,   0,   0,	  0,   0,   7,   7, 7, 7, 65, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   67,   64,   64,   64,   66,   0,   0,   0,   0,   0,   67,   64,   66,   0,   0,   0,	  0,   0,   67,   64, 64, 64, 66, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   7,   7,   7,   7,   65,   0,   0,   0,   0,   0,   7,   7,   7,   7,   65,   0,   0,	  0,   0,   0,   7, 7, 7, 7, 65, 0, 10, 11,
			8,   9,   62,   0,   67,   64,   64,   64,   66,   0,   0,   0,   0,   0,   67,   64,   64,   64,   66,   0,   0,	0,  0,   0,   67,  64, 64, 64, 66, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   7,   7,   7,   7,   65,   0,   0,   0,   7,   7,   7,   7,   7,   7,   65,   0,	  0,   0,   7,   7, 7, 7, 65, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   67,   64,   64,   64,   66,   0,   0,   0,   67,   64,   64,   64,   64,   64,   66,   0,	  0,   0,   67,   64, 64, 64, 66, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   7,   7,   7,   7,   65,   0,   7,   7,   7,   65,   0,   7,   7,   7,   65,	  0,   7,   7,   7, 7, 65, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   67,   64,   64,   64,   66,   0,   67,   64,   64,   66,   0,   67,   64,   64,   66,	  0,   67,   64,   64, 64, 66, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   7,   7,   7,   7,   7,   7,   65,   0,   0,   0,   7,   7,   7,	  7,   7,   7,   65, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   67,   64,   64,   64,   64,   64,   66,   0,   0,   0,   67,   64,   64,	64,   64,   64,   66, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 101, 0, 8, 9,
			10,   11,   7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 11,
		};
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER2)
			{
				ePos.x = x * TILE_SIZE;
				ePos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player2->SetPos(ePos);
				map[i] = 0;
			}
			else if (tile == Tile::ZENCHAN)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				e = new Enemy(pos, hState::EIDLE, hLook::ELEFT, hType::ZENCHAN);
				e->Initialise();
				e->SetTileMap(level);
				enemies.push_back(e);

				map[i] = 0;
			}
			/*else if (tile == Tile::ITEM_APPLE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::APPLE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CHILI)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHILI);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_WOOPER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::WOOPER);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_GULPIN)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::GULPIN);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_BAGON)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::BAGON);
				objects.push_back(obj);
				map[i] = 0;
			}*/

			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete map;
	return AppStatus::OK;
}
void Scene::PlayerBubbleSpawn()
{
	eBubblingTime += GetFrameTime();

	if (IsKeyPressed(KEY_E) && eBubblingTime >= .3)
	{
		if (player->IsLookingLeft())
		{
			PlayerBubble* bubble = new PlayerBubble(player->GetPos(), Directions::LEFT);
			bubble->Initialise();
			playerBubbles.push_back(bubble);
			PlayerBubble* pBubble = new PlayerBubble(player->GetPos(), Directions::LEFT);

		}
		else
		{
			PlayerBubble* bubble = new PlayerBubble(player->GetPos(), Directions::RIGHT);
			bubble->Initialise();
			playerBubbles.push_back(bubble);
			PlayerBubble* pBubble = new PlayerBubble(player->GetPos(), Directions::LEFT);
		}
		eBubblingTime = 0;

	}


}
void Scene::Update()
{
	Point p1, p2;
	AABB box;
	PlayerBubbleSpawn();
	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE)) {
		LoadLevel(1); 
		actualLevel = 1;
	}
	else if (IsKeyPressed(KEY_TWO)) {
		LoadLevel(2);
		actualLevel = 2;
	}
	else if (IsKeyPressed(KEY_THREE)) {
		LoadLevel(3);
		actualLevel = 3;
	}
	if (player->GetScore() >= goal_score[actualLevel - 1])
	{
		actualLevel++;
		
		if (actualLevel > 3) actualLevel = 1;
		player->InitScore();
		LoadLevel(actualLevel);
	}
	for (Enemy* e : enemies)
	{
		e->Update();
	}
	for (PlayerBubble* bubble : playerBubbles)
	{
		bubble->SetPlayer(player);
	}

	UpdateBubbles();
	level->Update();
	player->Update();
	player2->Update();


	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
		player2->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		player2->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	player2->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, player2_box, enemy_box, bubb_box, obj_box;
	
	player_box = player->GetHitbox();
	player2_box = player2->GetHitbox();
	
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else if (player2_box.TestAABB(obj_box))
		{
			player2->IncrScore((*it)->Points());

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
	//auto it_enemies = enemies.begin();
	//while (it_enemies != enemies.end())
	//{
	//	enemy_box = (*it_enemies)->GetHitbox();
	//	if (player_box.TestAABB(enemy_box))
	//	{
	//		/*player->LifeManager();*/
	//	}
	//	else
	//	{
	//		++it_enemies;
	//	}
	//}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	for (PlayerBubble* buble : playerBubbles)
	{
		delete buble;
	}
	playerBubbles.clear();
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

void Scene::UpdateBubbles()
{
	for (PlayerBubble* bubble : playerBubbles)
	{
		bubble->Update();
	}
}

void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
	auto it = playerBubbles.begin();
	while (it != playerBubbles.end())
	{

		(*it)->Draw();
		++it;
	}
	for (Enemy* enemy : enemies)
	{
		enemy->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
	for (Enemy* enemi : enemies)
	{
		enemi->DrawDebug(col);
	}

}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("1UP", player->GetScore()), 32, 0, 8, GREEN);
	DrawText(TextFormat("%d", player->GetScore()), 32, 8, 8, WHITE);

	DrawText(TextFormat("2UP", player2->GetScore()), 200, 0, 8, SKYBLUE);
	DrawText(TextFormat("%d", player2->GetScore()), 200, 8, 8, WHITE);
}