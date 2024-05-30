#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Sound sfxs[30];

Scene::Scene()
{
	isGameOver = false;
	pBubble = nullptr;
	player = nullptr;
	player2 = nullptr;
	level = nullptr;
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom
	bubbleCooldown = 0;
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
	player2 = new Player2({ 0,0 }, eState::IDLE, eLook::LEFT);
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

	font1 = new Text();
	if (font1 == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::ERROR;
	}
	//Initialise text font 1
	if (font1->Initialise(Resource::IMG_FONT, "images/font8x8.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

	//Assign the tile map reference to the player to check collisions while navigating
	player2->SetTileMap(level);
	player->SetTileMap(level);
	

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
			1,   1,   50,   0,   0,   0,   0,   150,   0,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   102,   0,   0,   0,   0,   0,   0,   0,   105,   0,   0,   0,   0,   0,	  0,   0,   104,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   80,   80,   53,   0,   0,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,	  80,   80,   80,   80, 53, 0, 0, 80, 80, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   151,	  0,   0,   0,   153, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   80,   80,   53,   0,   0,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,	  80,   80,   80,   80, 53, 0, 0, 80, 80, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   152,   0,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   80,   80,   53,   0,   0,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,	  80,   80,   80,   80, 53, 0, 0, 80, 80, 1, 1,
			1,   1,   51,  52,  54,   0,   0,   55,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,  52, 54, 0, 0, 55, 52, 1, 1,
			1,   1,   50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,  0,   0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 1, 1,
			1,   1,   50,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 101, 0, 1, 1,
			1, 1, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,	80, 80, 80, 80, 80, 80, 80, 80, 80, 1, 1


		};
		player->InitScore();
		player->InitLife();
		player2->InitScore();
		player2->InitLife();

	}
	else if (stage == 2)
	{
		map = new int[size] {
			22, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 22,
			2, 2, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 59, 0, 0, 0, 24, 24, 24, 24, 24, 24, 24, 2, 2,
			2, 2, 57, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 60, 0, 0, 0, 61, 58, 58, 58, 58, 58, 58, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 154, 0, 0, 0, 2, 59, 0, 0, 2, 59, 0, 0, 154, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 56, 0, 0, 2, 2, 2, 2, 2, 59, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 103, 0, 0, 0, 0, 2, 57, 58, 58, 58, 60, 0, 0, 61, 58, 58, 58, 2, 56, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 81, 81, 81, 2, 59, 0, 0, 2, 2, 81, 81, 2, 56, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 61, 58, 58, 58, 2, 56, 0, 0, 2, 57, 58, 58, 58, 60, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 56, 0, 0, 2, 2, 59, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 57, 60, 0, 0, 61, 2, 56, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 59, 103, 0, 2, 81, 56, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 61, 2, 56, 0, 0, 2, 57, 60, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 56, 0, 103, 2, 2, 59, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 0, 0, 0, 154, 0, 0, 0, 0, 154, 0, 0, 2, 2, 57, 60, 0, 0, 61, 2, 2, 59, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 0, 2, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 2, 57, 60, 0, 0, 0, 0, 61, 2, 56, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 0, 2, 2, 57, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 60, 0, 0, 0, 0, 0, 0, 2, 56, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 2, 2, 57, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 56, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 2, 57, 60, 0, 0, 0, 0, 154, 0, 0, 0, 0, 154, 0, 0, 0, 0, 0, 0, 0, 2, 2, 56, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 2, 56, 0, 0, 0, 2, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 2, 57, 60, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 2, 56, 0, 0, 2, 2, 57, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 60, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 2, 56, 0, 0, 2, 57, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 2, 56, 0, 0, 2, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 0, 61, 60, 0, 0, 61, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
			2, 2, 56, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 0, 2, 2,
			2, 2, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 59, 0, 0, 0, 81, 81, 81, 81, 81, 81, 81, 2, 2,
		};

	}
	else if (stage == 3)
	{
		map = new int[size] {
			22,   22,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 22, 22,
			8,   9,   25,   25,   25,   25,   25,   25,   25,   62,   0,   0,   0,   25,   25,   25,   25,   25,   25,   62,  0,   0,   0,  25, 25, 25, 25, 25, 25, 25, 8, 9,
			10,   11,   63,  64,  64,  64,  64,  64,  64,  66,  0,  0,  0,  67,  64,  64,  64, 64,  64,  66,  0,  0,  0,  67, 64,  64, 64, 64,  64, 64, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   154,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   105,   0,   0,   0,  0,   0,   0,   0, 0, 154, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   81,   81,   81,   81,   65,   0,   0,   0,   0,   0,   81,   81,   65,   0,   0,   0,  0,   0,   81,   81, 81, 81, 65, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   67,   64,   64,   64,   66,   0,   0,   0,   0,   0,   67,   64,   66,   0,   0,   0,  0,   0,   67,   64, 64, 64, 66, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   154,   0,   0,   0,   0,   0,   0,   103,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 154, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   81,   81,   81,   81,   65,   0,   0,   0,   0,   0,   81,   81,   81,   81,   65,   0,   0,  0,   0,   0,   81, 81, 81, 81, 65, 0, 10, 11,
			8,   9,   62,   0,   67,   64,   64,   64,   66,   0,   0,   0,   0,   0,   67,   64,   64,   64,   66,   0,   0,0,  0,   0,   67,  64, 64, 64, 66, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   154,   0,   0,   0,   0,   0,   0,   154,   0,   0,   154,   0,   0,   0,  0,   0,   0,   154, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   81,   81,   81,   81,   65,   0,   0,   0,   81,   81,   81,   81,   81,   81,   65,   0,  0,   0,   81,   81, 81, 81, 65, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   67,   64,   64,   64,   66,   0,   0,   0,   67,   64,   64,   64,   64,   64,   66,   0,  0,   0,   67,   64, 64, 64, 66, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   102,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   102,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   81,   81,   81,   81,   65,   0,   81,   81,   81,   65,   0,   81,   81,   81,   65,  0,   81,   81,   81, 81, 65, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   67,   64,   64,   64,   66,   0,   67,   64,   64,   66,   0,   67,   64,   64,   66,  0,   67,   64,   64, 64, 66, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   81,   81,   81,   81,   81,   81,   65,   0,   0,   0,   81,   81,   81,  81,   81,   81,   65, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,   0,   0,   0,   0,   0,   67,   64,   64,   64,   64,   64,   66,   0,   0,   0,   67,   64,   64,64,   64,   64,   66, 0, 0, 0, 0, 0, 8, 9,
			10,   11,   62,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 10, 11,
			8,   9,   62,  100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,   0,   0,  0, 0, 0, 0, 101, 0, 8, 9,
			10,   11,   81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 10, 11,
		};
	}
	else if (stage == 4)
	{
	map = new int[size] {
			22, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 22,
			13, 14, 26, 26, 26, 26, 26, 26, 26, 68, 0, 0, 0, 26, 26, 26, 26, 26, 26, 68, 0, 0, 0, 26, 26, 26, 26, 26, 26, 26, 13, 14,
			15, 16, 69, 70, 70, 70, 70, 70, 70, 72, 0, 0, 0, 67, 70, 70, 70, 70, 70, 66, 0, 0, 0, 67, 70, 70, 70, 70, 70, 70, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 0, 0, 83, 83, 83, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 83, 83, 83, 0, 0, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 83, 0, 0, 0, 105, 0, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 0, 83, 83, 0, 0, 0, 83, 83, 83, 0, 0, 0, 0, 0, 0, 83, 83, 83, 0, 0, 0, 83, 83, 0, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 83, 83, 0, 83, 83, 0, 83, 83, 83, 0, 0, 0, 0, 0, 0, 83, 83, 83, 0, 83, 83, 0, 83, 83, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 83, 83, 83, 83, 0, 0, 0, 0, 83, 83, 83, 0, 0, 0, 103, 0, 0, 83, 83, 83, 0, 0, 0, 0, 83, 83, 83, 83, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 12, 12, 12, 12, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 83, 83, 0, 83, 83, 0, 83, 83, 83, 0, 12, 0, 0, 12, 0, 83, 83, 83, 0, 83, 83, 0, 83, 83, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 12, 0, 0, 12, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 83, 83, 83, 83, 0, 0, 0, 0, 83, 83, 83, 0, 12, 0, 0, 12, 0, 83, 83, 83, 0, 0, 0, 0, 83, 83, 83, 83, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 12, 12, 12, 12, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 83, 83, 0, 83, 83, 0, 83, 83, 83, 0, 0, 0, 0, 0, 0, 83, 83, 83, 0, 83, 83, 0, 83, 83, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 83, 83, 83, 83, 0, 0, 0, 0, 83, 83, 83, 0, 0, 0, 0, 0, 0, 83, 83, 83, 0, 0, 0, 0, 83, 83, 83, 83, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 83, 83, 0, 83, 83, 0, 83, 83, 83, 0, 0, 0, 0, 0, 0, 83, 83, 83, 0, 83, 83, 0, 83, 83, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 83, 83, 83, 83, 83, 83, 83, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14,
			15, 16, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 16,
			13, 14, 83, 83, 0, 0, 83, 83, 83, 12, 83, 83, 0, 0, 0, 0, 0, 0, 0, 0, 83, 83, 83, 12, 83, 83, 0, 0, 83, 83, 13, 14,
			15, 16, 71, 70, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 15, 16,
			13, 14, 68, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 101, 0, 13, 14,
			15, 16, 83, 83, 83, 83, 83, 83, 83, 68, 0, 0, 0, 83, 83, 83, 83, 83, 83, 0, 0, 0, 68, 83, 83, 83, 83, 83, 83, 83, 15, 16,
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
				e->SetPlayer(player);
				e->SetPlayer2(player2);  
				enemies.push_back(e);

				map[i] = 0;
			}
			else if (tile == Tile::INVADER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				e = new Enemy(pos, hState::EIDLE, hLook::ELEFT, hType::INVADER);
				e->Initialise();
				e->SetTileMap(level);
				enemies.push_back(e);

				map[i] = 0;
			}
			else if (tile == Tile::MIGHTA)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				e = new Enemy(pos, hState::EIDLE, hLook::ELEFT, hType::MIGHTA);
				e->Initialise();
				e->SetTileMap(level);
				enemies.push_back(e);

				map[i] = 0;
			}
			else if (tile == Tile::DRUNK)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				e = new Enemy(pos, hState::EIDLE, hLook::ELEFT, hType::DRUNK);
				e->Initialise();
				e->SetTileMap(level);
				e->SetPlayer(player);
				e->SetPlayer2(player2);
				enemies.push_back(e);

				map[i] = 0;
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
	delete map;
	return AppStatus::OK;
}

