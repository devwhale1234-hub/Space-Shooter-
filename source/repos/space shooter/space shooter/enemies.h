#include<iostream>
#include"raylib.h"
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
void RespawnEnemy(int i) {
    enemies[i].rect.y = start - enemies[i].rect.height - (rand() % 500);
    enemies[i].rect.x = rand() % (width_sc - (int)enemies[i].rect.width);
}

void InitializeEnemies() {
    srand(time(0));       //seed 
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