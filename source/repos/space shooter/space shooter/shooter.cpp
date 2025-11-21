#include "raylib.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int width_sc = 1000;
const int height_sc = 600;
const int Header_h = 50;
const int start = Header_h;

// BULLETS
const int Bmax = 100;
int bulletsX[Bmax] = { 0 };
int bulletsY[Bmax] = { 0 };
bool bulletStatus[Bmax] = { false };
int bullet_idx = 0;
int bulletSpeed = 8;

// ENEMIES
const int Ecount = 15;
struct Enemy {
    Rectangle rect;
    float speed;
    bool active;
    Color color;
    int scoreValue;
};
Enemy enemies[Ecount]; // Array of enemies

// BOSS
Rectangle bossRect = { width_sc / 2 - 50, 100, 120, 60 };       //boss shape
int bossHP = 50;
float bossSpeed = 3;
bool bossDirectionRight = true;


const int BOSSBMAX = 10;
Rectangle bossBullets[BOSSBMAX];                                 //boss fight bullets
bool bossBulletActive[BOSSBMAX] = { false };
float bossBulletSpeed = 5;


int score = 0;
int lives = 3;                                          //player variables
int current_level = 1;
int gState = 0;

int pos_x = width_sc / 2;
int pos_y = height_sc - 60;
int width = 45;
int height = 20;
int playerSpeed = 10;

void Shoot(int playerX, int playerY, int pWidth);
void UpdateBullets();
void ResetGame();
void InitializeEnemies();
void RespawnEnemy(int i);
void UpdateBoss();
void BossShoot();
void UpdateBossBullets();

void Shoot(int playerX, int playerY, int Width) {
    bulletsX[bullet_idx] = playerX + Width / 2;
    bulletsY[bullet_idx] = playerY;
    bulletStatus[bullet_idx] = true;
    bullet_idx++;

    if (bullet_idx >= Bmax)
    {
        bullet_idx = 0;                                     //reset the bullets when greater than max or equal to it
    }
}

void UpdateBullets() {
    for (int i = 0; i < Bmax; i++) {
        if (bulletStatus[i]) {
            bulletsY[i] -= bulletSpeed;                                     //shooted in vertical direction
            if (bulletsY[i] < 0)
            {
                bulletStatus[i] = false;
            }
        }
    }
}


void RespawnEnemy(int i) {
    enemies[i].rect.y = start - enemies[i].rect.height - (rand() % 500);
    enemies[i].rect.x = rand() % (width_sc - (int)enemies[i].rect.width);
}

