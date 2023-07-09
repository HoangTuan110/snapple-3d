/* Snapple 3D - Snake, but you are the apple, but it doesn't played like Snake at all
** 
** The license of this game is MIT License. Check LICENSE for details.
*/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

// Struct definition //
typedef struct Entity {
    Vector3 pos;
    float   w, h, l;
    Color   mainColor, wireColor;
} Entity;

// Global Values Declaration //
static int screenWidth = 800,
           screenHeight = 450,
           normalFontSize = 32,
           playerSpeed = 1,
           score = 0,
           fontSpacing = 2;
static float defaultSoundPitch = 0.5f,
             snakeSpeed = 0.25f;
static bool gameOver = false,
            gameMenu = true,
            gamePause = false,
            gameCredits = false;
static Color snakeBodyColor = GREEN,
             snakeHeadColor = LIME,
             snakeWireColor = DARKGREEN;
static char gameTitle[512] = "Snapple 3D - GMTK 2023";
static Entity player = {
    { 0.0f, 1.0f, 0.0f },
      2.0f, 2.0f, 2.0f,
      RED, BLACK
};
static Entity snake[4];

// Helper functions //
bool stringEqual(char *a, char *b) { return !strcmp(a, b); }
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
void drawTextExtra(Font font, const char *text, int posX, int posY, int fontSize, Color col) {
    DrawTextEx(font, text, (Vector2){posX, posY}, fontSize, fontSpacing, col);
}
bool checkCollisionBoxes(Entity box1, Entity box2) {
    return CheckCollisionBoxes(
            (BoundingBox){(Vector3){ box1.pos.x - box1.w/2,
                                     box1.pos.y - box1.h/2,
                                     box1.pos.z - box1.l/2 },
                          (Vector3){ box1.pos.x + box1.w/2,
                                     box1.pos.y + box1.h/2,
                                     box1.pos.z + box1.l/2 }},
            (BoundingBox){(Vector3){ box2.pos.x - box2.w/2,
                                     box2.pos.y - box2.h/2,
                                     box2.pos.z - box2.l/2 },
                          (Vector3){ box2.pos.x + box2.w/2,
                                     box2.pos.y + box2.h/2,
                                     box2.pos.z + box2.l/2 }}
    );
}

// Handle Entity-related things //
void drawEntity(Entity entity) {
    DrawCube(entity.pos, entity.w, entity.h, entity.l, entity.mainColor);
    DrawCubeWires(entity.pos, entity.w, entity.h, entity.l, entity.wireColor);
}

// Generate snake at random position within the grid
void generateSnake() {
    // Generate snake head
    snake[0].pos = (Vector3){
        (rand() % 2) ? rand() % 20 + 2 : rand() % -20 + -2, 1.0f,
        (rand() % 2) ? rand() % 18 + 2 : rand() % -16 + -2 };
    snake[0].w = 2.0f, snake[0].h = 2.0f, snake[0].l = 2.0f;
    snake[0].mainColor = snakeHeadColor,
    snake[0].wireColor = snakeWireColor;
    for (int i = 1; i < 4; i++) {
        snake[i].pos = snake[i - 1].pos;
        snake[i].pos.x += 2.0f;
        snake[i].w = 2.0f, snake[i].h = 2.0f, snake[i].l = 2.0f;
        snake[i].mainColor = snakeBodyColor,
        snake[i].wireColor = snakeWireColor;
    }
}

// Move the snake to a specific position
void moveSnake(Vector3 v) {
    Entity prev = snake[0];
    snake[0].pos = v;
    for (int i = 1; i < 4; i++) {
        Vector2 snakeBodyDir = Vector2MoveTowards(
            (Vector2){ snake[i].pos.x, snake[i].pos.z },
            (Vector2){ prev.pos.x, prev.pos.z },
            snakeSpeed
        );
        if (!(checkCollisionBoxes(snake[i], prev)))
            snake[i].pos = (Vector3){
                snakeBodyDir.x,
                snake[i].pos.y,
                snakeBodyDir.y
            };
        prev = snake[i];
    }
}

