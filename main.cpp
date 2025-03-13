#include "raylib.h"
#include <cstdio>
#include <list>
#include <stdint.h>

class PlayerBullet
{
private:
	static constexpr float speed = 10.0f;
	static constexpr Vector2 size = {10.0f, 30.0f}; 
public:
	Vector2 coords;
	void Draw()
	{
		DrawRectangleV(coords, size, ORANGE);
	}
	void Move()
	{
		coords.y += speed;
	}
	PlayerBullet(Vector2 newCoords)
	{
		coords = newCoords;
	}
};

int main(void)
{
	uint64_t score = 0;
	constexpr uint64_t maxScore = 999'999'999'999;
	InitWindow(640, 480, "vshooter");

	Camera2D camera;
	camera.target = {0.0, 0.0};
	camera.offset = {0.0, 0.0};
	camera.rotation = 0.0;
	camera.zoom = 1.0;
	constexpr auto cameraX = 300, cameraY = 400;
	constexpr int padding = 40;
	RenderTexture camTexture = LoadRenderTexture(cameraX, cameraY);

	Font titleFont = LoadFont("titlefont.fnt");
	Font scoreFont = LoadFont("fantasque.ttf");
	Vector2 scoreSize = MeasureTextEx(scoreFont, "000000000000", 32.0, 0.0);

	Vector2 player = {150.0, 200.0};
	std::list<PlayerBullet> playerBullets;
	float shootCooldown = 0.0f;

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// Time passed between frames.
		// Can be used to make things more consistant between frames.
		float deltaTime = GetFrameTime();

		// Player movement, restricted by borders of screen
		if (IsKeyDown(KEY_UP) && player.y < cameraY)
			player.y += 100.0f * deltaTime;
		else if (IsKeyDown(KEY_DOWN) && player.y > 0.0f)
			player.y -= 100.0f * deltaTime;
		if (IsKeyDown(KEY_RIGHT) && player.x < cameraX)
			player.x += 100.0f * deltaTime;
		else if (IsKeyDown(KEY_LEFT) && player.x > 0.0f)
			player.x -= 100.0f * deltaTime;

		// Shooting. Is limited by arbitrary cooldown.
		if (IsKeyDown(KEY_Z) && shootCooldown <= 0.0f) {
			// Limits playe to only 4 shots per second.
			shootCooldown = 0.25f;
			// TODO: it is slightly off-centered.
			playerBullets.emplace_back(player);
		}
		else {
			if (shootCooldown > 0.0f) shootCooldown -= deltaTime;
		}

		// Moving bullets. They are removed if they move outside window.
		std::list<PlayerBullet>::iterator it;
		for (it = playerBullets.begin(); it != playerBullets.end(); ) {
			it->Move();
			if (it->coords.y > cameraY)
				playerBullets.erase(it++);
			else it++;
		}

		if (score < maxScore) score++;
		char scoreString[13];
		std::snprintf(scoreString, 13, "%012llu", score);

		BeginTextureMode(camTexture);
		{
			ClearBackground(BLACK);
			BeginMode2D(camera);
			// Player
			DrawCircleV(player, 30.0f, WHITE);
			// Bullets
			for (it = playerBullets.begin(); it != playerBullets.end(); ++it)
				it->Draw();
			EndMode2D();
		}
		EndTextureMode();

		BeginDrawing();
		{
			ClearBackground(WHITE);
			// Background
			DrawRectangleGradientV(0, 0, 640, 480, RED, PINK);
			// Title
			DrawTextEx(titleFont, "VShooter", {padding * 3 + cameraX, padding}, 42.0, 0.0, WHITE);
			// Game screen
			DrawTexture(camTexture.texture, padding, padding, WHITE);
			// Score
			constexpr float xScorePadding = padding * 3 + cameraX;
			constexpr float yScorePadding = padding * 3;
			DrawRectangleV({xScorePadding, yScorePadding}, scoreSize, BLACK);
			DrawTextEx(scoreFont, scoreString, {xScorePadding, yScorePadding}, 32.0f, 0.0f, WHITE);
		}
		EndDrawing();
	}
	UnloadRenderTexture(camTexture);
	CloseWindow();
	return 0;
}
