#pragma once
#include <raylib.h>
#include "Player.h"
#include "Player2.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Object.h"
#include "PlayerBubble.h"
#include "Player2Bubble.h"
#include "DrunkShot.h"
#include "InvaderShot.h"
#include "MightaShot.h"
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

    void DrunkShotSpawn();
    void deleteDrunkShot();
    void InvaderShotSpawn();
    void deleteInvaderShot();
    void MightaShotSpawn();
    void deleteMightaShot();
    void ObjectSpawn();
    void BubbleEnemySpawn();
    bool isGameOver;
    bool isGameWon;

private:

    void CheckCollisions();
    //Player 1 collisions
    void CheckPlayerObjectCollisions(const AABB& player_box);
    void CheckBubbleEnemyCollisions();
    void CheckPlayerEnemyCollisions(const AABB& player_box);
    void CheckPlayerProjectileCollisions(const AABB& player_box);
    void CheckPlayerBubbleCollisions(const AABB& player_box);

    //Player 2 collisions
    void CheckPlayer2ObjectCollisions(const AABB& player_box);
    void CheckBubble2EnemyCollisions();
    void CheckPlayer2EnemyCollisions(const AABB& player_box);
    void CheckPlayer2ProjectileCollisions(const AABB& player_box);
    void CheckPlayer2BubbleCollisions(const AABB& player_box);

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
    float waitForAnimTimer = 0;
    int goal_score[3];
    int actualLevel;
    std::vector<Object*> objects;
    Object* object;
    std::vector<Enemy*> enemies;
    Enemy* enemy;
    std::vector<PlayerBubble*> playerBubbles;
    PlayerBubble* pBubble;
    std::vector<Player2Bubble*> player2Bubbles;
    PlayerBubble* p2Bubble;
    std::vector<DrunkShot*> dShots;
    DrunkShot* dShot;
    std::vector<InvaderShot*> iShots;
    InvaderShot* iShot;
    std::vector<MightaShot*> mShots;
    MightaShot* mShot;
    int collisionCount;
    float bubbleCooldown;
    float bubble2Cooldown;
    float drunkCooldown;
    float invaderCooldown;
    float mightaCooldown;
    int objectChosen;
    int posChosen;
    float timeToDie = 0;

    bool canSpawnObjects = false;
    float objectSpawnTimer = 0;

    Camera2D camera;
    DebugMode debug;
};