// Handle Draw-related things //
void drawMenu(Font font)
{   
    ClearBackground(WHITE);
    drawTextExtra(font, "Snapple 3D", screenWidth / 2 - 145, screenHeight / 2 - 80, normalFontSize*2, GREEN);
    drawTextExtra(font, "Press Enter to start", screenWidth / 2 - 155, screenHeight / 2 - 80 + normalFontSize * 2, normalFontSize, BLACK);
    drawTextExtra(font, "A crappy game by Tsk. For GMTK Game Jam 2023.", 10, screenHeight - normalFontSize, normalFontSize - 16, GRAY);
    drawTextExtra(font, "C - Credits and License", 10, screenHeight - normalFontSize - 16, normalFontSize - 16, BLACK);
}

void drawCredits(Font font)
{   
    ClearBackground(WHITE);
    drawTextExtra(font, "Credits and License", 10, 10, normalFontSize * 2, GREEN);
    drawTextExtra(font, "Chiptune music by Juhani Junkala [CC0]", 10, 10 + normalFontSize * 2, normalFontSize, BLACK);
    drawTextExtra(font, "https://opengameart.org/content/4-chiptunes-adventure", 10, 10 + normalFontSize * 3, normalFontSize - 16, GRAY);
    drawTextExtra(font, "Font 'Monogram' by datagoblin [CC0]", 10, 10 + normalFontSize * 4, normalFontSize, BLACK);
    drawTextExtra(font, "https://datagoblin.itch.io/monogram", 10, 10 + normalFontSize * 5, normalFontSize - 16, GRAY);
    drawTextExtra(font, "Raylib by Ramon Santamaria [zlib]", 10, 10 + normalFontSize * 6, normalFontSize, BLACK);
    drawTextExtra(font, "https://raylib.com", 10, 10 + normalFontSize * 7, normalFontSize - 16, GRAY);
    drawTextExtra(font, "SFX by jsfxr", 10, 10 + normalFontSize * 8, normalFontSize, BLACK);
    drawTextExtra(font, "https://sfxr.me", 10, 10 + normalFontSize * 9, normalFontSize - 16, GRAY);
    drawTextExtra(font, "This game is under MIT license. See LICENSE", 10, 10 + normalFontSize * 10, normalFontSize, BLACK);
    drawTextExtra(font, "for details", 10, 10 + normalFontSize * 11, normalFontSize, BLACK);
    drawTextExtra(font, "Backspace - Back to menu", 10, screenHeight - normalFontSize, normalFontSize - 16, GRAY);
}

void drawUI(Font font)
{
    drawTextExtra(font, TextFormat("Score: %d", score), 10, 10, normalFontSize, BLACK);
    if (gamePause)
        drawTextExtra(font, "Paused", screenHeight - MeasureTextEx(font, "Paused", normalFontSize, fontSpacing).x, 10, normalFontSize, BLACK);
}

void drawGameOver(Font font)
{
    drawTextExtra(font, "Game over!", screenWidth / 2 - 155, screenHeight / 2 - 80,
                  normalFontSize*2, RED);
    drawTextExtra(font, TextFormat("Your score is %d", score), screenWidth / 2 - 155, screenHeight / 2 - 80 + normalFontSize * 2, normalFontSize, RED);
    drawTextExtra(font, "Press R to try again", screenWidth / 2 - 155, screenHeight / 2 - 80 + normalFontSize * 2 + normalFontSize, normalFontSize, RED);
}

// Update-Draw frame function //
void updateDrawFrame(Font font, Camera3D camera, Sound mainGameMusic, Sound menuMusic);
void setup();

