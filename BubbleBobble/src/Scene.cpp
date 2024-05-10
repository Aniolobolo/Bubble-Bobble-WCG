#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Sound sfxs[30];

Scene::Scene()
{
	isGameOver = false;
	pBubble = nullptr;
	player = nullptr;
	enemies = nullptr;
	shots = nullptr;
	//player2 = nullptr;
	level = nullptr;
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom
	eBubblingTime = 0;
	debug = DebugMode::OFF;

	actualLevel = 1;
	goal_score[0] = 1000;
	goal_score[1] = 3000;
	goal_score[2] = 5000;
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
	//if (player2 != nullptr)
	//{
	//	player2->Release();
	//	delete player2;
	//	player2 = nullptr;
	//}
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

	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
	if (shots != nullptr)
	{
		delete shots;
		shots = nullptr;
	}

	for (Entity* obj : objects)
	{
		delete obj;
	}
	

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

	////Create player
	//player2 = new Player2({ 0,0 }, eState::EIDLE, eLook::ERIGHT);
	//if (player2 == nullptr)
	//{
	//	LOG("Failed to allocate memory for Player2");
	//	return AppStatus::ERROR;
	//}
	////Initialise player
	//if (player2->Initialise() != AppStatus::OK)
	//{
	//	LOG("Failed to initialise Player2");
	//	return AppStatus::ERROR;
	//}

	//Create enemy manager
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create shot manager 
	shots = new ShotManager();
	if (shots == nullptr)
	{
		LOG("Failed to allocate memory for Shot Manager");
		return AppStatus::ERROR;
	}
	//Initialise shot manager
	if (shots->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Shot Manager");
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

	

	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	//player2->SetTileMap(level);
	shots->SetTileMap(level);
	enemies->SetShotManager(shots);
	

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
	AABB hitbox, area;
	Object *obj;
	
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
			1,   1,   50,   0,   0,   0,   0,   154,   0,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   53,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1, 53, 0, 0, 1, 1, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   154,	  0,   0,   0,   154, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   53,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1, 53, 0, 0, 1, 1, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   154,   0,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   53,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1, 53, 0, 0, 1, 1, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,  0,   0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  104,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,  1, 1, 1, 1, 1, 1, 1, 1


		};
		player->InitScore();
		player->InitLife();
		//player2->InitScore();
		//player2->InitLife();

	}
	else if (stage == 2)
	{
		map = new int[size] {
			22,   22,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 22, 22,
			2,   2,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,   59,	  0,   0,   0,  24, 24, 24, 24, 24, 24, 24, 2, 2,
			2,   2,   57,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58, 58,  58,  60,  0,  0,  0,  61, 58,  58, 58, 58,  58, 58, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   154,   0,   0,   0,   2,   59,	  0,   0,   2,   59, 0, 0, 154, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   56,	  0,   0,   2,   2,  2, 2, 2, 59, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   57,   58,   58,   58,   60,	  0,   0,   61,  58, 58, 58, 2, 56, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   59,	  0,   0,   2,   2,  2, 2, 2, 56, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   61,   58,   58,   58,   2,   56,	  0,   0,   2,   57, 58, 58, 58, 60, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   56,	  0,   0,   2,   2, 59, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,  0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   57,   60,	  0,   0,   61,   2, 56, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   59,	  0,   0,   2,   2, 56, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   61,   2,   56,	  0,   0,   2,   57, 60, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   56,	  0,   0,   2,   2, 59, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   0,   0,   0,   0,   154,   0,   0,   0,   0,   154,   0,   0,   2,   2,   57,   60,	  0,   0,   61,   2, 2, 59, 0, 0, 0, 2, 2,
			2,   2,   56,  0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   57,   60,   0,	  0,   0,   0,   61, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   0,   2,   2,   57,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,  60,   0,   0,	  0,   0,   0,   0, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   2,   57,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   57,   60,   0,   0,   0,   0,   154,   0,   0,   0,   0,   154,   0,   0,   0,   0,	  0,   0,   0,   2, 2, 56, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   56,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,	  2,   2,   2,   2, 57, 60, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   2,   56,   0,   0,   2,   2,   57,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,	  58,   58,   58,   58, 60, 0, 0, 0, 0, 2, 2,
			2,   2,   56,  0,   2,   56,   0,   0,   2,   57,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,  0,   2,   56,   0,   0,   2,   56,   0,   0,   0,   0,   0,   0, 0,  0,   0,  0,   0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,   0,   61,   60,   0,   0,   61,   60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   56,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 0, 0, 2, 2,
			2,   2,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 59, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
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
			8,   9,   62,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 154, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   7,   7,   7,   7,   65,   0,   0,   0,   0,   0,   7,   7,   65,   0,   0,   0,	  0,   0,   7,   7, 7, 7, 65, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   67,   64,   64,   64,   66,   0,   0,   0,   0,   0,   67,   64,   66,   0,   0,   0,	  0,   0,   67,   64, 64, 64, 66, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 154, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   7,   7,   7,   7,   65,   0,   0,   0,   0,   0,   7,   7,   7,   7,   65,   0,   0,	  0,   0,   0,   7, 7, 7, 7, 65, 0, 10, 11,
			8,   9,   62,   0,   67,   64,   64,   64,   66,   0,   0,   0,   0,   0,   67,   64,   64,   64,   66,   0,   0,	0,  0,   0,   67,  64, 64, 64, 66, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   154,   0,   0,   0,   0,   0,   0,   154,   0,   0,   154,   0,   0,   0,	  0,   0,   0,   154, 0, 0, 0, 0, 0, 8, 9,
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
			8,   9,   62,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 0, 0, 8, 9,
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
			//else if (tile == Tile::PLAYER2)
			//{
			//	ePos.x = x * TILE_SIZE;
			//	ePos.y = y * TILE_SIZE + TILE_SIZE - 1;
			//	player2->SetPos(ePos);
			//	map[i] = 0;
			//}
			else if (tile == Tile::MIGHTA)
			{
				pos.x += (MIGHTA_FRAME_SIZE - MIGHTA_PHYSICAL_WIDTH) / 2;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::MIGHTA);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::MIGHTA, area);
			}
			else if (tile == Tile::ITEM_CHERRY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ITEM_CHERRY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_BEER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ITEM_BEER);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_BANANA)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ITEM_BANANA);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_BORGER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ITEM_BORGER);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_WATERMELON)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ITEM_WATERMELON);
				objects.push_back(obj);
				map[i] = 0;
			}

			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;
	return AppStatus::OK;
}
void Scene::PlayerBubbleSpawn()
{
	eBubblingTime += GetFrameTime();

	if (IsKeyPressed(KEY_E) && eBubblingTime >= .3 && !isGameOver)
	{
		if (player->IsLookingLeft())
		{
			PlayerBubble* bubble = new PlayerBubble(player->GetPos(), Directions::LEFT);
			bubble->Initialise();
			sfxs[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");
			PlaySound(sfxs[0]);
			playerBubbles.push_back(bubble);
			PlayerBubble* pBubble = new PlayerBubble(player->GetPos(), Directions::LEFT);

		}
		else
		{
			PlayerBubble* bubble = new PlayerBubble(player->GetPos(), Directions::RIGHT);
			bubble->Initialise();
			sfxs[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");
			PlaySound(sfxs[0]);
			playerBubbles.push_back(bubble);
			PlayerBubble* pBubble = new PlayerBubble(player->GetPos(), Directions::LEFT);
		}
		eBubblingTime = 0;

	}


}
void Scene::deletePBubbles()
{
	auto check = playerBubbles.begin();
	int bubble = 0;
	while (check != playerBubbles.end() && bubble < playerBubbles.size()) {
		if (!playerBubbles[bubble]->isAlive()) {
			delete* check;
			check = playerBubbles.erase(check);
		}
		else {
			check++;
			bubble++;
		}
	}

}
void Scene::Update()
{
	Point p1, p2;
	AABB box;
	AABB hitbox;
	PlayerBubbleSpawn();
	deletePBubbles();
	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE)) {
		LoadLevel(1);
		player->InitLife();
		actualLevel = 1;
	}
	else if (IsKeyPressed(KEY_TWO)) {
		LoadLevel(2);
		player->InitLife();
		actualLevel = 2;
	}
	else if (IsKeyPressed(KEY_THREE)) {
		LoadLevel(3);
		player->InitLife();
		actualLevel = 3;
	}
	if (player->GetScore() >= goal_score[actualLevel - 1])
	{
		actualLevel++;
		
		if (actualLevel > 3) {
			isGameWon = true;
		}

		LoadLevel(actualLevel);
	}

	if (IsKeyPressed(KEY_F2)) {
		godMode = true;
	}
	else if (IsKeyPressed(KEY_F3)) {
		godMode = false;
	}

	for (PlayerBubble* bubble : playerBubbles)
	{
		bubble->SetPlayer(player);
	}

	UpdateBubbles();
	level->Update();
	player->Update();
	//player2->Update();
	hitbox = player->GetHitbox();
	enemies->Update(hitbox);
	shots->Update(hitbox);

	CheckObjectCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		enemies->Draw();
		player->Draw();
		shots->Draw();
		//player2->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		enemies->DrawDebug();
		player->DrawDebug(GREEN);
		shots->DrawDebug(GRAY);
		//player2->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	//player2->Release();
	ClearLevel();
}
void Scene::CheckObjectCollisions()
{
	AABB player_box, obj_box;

	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());

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
	enemies->Release();
	shots->Clear();
	

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
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}

}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("1UP", player->GetScore()), 32, 0, 8, GREEN);
	DrawText(TextFormat("%d", player->GetScore()), 32, 8, 8, WHITE);
	DrawText(TextFormat("Lives", player->GetScore()), 64, 0, 8, GREEN);
	DrawText(TextFormat("%d", player->getLife()), 64, 8, 10, WHITE);
	
	//DrawText(TextFormat("2UP", player2->GetScore()), 200, 0, 8, SKYBLUE);
	//DrawText(TextFormat("%d", player2->GetScore()), 200, 8, 8, WHITE);
}