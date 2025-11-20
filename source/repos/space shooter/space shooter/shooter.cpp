#include "raylib.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


const int width_sc = 1000;
const int height_sc = 600;               
const int Header_h = 50;
const int start = Header_h;


const int Bmax = 100;
int bulletsX[Bmax] = { 0 };
int bulletsY[Bmax] = { 0 };
bool bulletStatus[Bmax] = { false };
int bullet_idx = 0;
int bulletSpeed = 8;


const int Ecount = 15;
struct Enemy {
    Rectangle rect;
    int speed;
    bool active;
    Color color;
    int scoreValue;
};
Enemy enemies[Ecount];


int score = 0;                         //score and lives header type
int lives = 3;
int current_level = 1;
int gState = 0;
int activeEnemies = 6;


int pos_x = width_sc / 2;
int pos_y = height_sc - 60;
int width = 45;
int height = 20;
int playerSpeed = 10;



void Shoot(int playerX, int playerY, int pWidth);       // Bullets Logic
void UpdateBullets();                                                   // Bullets Logic
void ResetGame();                       // Reset Game
void InitializeEnemies();                            // Initialize Enemies
void RespawnEnemy(int i);                                                 // Respawn Enemy hwen killed


// Bullets Logic
void Shoot(int playerX, int playerY, int Width)
{
    bulletsX[bullet_idx] = playerX + Width / 2;
    bulletsY[bullet_idx] = playerY;

    bulletStatus[bullet_idx] = true;
    bullet_idx++;

    if (bullet_idx >= Bmax)
    {
		bullet_idx = 0;            // Reset to the first bullet slot if max is reached
    }
}

void UpdateBullets()
{
    for (int i = 0; i < Bmax; i++)
    {
        if (bulletStatus[i])
        {
            bulletsY[i] -= bulletSpeed;
            if (bulletsY[i] < 0)
            {
				bulletStatus[i] = false;      // Deactivate bullet if it goes off-screen
            }
        }
    }
}
// Respawn enemy to new position whenever they got destroyed
void RespawnEnemy(int i)
{
	enemies[i].rect.y = (int)start - enemies[i].rect.height - (int)GetRandomValue(0, 49);          // Random Y position above the screen
	enemies[i].rect.x = (int)GetRandomValue(0, width_sc - (int)enemies[i].rect.width);          // Random X position
}

void InitializeEnemies()
{
	srand(time(0));      // Seed for random position generation

    int SameSpawn = 0;

	for (int i = 0; i < activeEnemies; i++)       // Initialize only active enemies
    {
        enemies[i].rect.width = 30;
        enemies[i].rect.height = 30;
        enemies[i].speed = 0.5 + (current_level * 0.5);
        enemies[i].active = true;

		enemies[i].rect.x = (int)GetRandomValue(0, width_sc - (int)enemies[i].rect.width);              // Random X position


		enemies[i].rect.y = (int)start - (i*60);    // Staggered Y positions
        SameSpawn += 100;       

        if (i % 3 == 0) {
            enemies[i].color = RED;
            enemies[i].scoreValue = 30;
        }
        else if (i % 3 == 1) {
            enemies[i].color = BLUE;
            enemies[i].scoreValue = 20;
        }
        else {
            enemies[i].color = GREEN;
            enemies[i].scoreValue = 10;
        }
    }
}

void ResetGame()
{
    score = 0;
    lives = 3;
    current_level = 1;
    pos_x = width_sc / 2;
    pos_y = height_sc - 60;

    bullet_idx = 0;

    for (int i = 0; i < Bmax; i++)
    {
        bulletStatus[i] = false;
    }
    InitializeEnemies();
}