void Scene::PlayerBubbleSpawn()
{
	bubbleCooldown += GetFrameTime();

	if (IsKeyPressed(KEY_E) && bubbleCooldown >= .3 && !isGameOver)
	{
		if (player->CanStartShooting() == true) {
			if (player->IsLookingLeft())
			{
				player->StartShooting();
				PlayerBubble* bubble = new PlayerBubble(player->GetPos(), Directions::LEFT);
				bubble->Initialise();
				sfxs[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");
				PlaySound(sfxs[0]);
				playerBubbles.push_back(bubble);
				PlayerBubble* pBubble = new PlayerBubble(player->GetPos(), Directions::LEFT);

			}
			else
			{
				player->StartShooting();
				PlayerBubble* bubble = new PlayerBubble(player->GetPos(), Directions::RIGHT);
				bubble->Initialise();
				sfxs[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");
				PlaySound(sfxs[0]);
				playerBubbles.push_back(bubble);
				PlayerBubble* pBubble = new PlayerBubble(player->GetPos(), Directions::LEFT);
			}
			bubbleCooldown = 0;

		}
		

	}


}

void Scene::deletePBubbles()
{
	auto check = playerBubbles.begin();
	while (check != playerBubbles.end()) {
		
		if (!(*check)->isAlive()) {
			// Si hay un enemigo dentro de la burbuja
			

			// Libera la memoria de la burbuja
			delete* check;
			check = playerBubbles.erase(check);
		}
		else {
			++check;
		}
	}
}

void Scene::Player2BubbleSpawn()
{
	bubbleCooldown += GetFrameTime();

	if (IsKeyPressed(KEY_DOWN) && bubbleCooldown >= .3 && !isGameOver)
	{
		if (player2->CanStartShooting() == true) {
			if (player2->IsLookingLeft())
			{
				player2->StartShooting();
				Player2Bubble* bubble = new Player2Bubble(player2->GetPos(), Bubble2Directions::LEFT);
				bubble->Initialise();
				sfxs[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");
				PlaySound(sfxs[0]);
				player2Bubbles.push_back(bubble);
				Player2Bubble* pBubble = new Player2Bubble(player2->GetPos(), Bubble2Directions::LEFT);

			}
			else
			{
				player2->StartShooting();
				Player2Bubble* bubble = new Player2Bubble(player2->GetPos(), Bubble2Directions::RIGHT);
				bubble->Initialise();
				sfxs[0] = LoadSound("sound/SoundEffects/Characters/AttackFX.wav");
				PlaySound(sfxs[0]);
				player2Bubbles.push_back(bubble);
				Player2Bubble* pBubble = new Player2Bubble(player2->GetPos(), Bubble2Directions::LEFT);
			}
			bubbleCooldown = 0;

		}


	}


}

void Scene::deleteP2Bubbles()
{
	auto check = player2Bubbles.begin();
	while (check != player2Bubbles.end()) {

		if (!(*check)->isAlive()) {
			// Si hay un enemigo dentro de la burbuja


			// Libera la memoria de la burbuja
			delete* check;
			check = player2Bubbles.erase(check);
		}
		else {
			++check;
		}
	}
}


void Scene::Update()
{
	Point p1, p2;
	AABB box;
	PlayerBubbleSpawn();
	Player2BubbleSpawn();
	deletePBubbles();
	deleteP2Bubbles();
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
	else if (IsKeyPressed(KEY_FOUR)) {
		LoadLevel(4);
		player->InitLife();
		actualLevel = 4;
	}
	if ((player->GetScore() + player2->GetScore()) >= goal_score[actualLevel - 1])
	{
		actualLevel++;
		
		if (actualLevel > 3) {
			isGameWon = true;
		}

		LoadLevel(actualLevel);
	}
	else if (player2->GetScore() >= goal_score[actualLevel - 1])
	{
		actualLevel++;

		if (actualLevel > 3) {
			isGameWon = true;
		}

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
	player2->Update();
	player->Update();
	

	if (isPlayerDead && isPlayer2Dead) {
		StartDying();
	}
	else if (isPlayerDead && !player2->canPlay) {
		StartDying();
	}


	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player2->Draw();
		player->Draw();
		
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
	player2->Release();
	player->Release();
	
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box = player->GetHitbox();
	AABB player2_box = player2->GetHitbox();

	CheckPlayerObjectCollisions(player_box);
	CheckPlayer2ObjectCollisions(player2_box);

	CheckBubbleEnemyCollisions();
	CheckBubble2EnemyCollisions();

	CheckPlayerEnemyCollisions(player_box);
	CheckPlayer2EnemyCollisions(player2_box);

	
}

void Scene::CheckPlayerObjectCollisions(const AABB& player_box)
{
	auto it = objects.begin();
	while (it != objects.end())
	{
		AABB obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());

			// Delete the object
			delete* it;
			// Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			// Move to the next object
			++it;
		}
	}
}

void Scene::CheckBubbleEnemyCollisions() {
	auto it_bubbles = playerBubbles.begin();
	while (it_bubbles != playerBubbles.end()) {
		AABB bubb_box = (*it_bubbles)->GetHitbox();
		auto it_enemies = enemies.begin();
		while (it_enemies != enemies.end()) {
			AABB enemy_box = (*it_enemies)->GetHitbox();
			if (enemy_box.TestAABB(bubb_box) && !(*it_bubbles)->isEnemyInside) {
				// Obtener el tipo de enemigo
				hType enemyType = (*it_enemies)->GetType();
				// Manejar la colisión basándose en el tipo de enemigo
				if (enemyType == hType::ZENCHAN) {
					(*it_bubbles)->ZenchanInside();
				}
				else if (enemyType == hType::INVADER) {
					(*it_bubbles)->InvaderInside();
				}
				else if (enemyType == hType::MIGHTA) {
					(*it_bubbles)->MightaInside();
				}
				else if (enemyType == hType::DRUNK) {
					(*it_bubbles)->DrunkInside();
				}
				delete* it_enemies;
				it_enemies = enemies.erase(it_enemies);
				(*it_bubbles)->isEnemyInside = true;
				break; // Salir del bucle interno después de eliminar el enemigo
			}
			else {
				++it_enemies;
			}
		}

		if (it_bubbles != playerBubbles.end()) ++it_bubbles; // Avanzar a la siguiente burbuja si no fue eliminada
	}
}

void Scene::CheckPlayerEnemyCollisions(const AABB& player_box)
{
	auto it_enemies = enemies.begin();
	while (it_enemies != enemies.end())
	{
		AABB enemy_box = (*it_enemies)->GetHitbox();
		if (player_box.TestAABB(enemy_box))
		{
			player->LifeManager();
			if(player->isDead) {
				isPlayerDead = true;
			}
			break;
		}
		else
		{
			++it_enemies;
		}

		
	}
}

void Scene::CheckPlayer2ObjectCollisions(const AABB& player_box)
{
	auto it = objects.begin();
	while (it != objects.end())
	{
		AABB obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player2->IncrScore((*it)->Points());

			// Delete the object
			delete* it;
			// Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			// Move to the next object
			++it;
		}
	}
}

void Scene::CheckBubble2EnemyCollisions() {
	auto it_bubbles = player2Bubbles.begin();
	while (it_bubbles != player2Bubbles.end()) {
		AABB bubb_box = (*it_bubbles)->GetHitbox();
		auto it_enemies = enemies.begin();
		while (it_enemies != enemies.end()) {
			AABB enemy_box = (*it_enemies)->GetHitbox();
			if (enemy_box.TestAABB(bubb_box) && !(*it_bubbles)->isEnemyInside) {
				// Obtener el tipo de enemigo
				hType enemyType = (*it_enemies)->GetType();
				// Manejar la colisión basándose en el tipo de enemigo
				if (enemyType == hType::ZENCHAN) {
					(*it_bubbles)->ZenchanInside();
				}
				else if (enemyType == hType::INVADER) {
					(*it_bubbles)->InvaderInside();
				}
				else if (enemyType == hType::MIGHTA) {
					(*it_bubbles)->MightaInside();
				}
				else if (enemyType == hType::DRUNK) {
					(*it_bubbles)->DrunkInside();
				}
				delete* it_enemies;
				it_enemies = enemies.erase(it_enemies);
				(*it_bubbles)->isEnemyInside = true;
				break; // Salir del bucle interno después de eliminar el enemigo
			}
			else {
				++it_enemies;
			}
		}

		if (it_bubbles != player2Bubbles.end()) ++it_bubbles; // Avanzar a la siguiente burbuja si no fue eliminada
	}
}

void Scene::CheckPlayer2EnemyCollisions(const AABB& player_box)
{
	auto it_enemies = enemies.begin();
	while (it_enemies != enemies.end())
	{
		AABB enemy_box = (*it_enemies)->GetHitbox();
		if (player_box.TestAABB(enemy_box))
		{
			player2->LifeManager();
			if (player2->isDead) {
				isPlayer2Dead = true;
			}
			break;
		}
		else
		{
			++it_enemies;
		}


	}
}

void Scene::StartDying()
{
	timeToDie += GetFrameTime();

	if(timeToDie > 2) isGameOver = true;
}

void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	for (PlayerBubble* bubble : playerBubbles)
	{
		delete bubble;
	}
	playerBubbles.clear();
	for (Player2Bubble* bubble2 : player2Bubbles)
	{
		delete bubble2;
	}
	player2Bubbles.clear();
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
	for (Player2Bubble* bubb2 : player2Bubbles)
	{
		bubb2->Update();
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
	auto it2 = player2Bubbles.begin();
	while (it2 != player2Bubbles.end())
	{

		(*it2)->Draw();
		++it2;
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
	for (PlayerBubble* pbubb : playerBubbles)
	{
		pbubb->DrawDebug(col);
	}
	for (Player2Bubble* p2bubb : player2Bubbles)
	{
		p2bubb->DrawDebug(col);
	}

}
void Scene::RenderGUI() const
{
	//Player 1
	font1->Draw(29, 0, TextFormat("1UP"), GREEN);
	font1->Draw(29, 8, TextFormat("%d", player->GetScore()));

	if (player->getLife() - 1 == 2) {
		font1->Draw(0, WINDOW_HEIGHT - 8, TextFormat("##"));
	}
	else if(player->getLife() - 1 == 1) {
		font1->Draw(0, WINDOW_HEIGHT - 8, TextFormat("#"));
	}        
	else if (player->getLife() - 1 == 0) {
		font1->Draw(0, WINDOW_HEIGHT - 8, TextFormat(" "));
	}


	//Player 2
	font1->Draw(196, 0, TextFormat("2UP"), SKYBLUE);
	font1->Draw(196, 8, TextFormat("%d", player2->GetScore()));
	if (player2->getLife() - 1 == 2) {
		font1->Draw(WINDOW_WIDTH - 16, WINDOW_HEIGHT - 8, TextFormat("$$"));
	}
	else if (player2->getLife() - 1 == 1) {
		font1->Draw(WINDOW_WIDTH - 16, WINDOW_HEIGHT - 8, TextFormat("$"));
	}
	else if (player2->getLife() - 1 == 0) {
		font1->Draw(WINDOW_WIDTH - 16, WINDOW_HEIGHT - 8, TextFormat(" "));
	}

	//High Score
	font1->Draw(84, 00, TextFormat("HIGH SCORE"), RED);
	if (player->GetScore() > player2->GetScore()) {
		font1->Draw(110, 8, TextFormat("%d", player->GetScore()));
	}
	else if (player->GetScore() < player2->GetScore()) {
		font1->Draw(110, 8, TextFormat("%d", player2->GetScore()));
	}
	else {
		font1->Draw(110, 8, TextFormat("%d", player->GetScore()));
	}

}