#pragma once
#include <raylib.h>
#include "Player.h"
#include "Player2.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Object.h"
#include "Bubble.h"

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
    void RandomItemSpawn();
    void PlayerBubbleSpawn();
    int Score() const;
    int highScore();
    void ResetScore() const;

private:

    
    void BubbleSpawner();
    void UpdateBubbles();
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    Player2 *player2;
    Enemy* enemy1;
    TileMap *level;
    int goal_score[3];
    int actualLevel;
    std::vector<Object*> objects;
    std::vector<Bubble*> bubbles;

    float eBubblingTime;
    float eTimeSpawnX;
    float eTimeSpawnY;
    Camera2D camera;
    DebugMode debug;
};

