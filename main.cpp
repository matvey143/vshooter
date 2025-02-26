#include "raylib.h"
#include <cstdio>

int main(void)
{
	int score = 10;
	InitWindow(640, 480, "vshooter");

	Camera2D camera;
	camera.target = (Vector2) {0.0, 0.0};
	camera.offset = (Vector2) {0.0, 0.0};
	camera.rotation = 0.0;
	camera.zoom = 1.0;
	constexpr int cameraX = 300, cameraY = 400;
	constexpr int padding = 40;
	RenderTexture camTexture = LoadRenderTexture(cameraX, cameraY);

	Font titleFont = LoadFont("titlefont.fnt");

	Vector2 player = {150.0, 200.0}; 

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// Time passed between frames.
		// Can be used to make things more consistant between frames.
		float deltaTime = GetFrameTime();
		// Player movement, restricted by borders of screen
        	if (IsKeyDown(KEY_UP) && player.y < (float) cameraY)
			player.y += 100.0f * deltaTime;
        	else if (IsKeyDown(KEY_DOWN) && player.y > 0.0f)
			player.y -= 100.0f * deltaTime;
        	if (IsKeyDown(KEY_RIGHT) && player.x < (float) cameraX)
			player.x += 100.0f * deltaTime;
        	else if (IsKeyDown(KEY_LEFT) && player.x > 0.0f)
			player.x -= 100.0f * deltaTime;

		score++;
		char scoreString[12];
		std::snprintf(scoreString, 12, "%011d", score);
		BeginTextureMode(camTexture);
		{
			ClearBackground(BLACK);
			BeginMode2D(camera);
			DrawCircleV(player, 30.0f, WHITE);
			EndMode2D();
		}
		EndTextureMode();
		BeginDrawing();
		{
			ClearBackground(WHITE);
			// Background
			DrawRectangleGradientV(0, 0, 640, 480, RED, PINK);
			// Title
			DrawTextEx(titleFont, "VShooter", 
					(Vector2){padding * 3 + cameraX, padding},
					42.0, 0.0, WHITE);
			// Game screen
			DrawTexture(camTexture.texture, padding, padding, WHITE);
			// Score
			// TODO: I will use monowide font and scale black box.
			DrawRectangle(padding * 3 + cameraX, padding * 3,
					padding * 5, padding, BLACK);
			DrawText(scoreString, padding * 3 + cameraX, padding * 3,
					32, WHITE);
		}
		EndDrawing();
	}
	UnloadRenderTexture(camTexture);
	CloseWindow();
	return 0;
}
