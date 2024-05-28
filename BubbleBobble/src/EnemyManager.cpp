#include "EnemyManager.h"
#include "Enemy.h"


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
	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::SetTileMap(TileMap* level) {
	map = level;
}
void EnemyManager::Add(const Point& pos, hType type, const AABB& area, hLook look)
{


	Enemy* enemy = nullptr;

	if (type == hType::ZENCHAN)
	{
		enemy = new Enemy(pos, hState::EWALKING, hLook::ERIGHT, hType::ZENCHAN);
	}
	else if (type == hType::INVADER)
	{
		enemy = new Enemy(pos, hState::EWALKING, hLook::ERIGHT, hType::INVADER);
	}
	else if (type == hType::MIGHTA)
	{
		enemy = new Enemy(pos, hState::EWALKING, hLook::ERIGHT, hType::MIGHTA);
	}
	else if (type == hType::DRUNK)
	{
		enemy = new Enemy(pos, hState::EWALKING, hLook::ERIGHT, hType::DRUNK);
	}

	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	if (enemy)
	{
		enemy->Initialise();
		enemies.push_back(enemy);
	}
	else
	{
		LOG("Failed to create enemy");
	}
}

AABB EnemyManager::GetEnemyHitBox(const Point& pos, hType type) const
{
	int width, height;
	if (type == hType::ZENCHAN)
	{
		width = ENEMY_PHYSICAL_WIDTH;
		height = ENEMY_PHYSICAL_HEIGHT;
	}
	else if (type == hType::INVADER)
	{
		width = ENEMY_PHYSICAL_WIDTH;
		height = ENEMY_PHYSICAL_HEIGHT;
	}
	else if (type == hType::MIGHTA)
	{
		width = ENEMY_PHYSICAL_WIDTH;
		height = ENEMY_PHYSICAL_HEIGHT;
	}
	else if (type == hType::DRUNK)
	{
		width = ENEMY_PHYSICAL_WIDTH;
		height = ENEMY_PHYSICAL_HEIGHT;
	}
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
	bool shoot;
	Point p, d;

	//for (Enemy* enemy : enemies)
	//{
	//	shoot = enemy->Update(player_hitbox);
	//	if (shoot)
	//	{
	//		enemy->GetShootingPosDir(&p, &d);
	//		shots->Add(p, d);
	//		LOG("Added shot at position: ", p.x, p.y);

	//	}
	//}

}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	Color c;
	c.a = 0;

	for (const Enemy* enemy : enemies)
	{
		/*enemy->DrawVisibilityArea(c);*/
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::DestroyEnemy(Enemy* enemy)
{
	auto it = std::find(enemies.begin(), enemies.end(), enemy);
	if (it != enemies.end())
	{
		delete* it;
		enemies.erase(it);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}