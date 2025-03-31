#include "raylib.h"
#include <cstdio>
#include <list>
#include <stdint.h>
#include <random>
// Custom header files start here.
#include "enemies.hpp"
#include "globalVariables.hpp"

constexpr auto cameraX = 300, cameraY = 400;

std::random_device randDevice;
std::mt19937 rng(randDevice());
std::uniform_int_distribution<unsigned char> randomColor(0, 0xFF);
std::uniform_real_distribution<float> randomX(0.0f, cameraX);

class PlayerBullet {
private:
	static constexpr float speed = 1000.0f;
public:
	static constexpr Vector2 size = {4.0f, 12.0f};
	Vector2 coords;
	void Draw()
	{
		DrawRectangleV(coords, size, ORANGE);
	}
	void Move(float deltaTime)
	{
		coords.y += speed * deltaTime;
	}
	PlayerBullet(Vector2 newCoords)
	{
		coords = newCoords;
		coords.x -= size.x / 2.0f;
	}
};

std::uniform_real_distribution<float> randomStarSpeed(5.0f, 10.0f);
class BgStar {
private:
	Color bgColor;
	float speed;
public:
	Vector2 coords;
	BgStar()
	{
		bgColor.r = randomColor(rng);
		bgColor.g = randomColor(rng);
		bgColor.b = randomColor(rng);
		bgColor.a = 0xFF;
		coords.y = cameraY;
		coords.x = randomX(rng);
		speed = randomStarSpeed(rng);
	}
	void Move(float deltaTime)
	{
		coords.y -= speed * deltaTime;
	}
	void Draw()
	{
		DrawPixelV(coords, bgColor);
	}
};

enum PlayerFrame {
	PLAYER_FRAME_NORMAL_1 = 0,
	PLAYER_FRAME_NORMAL_2 = 1
};

class Player {
private:
	enum PlayerFrame currentFrame = PLAYER_FRAME_NORMAL_1;
	float spriteTime = 0.0f;
	float hitInvulTime = 0.0f;
	float shootCooldown = 0.0f;
public:
	Rectangle hitbox = {0.0f, 0.0f, 6.0f, 10.0f};
	bool isHit = false;
	int lives = 5;
	Vector2 coords;
	std::list<PlayerBullet> bullets;
	void Draw(Texture2D *sprites, bool debug)
	{
		if (isHit) {
			DrawTexture(sprites[currentFrame], coords.x, coords.y, RED);
			if (debug) {
				DrawRectangleRec(hitbox, HITBOX_COLOR_ALT);
			}
		}
		else {
			DrawTexture(sprites[currentFrame], coords.x, coords.y, WHITE);
			if (debug) {
				DrawRectangleRec(hitbox, HITBOX_COLOR);
			}
		}
	}
	void ChangeSprite(float deltaTime)
	{
		constexpr static float changeTime = 1.0 / (sizeof (PlayerFrame) / sizeof PLAYER_FRAME_NORMAL_1) / 2.0f;
		spriteTime += deltaTime;
		if (spriteTime >= changeTime) {
			spriteTime = 0.0f;
			if (currentFrame == PLAYER_FRAME_NORMAL_1)
				currentFrame = PLAYER_FRAME_NORMAL_2;
			else if (currentFrame == PLAYER_FRAME_NORMAL_2)
				currentFrame = PLAYER_FRAME_NORMAL_1;
		}
	}
	void Move(float deltaTime)
	{
		float speed = 100.0f;
		if (IsKeyDown(KEY_UP) && (hitbox.y + hitbox.height) < cameraY)
			coords.y += speed * deltaTime;
		else if (IsKeyDown(KEY_DOWN) && hitbox.y > 0.0f)
			coords.y -= speed * deltaTime;
		if (IsKeyDown(KEY_RIGHT) && (hitbox.x + hitbox.width) < cameraX)
			coords.x += speed * deltaTime;
		else if (IsKeyDown(KEY_LEFT) && hitbox.x > 0.0f)
			coords.x -= speed * deltaTime;

		// Hitbox is realigned after movement.
		constexpr float xOffset = 13.0, yOffset = 12.0f;
		hitbox.x = coords.x + xOffset;
		hitbox.y = coords.y + yOffset; 
	}
	void Fire(float deltaTime, uint64_t *score)
	{
		if (IsKeyDown(KEY_Z) && shootCooldown <= 0.0f) {
			// Limits playe to only 4 shots per second.
			shootCooldown = 0.25f;
			// Construct class member at the end of list.
			bullets.emplace_back((Vector2) {hitbox.x + (hitbox.width / 2.0f), hitbox.y});
			// Punishing player for spamming shot button
			if(*score > 0) *score -= 1;
		}
		else {
			if (shootCooldown > 0.0f) shootCooldown -= deltaTime;
		}
	}
	void MoveAllBullets(float deltaTime)
	{
		std::list<PlayerBullet>::iterator it;
		for (it = bullets.begin(); it != bullets.end(); ) {
			it->Move(deltaTime);
			if (it->coords.y > cameraY)
				bullets.erase(it++);
			else it++;
		}
	}
	void DrawAllBullets()
	{
		std::list<PlayerBullet>::iterator it;
		for (it = bullets.begin(); it != bullets.end(); ++it)
			it->Draw();
	}
	void WaitInvul(float deltaTime)
	{
		hitInvulTime += deltaTime;
		static constexpr float hitTimer = 1.0f;
		if (hitInvulTime >= hitTimer) {
			isHit = false;
			hitInvulTime = 0.0f;
		}
	}
	void Hit()
	{
		lives--;
		isHit = true;
	}
};

