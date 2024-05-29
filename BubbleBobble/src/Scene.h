#pragma once
#include <raylib.h>
#include "Player.h"
#include "Player2.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Object.h"
#include "PlayerBubble.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();
    AppStatus Init();
    void Update();
    void Render();
    void Release();
    AppStatus LoadLevel(int stage);
    void PlayerBubbleSpawn();
    void deletePBubbles();
    bool isGameOver;
    bool isGameWon;

private:

    void CheckCollisions();
    void CheckPlayerObjectCollisions(const AABB& player_box);
    void CheckBubbleEnemyCollisions();
    void CheckPlayerEnemyCollisions(const AABB& player_box);
    void StartDying();
    void ClearLevel();
    void UpdateBubbles();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    bool isPlayerDead = false;
    //Player2 *player2;
    TileMap *level;
    int goal_score[3];
    int actualLevel;
    std::vector<Object*> objects;
    std::vector<Enemy*> enemies;
    std::vector<PlayerBubble*> playerBubbles;
    PlayerBubble* pBubble;
    int collisionCount;
    float eBubblingTime;
    float timeToDie = 0;
    Camera2D camera;
    DebugMode debug;
};

