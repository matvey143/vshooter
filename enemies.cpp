#include "raylib.h"
#include <list>
#include "globalVariables.hpp"

class Meteroid {
private:
	float vSpeed = -200.0f;
	float hSpeed = 0.0f;
	float radius = 16.0f;
public:
	Vector2 coords;
	void Draw(Texture2D texture)
	{
		DrawTexture(texture, coords.x - radius, coords.y - radius, WHITE);
		if (debug) {
			DrawCircleV(coords, radius, hitboxColor);
		}
	}
	void Move(float deltaTime)
	{
		coords.x += hSpeed * deltaTime;
		coords.y += vSpeed * deltaTime;
	}
	bool CollisionCheck(Rectangle player)
	{
		return CheckCollisionCircleRec(coords, radius, player);
	}
};

class EnemyBullet {
private:
	float speed = 250.0f;
	Color color = GREEN;
public:
	Vector2 size = {4.0f, 12.0f};
	Vector2 coords;
	void Draw()
	{
		DrawRectangleV(coords, size, GREEN);
	}
	bool CollisionCheck(Rectangle player)
	{
		return CheckCollisionRecs({coords.x, coords.y, size.x, size.y}, player);
	}
	void Move(float deltaTime)
	{
		coords.y -= speed * deltaTime;
	}
	EnemyBullet(Vector2 newCoords)
	{
		coords = newCoords;
	}
	// Might add variants for different speeds and bullet sizes.
};
std::list<EnemyBullet> enemyBullets;

enum EnemyFrameUFO {
	UFO_FRAME_NORMAL_1 = 0,
	UFO_FRAME_NORMAL_2 = 1
};

class EnemyUFO {
private:
	enum EnemyFrameUFO currentFrame = UFO_FRAME_NORMAL_1;
	Rectangle hitbox = {0.0f, 0.0f, 28.0f, 15.0f};
	float spriteTime = 0.0f;
	float shootTime = 0.0f;
	float shootCooldown = 0.5f;
	float moveTime = 0.0f;
	float moveCooldown = 1.0f;
	float speed = 100.0f;
	bool movesRight = false;
public:
	Vector2 coords;
	void Draw(Texture2D *sprites)
	{
		DrawTexture(sprites[currentFrame], coords.x, coords.y, WHITE);
		if (debug) {
			DrawRectangleRec(hitbox, hitboxColor);
		}
	}
	bool CollisionCheck(Rectangle player)
	{
		return CheckCollisionRecs(hitbox, player);
	}
	void ChangeSprite(float deltaTime)
	{
		constexpr static float changeTime = 1.0f / (sizeof (EnemyFrameUFO) / sizeof UFO_FRAME_NORMAL_1) / 2.0f;
		spriteTime += deltaTime;
		if (spriteTime >= changeTime) {
			if (currentFrame == UFO_FRAME_NORMAL_1)
				currentFrame = UFO_FRAME_NORMAL_2;
			else if (currentFrame == UFO_FRAME_NORMAL_2)
				currentFrame = UFO_FRAME_NORMAL_1;
			spriteTime = 0.0f;
		}
	}
	void Shoot(float deltaTime)
	{
		if (shootTime >= shootCooldown) {
			shootTime = 0.0f;
			enemyBullets.emplace_back((Vector2){coords.x + hitbox.width / 2.0f, coords.y});
		}
		else shootTime += deltaTime;
	}
	void Move(float deltaTime)
	{
		coords.y -= speed * deltaTime;
		if (movesRight) coords.x += speed * deltaTime; 
		else coords.x -= speed * deltaTime;
		moveTime += deltaTime;
		if (moveCooldown <= moveTime) {
			moveTime = 0.0f;
			movesRight = !movesRight;
		}
		// Hitbox is realigned after movement.
		constexpr float xOffset = 2.0, yOffset = 0.0f;
		hitbox.x = coords.x + xOffset;
		hitbox.y = coords.y + yOffset; 
	}
};