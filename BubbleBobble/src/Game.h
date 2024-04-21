#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, ACLARATION, PROFESSORS, INSERT_COIN, GAMEOVER, WIN, PLAYING, SETTINGS, CREDITS };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    GameState gState;
    
    

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* img_aclaration;
    const Texture2D* img_credits;
    const Texture2D* img_profs;
    const Texture2D* img_win;
    const Texture2D *img_gameover;
    const Texture2D *img_insertcoin;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};