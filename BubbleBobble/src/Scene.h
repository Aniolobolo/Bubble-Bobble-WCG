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
    int Score() const;
    int highScore();
    void ResetScore() const;

private:

    void CheckCollisions(); 
    void ClearLevel();
    void UpdateBubbles();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    Player2 *player2;
    std::vector<Enemy*> enemies;
    TileMap *level;
    int goal_score[3];
    int actualLevel;
    std::vector<Object*> objects;
    std::vector<PlayerBubble*> playerBubbles;
    PlayerBubble* pBubble;

    float eBubblingTime;
    float eTimeSpawnX;
    float eTimeSpawnY;
    Camera2D camera;
    DebugMode debug;
};