void InitializeEnemies() {
    srand(time(0));        //seed 
    int spawnset = 0;

    for (int i = 0; i < Ecount; i++) {

        enemies[i].rect.width = 30;                     //enemies dimensions
        enemies[i].rect.height = 30;
        enemies[i].speed = 0.5 + (current_level * 0.5);
        enemies[i].active = true;

        enemies[i].rect.x = rand() % (width_sc - (int)enemies[i].rect.width + 1);

        enemies[i].rect.y = Header_h - enemies[i].rect.height - (20 + rand() % 100);
        enemies[i].rect.y -= spawnset;

        spawnset += 80;

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



void ResetGame() {
    score = 0;
    lives = 3;                                              //when gameover the variables reset
    current_level = 1;
    pos_x = width_sc / 2;
    pos_y = height_sc - 60;

    bullet_idx = 0;

    for (int i = 0; i < Bmax; i++)
    {
        bulletStatus[i] = false;
    }
    bossHP = 50;
    bossRect = { width_sc / 2 - 50, 100, 120, 60 };

    InitializeEnemies();
}

void UpdateBoss() {
    if (bossDirectionRight) {
        bossRect.x += bossSpeed;
        if (bossRect.x + bossRect.width >= width_sc)
        {
            bossDirectionRight = false;
        }
    }
    else {
        bossRect.x -= bossSpeed;
        if (bossRect.x <= 0)
        {
            bossDirectionRight = true;
        }
    }

    if (rand() % 41 == 0)
    {
        BossShoot();
    }
}

void BossShoot() {
    for (int i = 0; i < BOSSBMAX; i++) {
        if (!bossBulletActive[i]) {
            bossBullets[i] = { bossRect.x + bossRect.width / 2 - 5, bossRect.y + bossRect.height, 10, 15 };
            bossBulletActive[i] = true;
            break;
        }
    }
}

void UpdateBossBullets() {
    for (int i = 0; i < BOSSBMAX; i++) {
        if (bossBulletActive[i]) {
            bossBullets[i].y += bossBulletSpeed;

            if (bossBullets[i].y > height_sc)
                bossBulletActive[i] = false;

            Rectangle playerRect = { pos_x, pos_y, width, height };

            if (CheckCollisionRecs(playerRect, bossBullets[i]))
            {
                lives--;
                bossBulletActive[i] = false;
            }
        }
    }
}

//main loop GUI
int main() {
    InitWindow(width_sc, height_sc, "SPACE SHOOTER!");
    SetTargetFPS(60);
    Texture2D playerTexture = LoadTexture("assets/extra.png");
    Texture2D enemyTexture = LoadTexture("assets/red.png");
    Texture2D enemy2Texture = LoadTexture("assets/yellow.png");
    Texture2D enemy3Texture = LoadTexture("assets/green.png");
	Texture2D bossTexture = LoadTexture("assets/boss.png");
    InitializeEnemies();
    srand(time(0));

    while (!WindowShouldClose()) {
        float x = GetFrameTime();        //frame rate

        if (gState == 0) {
            if (IsKeyPressed(KEY_SPACE)) gState = 1;
        }

        if (lives <= 0)
        {
            gState = 2;
        }
        if (IsKeyPressed(KEY_R) && (gState == 1 || gState == 2 || gState == 3)) {
            ResetGame();
            gState = 1;
        }

        if (gState == 1) {

            // ------------ MOVEMENT ------------
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                pos_x += playerSpeed;
            }
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            {
                pos_x -= playerSpeed;
            }

            if (pos_x < 0)
            {
                pos_x = 0;
            }
            if (pos_x > width_sc - width)
            {
                pos_x = width_sc - width;
            }


            if (IsKeyPressed(KEY_SPACE))
            {
                Shoot(pos_x, pos_y, width);
            }
            UpdateBullets();

            Rectangle playerRect = { pos_x, pos_y, width, height };


            if (current_level < 3) {
                for (int i = 0; i < Ecount; i++) {
                    enemies[i].rect.y += enemies[i].speed * x * 50;

                    if (enemies[i].rect.y > height_sc) {
                        lives--;
                        RespawnEnemy(i);
                    }

                    if (CheckCollisionRecs(playerRect, enemies[i].rect)) {
                        lives--;
                        RespawnEnemy(i);
                    }

                    for (int b = 0; b < Bmax; b++) {
                        if (bulletStatus[b]) {
                            Rectangle bulletRect = { bulletsX[b] - 2, bulletsY[b] - 5, 5, 10 };

                            if (CheckCollisionRecs(bulletRect, enemies[i].rect)) {
                                score += enemies[i].scoreValue;
                                bulletStatus[b] = false;
                                RespawnEnemy(i);
                                break;
                            }
                        }
                    }
                }

                if (score >= current_level * 200) {
                    current_level++;
                    if (current_level == 3) {
                        // prepare boss
                    }
                }
            }


            if (current_level == 3) {
                UpdateBoss();                                //Boss Fight
                UpdateBossBullets();

                // Player bullets hit boss
                for (int b = 0; b < Bmax; b++) {
                    if (bulletStatus[b]) {
                        Rectangle bRect = { (float)bulletsX[b], (float)bulletsY[b], 5, 10 };
                        if (CheckCollisionRecs(bRect, bossRect)) {
                            bossHP--;
                            bulletStatus[b] = false;
                        }
                    }
                }

                if (bossHP <= 0) {
                    gState = 3; // WIN!
                }
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, width_sc, Header_h, DARKGRAY);
        DrawLine(0, Header_h, width_sc, Header_h, RAYWHITE);

        DrawText("SPACE SHOOTER", 10, 15, 20, RAYWHITE);
        DrawText("LEVEL", 250, 15, 20, LIGHTGRAY);
        DrawText(TextFormat("%i", current_level), 330, 15, 20, YELLOW);
        DrawText("SCORE", 500, 15, 20, LIGHTGRAY);
        DrawText(TextFormat("%05i", score), 580, 15, 20, WHITE);
        DrawText("LIVES", 820, 15, 20, LIGHTGRAY);
        DrawText(TextFormat("%i", lives), 900, 15, 20, RED);

        if (gState == 0) {
            DrawText("INSTRUCTIONS", width_sc / 2 - 200, 150, 40, WHITE);
            DrawText("Use A/D to move", 300, 300, 20, LIGHTGRAY);
            DrawText("Press SPACE to Fire", 300, 330, 20, LIGHTGRAY);
            DrawText("Hit Debris for Score", 300, 360, 20, LIGHTGRAY);
            DrawText("Press R to Start", 300, 390, 20, LIGHTGRAY);
            DrawText("Press ESC to Start", 300, 420, 20, LIGHTGRAY);

        }

        if (gState == 1) {

            // DRAW PLAYER
            Rectangle playerSrcRec = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };
            Rectangle playerDestRec = { (float)pos_x, (float)pos_y, (float)width, (float)height };
            DrawTexturePro(playerTexture, playerSrcRec, playerDestRec, { 0,0 }, 0.0f, WHITE);  //draw player ship

            // DRAW BULLETS
            for (int i = 0; i < Bmax; i++) {
                if (bulletStatus[i])
                    DrawRectangle(bulletsX[i], bulletsY[i], 5, 10, YELLOW);
            }

            if (current_level < 3) {
                for (int i = 0; i < Ecount; i++) {

                    if (enemies[i].rect.y >= Header_h + 5) {

                        
                        Texture2D* enemyTex = &enemy3Texture;

                        if (i % 3 == 0)
                        {
                            enemyTex = &enemyTexture; // Red enemy
                        }
                        else if (i % 3 == 1)
                        {
                            enemyTex = &enemy2Texture; // Yellow enemy
                        }
                        else 
                            enemyTex = &enemy3Texture; // Green enemy

                       
                        Rectangle enemySrcRec = { 0.0f, 0.0f, (float)enemyTex->width, (float)enemyTex->height };
                        DrawTexturePro(*enemyTex, enemySrcRec, enemies[i].rect, { 0,0 }, 0.0f, WHITE);
                     
                    }

                }
            }

            if (current_level == 3) {
				Rectangle bossSrcRec = { 0.0f, 0.0f, (float)bossTexture.width, (float)bossTexture.height };
				DrawTexturePro(bossTexture, bossSrcRec, bossRect, { 0,0 }, 0.0f, WHITE);

                for (int i = 0; i < BOSSBMAX; i++) {
                    if (bossBulletActive[i])
                        DrawRectangleRec(bossBullets[i], RED);
                }

                DrawText(TextFormat("BOSS HP: %i", bossHP), width_sc / 2 - 50, 60, 20, RED);
            }
        }

        if (gState == 2) {
            DrawText("GAME OVER!", width_sc / 2 - 150, height_sc / 2 - 50, 60, RED);
        }

        if (gState == 3) {
            DrawText("YOU WIN!", width_sc / 2 - 150, height_sc / 2 - 50, 60, GREEN);
        }

        EndDrawing();
    }
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(enemy2Texture);
    UnloadTexture(enemy3Texture);
	UnloadTexture(bossTexture);


    CloseWindow();
    return 0;
}