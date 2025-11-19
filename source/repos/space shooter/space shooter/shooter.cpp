#include <iostream>
#include<conio.h>
#include "raylib.h"
const int MaxB = 100;
float bulletsX[MaxB] = { 0 };
float bulletsY[MaxB] = { 0 };
bool bulletStatus[MaxB] = { false };
int bullet_idx = 0;
float bulletSpeed = 6;
void shoot(int Player_x, int Player_Y, int width)
{
	bulletsX[bullet_idx] = Player_x + width / 2;
	bulletsY[bullet_idx] = Player_Y;
	bulletStatus[bullet_idx] = true;
	bullet_idx++;
	if (bullet_idx >= MaxB)
	{
		bullet_idx = 0;
	}
}
void updateBullets(float bulletsY[], float bulletSpeed, int maxBullets, bool bulletStatus[])
{
	int i;
	for (i = 0; i < MaxB; i++)
	{
		if (bulletStatus[i])
		{
			bulletsY[i] = bulletsY[i] - bulletSpeed;
		}
		if (bulletsY[i] < 0)
		{
			bulletStatus[i] = false;
		}

	}
}

using namespace std;
int main() {
	char key;
	const int widthsc = 1000;
	const int heightsc = 600;
	InitWindow(widthsc, heightsc, "BOOM!WELCOME TO SPACE SHOOTER");
	SetTargetFPS(60);
	int pos_x = widthsc / 2;
	int pos_y = heightsc - 60;
	int width = 45;
	int height = 20;
	int enpos_x = rand() % 600 + 1;
	int enpos_2x = rand() % 600 + 1;
	int enpos_3x = rand() % 600 + 1;
	int enpos_2y = -20;
	int enpos_3y = -40;

	int enpos_y = 0;

	while (WindowShouldClose() != true) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(pos_x, pos_y, width, height, RED);
		DrawRectangle(enpos_x, enpos_y, width, height, BLACK);
		DrawRectangle(enpos_x + 70, enpos_2y, width, height, BLUE);
		DrawRectangle(enpos_x + 2 * 70, enpos_3y, width, height, RED);
		float x = GetFrameTime();

		if (IsKeyDown(KEY_D))
		{
			pos_x += 5;
		}
		if (IsKeyDown(KEY_A))
		{
			pos_x -= 5;
		}

		if (pos_x < 0)
		{
			pos_x = 0;
		}
		if (pos_x > 900)
		{
			pos_x = 900;
		}
		if (pos_y < 0)
		{
			pos_y = 0;
		}
		if (pos_y > 550)
		{
			pos_y = 550;
		}	   enpos_y += 200 * x;
		if (enpos_y > heightsc) {
			enpos_y = 20;
		}
		enpos_2y += 200 * x;
		if (enpos_2y > heightsc) {
			enpos_2y = 70;
		}
		enpos_3y += 200 * x;
		if (enpos_3y > heightsc) {
			enpos_3y = 120;
		}
		for (int i = 0; i < MaxB; i++)
		{
			if (bulletStatus[i])
			{
				DrawCircle(bulletsX[i], bulletsY[i], 5, RED);
			}
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			shoot(pos_x, pos_y, width);
		}
		updateBullets(bulletsY, bulletSpeed, MaxB, bulletStatus);


		EndDrawing();
	}
	CloseWindow();
	return 0;
}