int main(void)
{
    // Initialization
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, gameTitle);
    InitAudioDevice();
    SetTargetFPS(60);
    Font font = LoadFont("res/monogram.ttf");
    Sound mainGameMusic = LoadSound("res/jj_main_music.ogg");
    Sound menuMusic = LoadSound("res/jj_stage_select.ogg");
    SetSoundPitch(mainGameMusic, defaultSoundPitch);

    // Define the camera to look into our 3D world //
    Camera3D camera   = { 0 };
    camera.position   = (Vector3){ 0.0f, 10.0f, 10.0f };// Camera position
    camera.target     = (Vector3){ 0.0f, 0.0f, 0.0f }; // Camera looking at point
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f }; // Camera up vector (rotation towards target)
    camera.fovy       = 22.5f;                         // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC;           // Camera mode type

    generateSnake();

    // Main game loop
    while (!WindowShouldClose()) {
        updateDrawFrame(font, camera, mainGameMusic, menuMusic);
    }

    // De-Initialization
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void updateDrawFrame(Font font, Camera3D camera, Sound mainGameMusic, Sound menuMusic) {
    // Should have use a state machine smh
    if (gameMenu || gameCredits) {
        if (!(IsSoundPlaying(menuMusic))) PlaySound(menuMusic);
    } 
    else if (!gameOver) {
        if (!gamePause) {
            ResumeSound(mainGameMusic);
            if (IsSoundPlaying(menuMusic)) StopSound(menuMusic);
            if (!(IsSoundPlaying(mainGameMusic))) PlaySound(mainGameMusic);
        } else
            if (IsSoundPlaying(mainGameMusic)) PauseSound(mainGameMusic);
    }
    if (gameMenu) {
        if (IsKeyPressed(KEY_ENTER)) gameMenu = !gameMenu;
        if (IsKeyPressed(KEY_C)) {
            gameCredits = !gameCredits;
            gameMenu = !gameMenu;
        }
    }
    else if (gameCredits) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            gameMenu = !gameMenu;
            gameCredits = !gameCredits;
        }
    }
    else if (!gameOver) {
        if (!gamePause) {
            float delta = 30.0f * GetFrameTime();
            // Make the player move based on user input
            // Extra if statement added to prevent player getting out of screen
            if (IsKeyDown(KEY_UP))
                if (!(player.pos.z <= -16.0f)) player.pos.z -= playerSpeed * delta;
            if (IsKeyDown(KEY_DOWN))
                if (!(player.pos.z >= 18.0f)) player.pos.z += playerSpeed * delta;
            if (IsKeyDown(KEY_LEFT))
                if (!(player.pos.x <= -20.0f)) player.pos.x -= playerSpeed * delta;
            if (IsKeyDown(KEY_RIGHT))
                if (!(player.pos.x >= 20.0f)) player.pos.x += playerSpeed * delta;

            // Making sure the player doesn't get out of screen
            printf("(%.2f, %.2f, %.2f)\n", player.pos.x, player.pos.y, player.pos.z);

            Vector2 snakeDir = Vector2MoveTowards((Vector2){snake[0].pos.x, snake[0].pos.z},
                    (Vector2){player.pos.x, player.pos.z},
                    snakeSpeed);
            moveSnake((Vector3){snakeDir.x, snake[0].pos.y, snakeDir.y});

            // Check collision between snake and player
            for (int i = 0; i < 4; i++)
                if (checkCollisionBoxes(player, snake[i])) {
                    Sound hitHurtSFX = LoadSound("res/hitHurt.wav");
                    SetSoundVolume(hitHurtSFX, 5);
                    PlaySound(hitHurtSFX);
                    gameOver = true;
                }

            // Increase the score and increase snake speed if reached 100, 200,...
            // along with increasing the speed of the mainGameMusic as well
            // when the score reached 200, 400,...
            score += 1;
            if (!(score % 100) && score != 0) {
                snakeSpeed += ((rand() % 4) + 1) * .01f;
                if (!(score % 200)) {
                    defaultSoundPitch += 0.25f;
                    SetSoundPitch(mainGameMusic, defaultSoundPitch);
                }
            }
            
            // Pause the game by using the Space key
            if (IsKeyPressed(KEY_SPACE)) gamePause = !gamePause;
        } else {
            if (IsKeyPressed(KEY_SPACE)) gamePause = !gamePause;
        }
    }
    if (gameOver) StopSound(mainGameMusic);
    if (IsKeyPressed(KEY_R) && gameOver) {
        setup();
        SetSoundPitch(mainGameMusic, defaultSoundPitch);
    }

    // Draw
    BeginDrawing();
    drawMenu(font);
    BeginMode3D(camera);
    ClearBackground(WHITE);
    if (!gameOver) {
        drawEntity(player);
        for (int i = 0; i < 4; i++)
            drawEntity(snake[i]);
        DrawGrid(100, 1.0f);
    }
    EndMode3D();
    if (gameCredits) drawCredits(font);
    else if (gameMenu && !gameOver) drawMenu(font);
    else if (!gameOver) drawUI(font);
    else drawGameOver(font);
    EndDrawing();

}

void setup() {
    gameOver = false;
    gamePause = false;
    score = 0;
    player.pos = (Vector3){ 0.0f, 1.0f, 0.0f };
    generateSnake();
    snakeSpeed = 0.25f;
    defaultSoundPitch = 0.5f;
}
