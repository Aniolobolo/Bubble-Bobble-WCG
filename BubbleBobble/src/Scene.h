#pragma once
#include <raylib.h>
#include "Player.h"
#include "Player2.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Object.h"
#include "PlayerBubble.h"
#include "Player2Bubble.h"
#include "Text.h"

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

    void Player2BubbleSpawn();
    void deleteP2Bubbles();
    bool isGameOver;
    bool isGameWon;

private:

    void CheckCollisions();
    //Player 1 collisions
    void CheckPlayerObjectCollisions(const AABB& player_box);
    void CheckBubbleEnemyCollisions();
    void CheckPlayerEnemyCollisions(const AABB& player_box);
    //Player 2 collisions
    void CheckPlayer2ObjectCollisions(const AABB& player_box);
    void CheckBubble2EnemyCollisions();
    void CheckPlayer2EnemyCollisions(const AABB& player_box);

    void StartDying();
    void ClearLevel();
    void UpdateBubbles();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    bool isPlayerDead = false;
    bool isPlayer2Dead = false;
    bool isEnemyInBubble = false;
    Player2 *player2;
    TileMap *level;
    Text* font1;
    int goal_score[3];
    int actualLevel;
    std::vector<Object*> objects;
    std::vector<Enemy*> enemies;
    std::vector<PlayerBubble*> playerBubbles;
    PlayerBubble* pBubble;
    std::vector<Player2Bubble*> player2Bubbles;
    PlayerBubble* p2Bubble;
    int collisionCount;
    float bubbleCooldown;
    float timeToDie = 0;
    Camera2D camera;
    DebugMode debug;
};