int main()
{
    InitWindow(width_sc, height_sc, "SPACE SHOOTER!");
    SetTargetFPS(60);

    InitializeEnemies();

    srand(time(0));

    while (!WindowShouldClose())
    {
       float x = GetFrameTime();

        if (gState == 0)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                gState = 1;
            }
        }

        if (lives <= 0)
        {
            gState = 2;
        }

        if (IsKeyPressed(KEY_R) && (gState == 1 || gState == 2))
        {
            ResetGame();
            gState = 1;
        }

        if (gState == 1)
        {
            // Player Movement
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                pos_x += playerSpeed;
            }
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            {
                pos_x -= playerSpeed;
            }
            if (pos_x < 0) {
                pos_x = 0;
            }
            if (pos_x > width_sc - width)
            {
                pos_x = width_sc - width;
            }
            // Shooting
            if (IsKeyPressed(KEY_SPACE))
            {
                Shoot(pos_x, pos_y, width);
            }

            UpdateBullets();

            Rectangle playerRect = { (float)pos_x, (float)pos_y, (float)width, (float)height };

            // Enemy Logic
            for (int i = 0; i < activeEnemies; i++)
            {
                enemies[i].rect.y += enemies[i].speed * x * 60;

                if (enemies[i].rect.y > height_sc)
                {
                    lives--;
                    RespawnEnemy(i);
                }

                if (CheckCollisionRecs(playerRect, enemies[i].rect))
                {
                    lives--;
                    RespawnEnemy(i);
                    pos_x = width_sc / 2;
                }

                // Bullet Collision
                for (int b = 0; b < Bmax; b++)
                {
                    if (bulletStatus[b])
                    {
                        Rectangle bulletRect = { bulletsX[b] - 2, bulletsY[b] - 5, 5, 10 };

                        if (CheckCollisionRecs(bulletRect, enemies[i].rect))
                        {
                            score += enemies[i].scoreValue;
                            bulletStatus[b] = false;
                            RespawnEnemy(i);
                            break;
                        }
                    }
                }
            }

            // Level Up
            if (score >= current_level * 200)
            {
                current_level++;

                activeEnemies += 4;
                if (activeEnemies > Ecount) activeEnemies = Ecount;

                for (int i = 0; i < activeEnemies; i++)
                {
                    enemies[i].speed += 0.5;
                }
            }
        }

        // ---------------------- DRAWING ----------------------
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, width_sc, Header_h, DARKGRAY);
        DrawLine(0, Header_h, width_sc, Header_h, RAYWHITE);

        DrawText("SPACE SHOOTER", 10, 15, 20, RAYWHITE);
        DrawText("LEVEL", width_sc / 4 + 50, 15, 20, LIGHTGRAY);
        DrawText(TextFormat("%i", current_level), width_sc / 4 + 130, 15, 20, YELLOW);
        DrawText("SCORE", width_sc / 2 + 100, 15, 20, LIGHTGRAY);
        DrawText(TextFormat("%05i", score), width_sc / 2 + 180, 15, 20, WHITE);
        DrawText("LIVES", width_sc - 150, 15, 20, LIGHTGRAY);
        DrawText(TextFormat("%i", lives), width_sc - 70, 15, 20, RED);

        if (gState == 0)
        {
            DrawText("INSTRUCTIONS", width_sc / 2 - 200, height_sc / 2 - 100, 40, WHITE);
            DrawText("Use A/D to move", width_sc / 2 - 200, height_sc / 2, 20, LIGHTGRAY);
            DrawText("Press SPACE to Fire", width_sc / 2 - 200, height_sc / 2 + 30, 20, LIGHTGRAY);
            DrawText("Hit Debris for Score (Colored = Higher Value)", width_sc / 2 - 200, height_sc / 2 + 60, 20, LIGHTGRAY);
            DrawText("Press SPACE to Start Level 1", width_sc / 2 - 200, height_sc / 2 + 150, 30, GREEN);
        }
        else if (gState == 1)
        {
            DrawRectangle(pos_x, pos_y, width, height, GREEN);

            for (int i = 0; i < Bmax; i++)
            {
                if (bulletStatus[i])
                {
                    DrawCircle(bulletsX[i], bulletsY[i], 3, RAYWHITE);
                }
            }

            for (int i = 0; i < activeEnemies; i++)
            {
                DrawRectangleRec(enemies[i].rect, enemies[i].color);
            }
        }
        else if (gState == 2)
        {
            DrawText("GAME OVER", width_sc / 2 - 150, height_sc / 2 - 50, 60, RED);
            DrawText(TextFormat("FINAL SCORE: %05i", score), width_sc / 2 - 150, height_sc / 2 + 50, 30, YELLOW);
            DrawText("Press R to Restart", width_sc / 2 - 150, height_sc / 2 + 100, 30, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
