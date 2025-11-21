#include<iostream>
#include"raylib.h"
using namespace std;
const int Bmax = 100;
int bulletsX[Bmax] = { 0 };
int bulletsY[Bmax] = { 0 };
bool bulletStatus[Bmax] = { false };
int bullet_idx = 0;
int bulletSpeed = 8;

void Shoot(int playerX, int playerY, int Width) {
    bulletsX[bullet_idx] = playerX + Width / 2;
    bulletsY[bullet_idx] = playerY;
    bulletStatus[bullet_idx] = true;
    bullet_idx++;

    if (bullet_idx >= Bmax)
    {
        bullet_idx = 0;                             //reset the bullets when greater than max or equal to it
    }
}

void UpdateBullets() {
    for (int i = 0; i < Bmax; i++) {
        if (bulletStatus[i]) {
            bulletsY[i] -= bulletSpeed;                             //shooted in vertical direction
            if (bulletsY[i] < 0)
            {
                bulletStatus[i] = false;
            }
        }
    }
}

