#include "raylib.h"
#include <list>
#include "globalVariables.hpp"
#include "enemies.hpp"

// Meteoroid class
void Meteoroid::Draw(Texture2D texture, bool debug)
{
	DrawTexture(texture, coords.x - radius, coords.y - radius, WHITE);
	if (debug) {
		DrawCircleV(coords, radius, HITBOX_COLOR);
	}
}
void Meteoroid::Move(float deltaTime)
{
	coords.x += hSpeed * deltaTime;
	coords.y += vSpeed * deltaTime;
}
bool Meteoroid::CollisionCheck(Rectangle player)
{
	return CheckCollisionCircleRec(coords, radius, player);
}
Meteoroid::Meteoroid(Vector2 coordsNew, float hSpeedNew, float vSpeedNew)
{
	coords = coordsNew;
	vSpeed = vSpeedNew;
	hSpeed = hSpeedNew;
}

//EnemyBullet class
void EnemyBullet::Draw()
{
	DrawRectangleV(coords, size, GREEN);
}
bool EnemyBullet::CollisionCheck(Rectangle player)
{
	return CheckCollisionRecs({coords.x, coords.y, size.x, size.y}, player);
}
void EnemyBullet::Move(float deltaTime)
{
	coords.y -= speed * deltaTime;
}
EnemyBullet::EnemyBullet(Vector2 newCoords)
{
	coords = newCoords;
}
// Might add variants for different speeds and bullet sizes.

// EnemyUFO class
void EnemyUFO::Draw(Texture2D *sprites, bool debug)
{
	DrawTexture(sprites[currentFrame], coords.x, coords.y, WHITE);
	if (debug) {
		DrawRectangleRec(hitbox, HITBOX_COLOR);
	}
}
bool EnemyUFO::CollisionCheck(Rectangle player)
{
	return CheckCollisionRecs(hitbox, player);
}
void EnemyUFO::ChangeSprite(float deltaTime)
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
void EnemyUFO::Shoot(float deltaTime, std::list<EnemyBullet> *enemyBullets)
{
	if (shootTime >= shootCooldown) {
		shootTime = 0.0f;
		enemyBullets->emplace_back((Vector2){coords.x + hitbox.width / 2.0f, coords.y});
	}
	else shootTime += deltaTime;
}
void EnemyUFO::Move(float deltaTime)
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