std::uniform_real_distribution<float> randomSpeedXMeteoroid(-20.0f, 20.0f);
std::uniform_real_distribution<float> randomSpeedYMeteoroid(-100.0f, -50.0f);
void events(float deltaTime, std::list<Meteoroid> *meteoroid, std::list<EnemyUFO> *ufos)
{
	static unsigned long seconds = 0;
	static float time = 0.0f;
	time += deltaTime;
	if (time >= 1.0f) {
		time = 0.0f;
		seconds++;
		// Should spawn enemies every X seconds.
		if (seconds % 3 == 0)
			meteoroid->emplace_back((Vector2){randomX(rng), cameraY}, randomSpeedXMeteoroid(rng), randomSpeedYMeteoroid(rng));
		if (seconds % 5 == 0) ufos->emplace_back(randomX(rng), cameraY);
	}
}

class Explosion {
private:
	float time = 0.0f;
	int x, y;
public:
	int index = 0;
	void Wait (float deltaTime, const int arraySize)
	{
		constexpr float cooldown = 0.05f;
		time += deltaTime;
		if (time >= cooldown) {
			time = 0.0f;
			index++;
			// Meaning that explosion is about to be removed from memory.
			if (index >= arraySize) index = -1;
		}
	}
	void Draw(Texture2D *spriteArray)
	{
		DrawTexture(spriteArray[index], x, y, WHITE);
	}
	Explosion(Vector2 coords)
	{
		x = (int) coords.x;
		y = (int) coords.y;
	}
};

void gameOver(uint64_t *score, Player *player)
{
	*score = 0;
	player->lives = 5;
}

