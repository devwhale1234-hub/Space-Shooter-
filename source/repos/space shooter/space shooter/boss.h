#include<iostream>
#include"raylib.h"
using namespace std;
Rectangle bossRect = { width_sc / 2 - 50, 100, 120, 60 };       //boss shape
int bossHP = 50;
float bossSpeed = 3;
bool bossDirectionRight = true;


const int BOSSBMAX = 10;
Rectangle bossBullets[BOSSBMAX];                                 //boss fight bullets
bool bossBulletActive[BOSSBMAX] = { false };
float bossBulletSpeed = 5;
void BossShoot() {
	for (int i = 0; i < BOSSBMAX; i++) {
		if (!bossBulletActive[i]) {
			bossBullets[i] = { bossRect.x + bossRect.width / 2 - 5, bossRect.y + bossRect.height, 10, 15 };
			bossBulletActive[i] = true;
			break;
		}
	}
}
void UpdateBoss() {
	// Move boss left and right
	if (bossDirectionRight) {
		bossRect.x += bossSpeed;
		if (bossRect.x + bossRect.width >= width_sc) {
			bossDirectionRight = false;
		}
	}
	else {
		bossRect.x -= bossSpeed;
		if (bossRect.x <= 0) {
			bossDirectionRight = true;
		}
	}
	// Boss shooting logic
	 int shootTimer = 0;
	shootTimer++;
	if (shootTimer >= 60) { // Shoot every 60 frames
		for (int i = 0; i < BOSSBMAX; i++) {
			if (!bossBulletActive[i]) {
				bossBullets[i] = { bossRect.x + bossRect.width / 2 - 2, bossRect.y + bossRect.height, 5, 10 };
				bossBulletActive[i] = true;
				break;
			}
		}
		shootTimer = 0;
	}
}
