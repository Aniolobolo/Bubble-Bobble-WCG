#include "Game.h"
#include "Globals.h"
#include "Player.h"
#include "ResourceManager.h"
#include <stdio.h>

Sound soundEffects[30];
Music levelMusic;
Music bossMusic;

Game::Game()
{
    gState = GameState::ACLARATION;
    scene = nullptr;
    img_menu = nullptr;
    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Bubble Bobble");
    

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
    
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/menu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    
    if (data.LoadTexture(Resource::IMG_GAMEOVER, "images/gameOver.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_gameover = data.GetTexture(Resource::IMG_GAMEOVER);

    if (data.LoadTexture(Resource::IMG_GAMEWON, "images/badend.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_win = data.GetTexture(Resource::IMG_GAMEWON);

    if (data.LoadTexture(Resource::IMG_GAMEGOODEND, "images/happyend.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_goodwin = data.GetTexture(Resource::IMG_GAMEGOODEND);

    if (data.LoadTexture(Resource::IMG_INSERTCOIN, "images/insertCoin.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_insertcoin = data.GetTexture(Resource::IMG_INSERTCOIN);

    if (data.LoadTexture(Resource::IMG_ACLARATION, "images/aclaration.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_aclaration = data.GetTexture(Resource::IMG_ACLARATION);

    if (data.LoadTexture(Resource::IMG_CREDITS, "images/credits.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_credits = data.GetTexture(Resource::IMG_CREDITS);
    
    if (data.LoadTexture(Resource::IMG_PROFESSORS, "images/professors.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_profs = data.GetTexture(Resource::IMG_PROFESSORS);

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    levelMusic = LoadMusicStream("sound/Music/1-Main-Theme.ogg");
    PlayMusicStream(levelMusic);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }
    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;
    UpdateMusicStream(levelMusic);
    switch (gState)
    {
    case GameState::ACLARATION:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            gState = GameState::CREDITS;
        }
        break;
    case GameState::CREDITS:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            gState = GameState::PROFESSORS;
        }
        break;
    case GameState::PROFESSORS:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            gState = GameState::MAIN_MENU;
        }
        break;
    case GameState::MAIN_MENU: 
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                gState = GameState::INSERT_COIN;
            }
            break;
        case GameState::INSERT_COIN:
            if (IsKeyPressed(KEY_ESCAPE)) gState = GameState::MAIN_MENU;
            if (IsKeyPressed(KEY_SPACE))
            {
                soundEffects[1] = LoadSound("sound/SoundEffects/Intro/CoinInsertedFX.wav");
                PlaySound(soundEffects[1]);
                if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                gState = GameState::PLAYING;
            }
            break;

        case GameState::GAMEOVER:
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_SPACE)) gState = GameState::MAIN_MENU;

            break;
        case GameState::WIN:
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_SPACE)) gState = GameState::MAIN_MENU;

            break;

        case GameState::GOODWIN:
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_SPACE)) gState = GameState::MAIN_MENU;

            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE))
            {
                StopMusicStream(levelMusic);
                FinishPlay();
                gState = GameState::MAIN_MENU;
            }
            else if (IsKeyPressed(KEY_O))
            {
                StopMusicStream(levelMusic);
                soundEffects[2] = LoadSound("sound/Music/9-Game-Over.ogg");
                PlaySound(soundEffects[2]);
                FinishPlay();
                gState = GameState::GAMEOVER;
                
            }
            else if (IsKeyPressed(KEY_P))
            {
                StopMusicStream(levelMusic);
                FinishPlay();
                gState = GameState::GOODWIN;
            }
            else if (IsKeyPressed(KEY_L))
            {
                StopMusicStream(levelMusic);
                FinishPlay();
                gState = GameState::WIN;
            }
            else if (scene->isGameOver == true) {
                StopMusicStream(levelMusic);
                soundEffects[2] = LoadSound("sound/Music/9-Game-Over.ogg");
                PlaySound(soundEffects[2]);
                gState = GameState::GAMEOVER;
            }
            else if (scene->isGameWon == true) {
                StopMusicStream(levelMusic);
                gState = GameState::WIN;
                
            }
            else if (scene->isGoodGameWon == true) {
                StopMusicStream(levelMusic);
                gState = GameState::GOODWIN;
            }
            else
            {
                //Game logic
                scene->Update();
            }
            
            break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (gState)
    {
        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);
            break;

        case GameState::INSERT_COIN:
            DrawTexture(*img_insertcoin, 0, 0, WHITE);
            break;

        case GameState::WIN:
            DrawTexture(*img_win, 0, 0, WHITE);
            break;

        case GameState::GOODWIN:
            DrawTexture(*img_goodwin, 0, 0, WHITE);
            break;

        case GameState::GAMEOVER:
            DrawTexture(*img_gameover, 0, 0, WHITE);
            break;

        case GameState::ACLARATION:
            DrawTexture(*img_aclaration, 0, 0, WHITE);
            break;

        case GameState::CREDITS:
            DrawTexture(*img_credits, 0, 0, WHITE);
            break;

        case GameState::PROFESSORS:
            DrawTexture(*img_profs, 0, 0, WHITE);
            break;

        case GameState::PLAYING:
            scene->Render();
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    StopMusicStream(levelMusic);
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_INSERTCOIN);
    data.ReleaseTexture(Resource::IMG_GAMEOVER);
    data.ReleaseTexture(Resource::IMG_GAMEWON);
    data.ReleaseTexture(Resource::IMG_GAMEGOODEND);
    data.ReleaseTexture(Resource::IMG_ACLARATION);
    data.ReleaseTexture(Resource::IMG_CREDITS);
    data.ReleaseTexture(Resource::IMG_PROFESSORS);

    UnloadRenderTexture(target);
}