int main(void)
{
	std::list<EnemyBullet> enemyBullets;
	uint64_t score = 0;
	constexpr uint64_t maxScore = 999'999'999'999;
	InitWindow(640, 480, "vshooter");

	Camera2D camera;
	camera.target = {0.0, 0.0};
	camera.offset = {0.0, 0.0};
	camera.rotation = 0.0;
	camera.zoom = 1.0;

	constexpr int padding = 40;
	RenderTexture camTexture = LoadRenderTexture(cameraX, cameraY);

	Font titleFont = LoadFont("titlefont.fnt");
	Font scoreFont = LoadFontEx("fantasque.ttf", 32, NULL, 0xFFFF);
	Vector2 scoreSize = MeasureTextEx(scoreFont, "000000000000", 32.0, 0.0);

	Player player;
	player.coords = {150.0, 200.0};
	float shootCooldown = 0.0f;
	Texture2D playerSprites[] = {LoadTexture("graphics/player-1.png"), LoadTexture("graphics/player-2.png")};
	int lives = 5;
	bool debug = false;
	bool mainMenu = true;

	bool playerBulletCollission = false;
	float bgStarCooldown = 0.0f;
	std::list<BgStar> stars;

	Texture2D ufoEnemySprites[] = {LoadTexture("graphics/ufo-normal1.png"), LoadTexture("graphics/ufo-normal2.png")};
	std::list<EnemyUFO> saucers;

	Texture2D meteoroidSprite = LoadTexture("graphics/meteoroid-1.png");
	std::list<Meteoroid> meteoroids;
	Meteoroid exampleMeteoroid = Meteoroid({100.0f, 100.0f}, 0.0f, 0.0f);

	Texture2D explosionSprites[] = {
		LoadTexture("graphics/explosion_1.png"),
		LoadTexture("graphics/explosion_2.png"),
		LoadTexture("graphics/explosion_3.png"),
		LoadTexture("graphics/explosion_4.png"),
		LoadTexture("graphics/explosion_5.png"),
		LoadTexture("graphics/explosion_6.png"),
		LoadTexture("graphics/explosion_7.png"),
		LoadTexture("graphics/explosion_8.png"),
		LoadTexture("graphics/explosion_9.png"),
		LoadTexture("graphics/explosion_10.png"),
		LoadTexture("graphics/explosion_11.png"),
		LoadTexture("graphics/explosion_12.png"),
		LoadTexture("graphics/explosion_13.png"),
		LoadTexture("graphics/explosion_14.png"),
		LoadTexture("graphics/explosion_15.png"),
		LoadTexture("graphics/explosion_16.png"),
		LoadTexture("graphics/explosion_17.png")
	};
	constexpr int explosionSpritesAmount = sizeof explosionSprites / sizeof explosionSprites[0];
	std::list<Explosion> explosions;

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		if (mainMenu) {
			Vector2 titleSize = MeasureTextEx(titleFont, "VShooter", 32.0f, 0.0f);
			int pressEnterSize = MeasureText("Press \"ENTER\" to begin.", 32);
			if (IsKeyReleased(KEY_ENTER)) mainMenu = false;
			BeginDrawing();
			{
				ClearBackground(BLACK);
				// Assuming it is 640x480.
				DrawTextEx(titleFont, "VShooter", {320.0f - titleSize.x / 2.0f, 200.0f}, 32.0f, 0.0f, WHITE);
				DrawText("Press \"ENTER\" to begin.", 320 - pressEnterSize / 2, 400, 32, WHITE);
			}
			EndDrawing();
		}
		else {
			// Time passed between frames.
			// Can be used to make things more consistant between frames.
			float deltaTime = GetFrameTime();
			player.ChangeSprite(deltaTime);
			// Player movement, restricted by borders of screen
			player.Move(deltaTime);
			player.Fire(deltaTime, &score);
			// Moving bullets. They are removed if they move outside window.
			player.MoveAllBullets(deltaTime);
		
			if (IsKeyReleased(KEY_TAB)) debug = !debug;
			events(deltaTime, &meteoroids, &saucers);

			std::list<Explosion>::iterator blast_it;
			for (blast_it = explosions.begin(); blast_it != explosions.end(); ) {
				blast_it->Wait(deltaTime, explosionSpritesAmount);
				if (blast_it->index == -1) explosions.erase(blast_it++);
				else blast_it++;
			}

			std::list<PlayerBullet>::iterator pbullets_it;
			std::list<EnemyUFO>::iterator ufo_it;
			for (ufo_it = saucers.begin(); ufo_it != saucers.end(); ) {
				ufo_it->Move(deltaTime);
				if (ufo_it->CollisionCheck(player.hitbox) && !player.isHit) player.Hit();
				// Collision checks for player's bullets.
				for (pbullets_it = player.bullets.begin(); pbullets_it != player.bullets.end(); pbullets_it++) {
					if (ufo_it->CollisionCheck({pbullets_it->coords.x, pbullets_it->coords.y, pbullets_it->size.x, pbullets_it->size.y})) {
						playerBulletCollission = true;
						player.bullets.erase(pbullets_it++);
						break;
					}
				}
				if (playerBulletCollission) {
					playerBulletCollission = false;
					explosions.emplace_back(ufo_it->coords);
					if (score < maxScore - 200) score += 200;
					else score = maxScore;
					saucers.erase(ufo_it++);
					continue;
				}
				ufo_it->Shoot(deltaTime, &enemyBullets);
				ufo_it->ChangeSprite(deltaTime);
				if (ufo_it->hitbox.y + ufo_it->hitbox.height <= 0.0f) saucers.erase(ufo_it++);
				else ufo_it++;
			}

			std::list<Meteoroid>::iterator meteor_it;
			for (meteor_it = meteoroids.begin(); meteor_it != meteoroids.end(); ) {
				meteor_it->Move(deltaTime);
				if (meteor_it->CollisionCheck(player.hitbox) && !player.isHit) player.Hit();
				// Player bullet collsion checks.
				for (pbullets_it = player.bullets.begin(); pbullets_it != player.bullets.end(); pbullets_it++) {
					if (meteor_it->CollisionCheck((Rectangle){pbullets_it->coords.x, pbullets_it->coords.y, pbullets_it->size.x, pbullets_it->size.y})) {
						playerBulletCollission = true;
						player.bullets.erase(pbullets_it++);
						break;
					}
				}
				if (playerBulletCollission) {
					playerBulletCollission = false;
					explosions.emplace_back((Vector2){meteor_it->coords.x - meteor_it->radius, meteor_it->coords.y - meteor_it->radius});
					if (score < maxScore - 100) score += 100;
					else score = maxScore;
					meteoroids.erase(meteor_it++);
					continue;
				}
				if (meteor_it->coords.y + meteor_it->radius <= 0.0f) meteoroids.erase(meteor_it++);
				else meteor_it++;
			}

			if (bgStarCooldown >= 0.5f) {
				stars.emplace_back();
				bgStarCooldown = 0.0f;
			}
			else bgStarCooldown += deltaTime;
			std::list<BgStar>::iterator it;
			for (it = stars.begin(); it != stars.end(); ) {
				it->Move(deltaTime);
				if (it->coords.y <= 0.0f)
					stars.erase(it++);
				else it++;
			}
			std::list<EnemyBullet>::iterator ebullets_it;
			for (ebullets_it = enemyBullets.begin(); ebullets_it != enemyBullets.end(); ) {
				ebullets_it->Move(deltaTime);
				if (ebullets_it->CollisionCheck(player.hitbox) && !player.isHit) player.Hit();
				if (ebullets_it->coords.y + ebullets_it->size.y <= 0.0f)
					enemyBullets.erase(ebullets_it++);
				else ebullets_it++;
			}
			// Invulnerability time
			player.WaitInvul(deltaTime);

			if (player.lives < 0) {
				gameOver(&score, &player);
				mainMenu = true;
			}
			char scoreString[13];
			std::snprintf(scoreString, 13, "%012llu", score);

			BeginTextureMode(camTexture);
			{
				// It should be like a pretty dark shade of blue.
				ClearBackground({6, 6, 61, 255});
				BeginMode2D(camera);
				for (it = stars.begin(); it != stars.end(); ++it)
					it->Draw();
				// Player
				player.Draw(playerSprites, debug);
				// UFO
				for (ufo_it = saucers.begin(); ufo_it != saucers.end(); ++ufo_it)
					ufo_it->Draw(ufoEnemySprites, debug);
				// Meteoroid
				for (meteor_it = meteoroids.begin(); meteor_it != meteoroids.end(); ++meteor_it)
					meteor_it->Draw(meteoroidSprite, debug);
				// Explosions
				for (blast_it = explosions.begin(); blast_it != explosions.end(); ++blast_it)
					blast_it->Draw(explosionSprites);
				// Player's bullets
				player.DrawAllBullets();
				// Enemy's bullets
				for (ebullets_it = enemyBullets.begin(); ebullets_it != enemyBullets.end(); ++ebullets_it)
					ebullets_it->Draw();
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
				DrawTextEx(scoreFont, u8"Очки", {xScorePadding, yScorePadding - padding}, 32.0f, 0.0f, WHITE);
				DrawRectangleV({xScorePadding, yScorePadding}, scoreSize, BLACK);
				DrawTextEx(scoreFont, scoreString, {xScorePadding, yScorePadding}, 32.0f, 0.0f, WHITE);
				// Lives
				DrawTextEx(scoreFont, u8"Жизни", {xScorePadding, padding * 4}, 32.0f, 0.0f, WHITE);
				for (int i = 0; i < player.lives; i++)
					DrawTexture(playerSprites[0], xScorePadding + padding * i, padding * 5, WHITE);
			}
			EndDrawing();
		}
	}
	// Quiting the program
	UnloadFont(scoreFont);
	UnloadFont(titleFont);
	UnloadTexture(meteoroidSprite);
	for (int i = 0; i < sizeof playerSprites / sizeof playerSprites[0]; i++)
		UnloadTexture(playerSprites[i]);
	for (int i = 0; i < sizeof ufoEnemySprites / sizeof ufoEnemySprites[0]; i++)
		UnloadTexture(ufoEnemySprites[i]);
	UnloadRenderTexture(camTexture);
	CloseWindow();
	return 0;
}
