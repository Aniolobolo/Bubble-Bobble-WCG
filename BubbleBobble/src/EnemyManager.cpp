#include "EnemyManager.h"
#include "Mighta.h"
#include "Zenchan.h"


EnemyManager::EnemyManager()
{
	shots = nullptr;
	map = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/Enemies.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::SetTileMap(TileMap* level) {
	map = level;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy = nullptr;

	if (type == EnemyType::MIGHTA)
	{
		enemy = new Mighta(pos, MIGHTA_PHYSICAL_WIDTH, MIGHTA_PHYSICAL_HEIGHT, MIGHTA_FRAME_SIZE, MIGHTA_FRAME_SIZE, map);
	}
	else if (type == EnemyType::ZENCHAN)
	{
		enemy = new Mighta(pos, ZENCHAN_PHYSICAL_WIDTH, ZENCHAN_PHYSICAL_HEIGHT, ZENCHAN_FRAME_SIZE, ZENCHAN_FRAME_SIZE, map);
	}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	if (enemy)
	{
		enemy->Initialise(look, area);
		enemies.push_back(enemy);
	}
	else
	{
		LOG("Failed to create enemy");
	}

	enemy->Initialise(look, area);
	enemies.push_back(enemy);
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::MIGHTA)
	{
		width = MIGHTA_PHYSICAL_WIDTH;
		height = MIGHTA_PHYSICAL_HEIGHT;
	}
	//else if (type == EnemyType::TURRET)
	//{
	//	width = TURRET_PHYSICAL_WIDTH;
	//	height = TURRET_PHYSICAL_HEIGHT;
	//}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox)
{
	AABB box;
	bool shoot;
	Point p, d;
	for (Enemy* enemy : enemies)
	{
		shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
		}
	}
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		enemy->DrawVisibilityArea(DARKGRAY);
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	/*for (int i = 0; i<enemies.size(); ++i)
	{
		delete enemy[i]
	}*/
	for (Enemy* enemy : enemies)
	{
		if (enemy != nullptr)
			delete enemy;
	}
	enemies.clear();
}