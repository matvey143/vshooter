#include "raylib.h"
#include <list>
#include "globalConstants.hpp"
#include "enemies.hpp"
#include "raymath.h"

// Meteoroid class
void Meteoroid::Draw(Texture2D texture, bool debug)
{
	DrawTexture(texture, coords.x - radius, coords.y - radius, WHITE);
	if (debug) DrawCircleV(coords, radius, HITBOX_COLOR);
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
	if (debug) DrawRectangleRec(hitbox, HITBOX_COLOR);
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
void EnemyUFO::Shoot(float deltaTime, std::list<EnemyBullet> &enemyBullets, Sound blaster)
{
	if (shootTime >= shootCooldown) {
		shootTime = 0.0f;
		enemyBullets.emplace_back((Vector2){coords.x + hitbox.width / 2.0f, coords.y});
		PlaySound(blaster);
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
EnemyUFO::EnemyUFO(float newX, float newY)
{
	coords.x = newX;
	coords.y = newY;
}

// Class EnemyAimedProjectile
bool EnemyAimedProjectile::CollissionCheck(Rectangle player)
{
	return CheckCollisionCircleRec({coords.x + radius, coords.y + radius}, radius, player);
}
void EnemyAimedProjectile::Move(float deltaTime)
{
	coords = Vector2MoveTowards(coords, target, speed * deltaTime);
}
void EnemyAimedProjectile::Draw(Texture2D sprite, bool debug)
{
	DrawTexture(sprite, coords.x - radius, coords.y - radius, WHITE);
	if (debug) DrawCircleV(coords, radius, HITBOX_COLOR_ALT);
}
EnemyAimedProjectile::EnemyAimedProjectile(Vector2 source, Vector2 player)
{
	target = player;
	coords = source;
}

// Class EnemySpaceship
void EnemySpaceship::ChangeSprite(float deltaTime)
{
	frameTime += deltaTime;
	if (frameTime > 0.35f) {
		if (currentFrame == 0) currentFrame = 1;
		else if (currentFrame == 1) currentFrame = 0;
		frameTime = 0.0f;
	}
}
void EnemySpaceship::Move(float deltaTime)
{
	coords.y -= speed * deltaTime;
}
void EnemySpaceship::Shoot(Vector2 playerXY, float deltaTime, std::list<EnemyAimedProjectile> &eaProjectiles, Sound blaster)
{
	shootTime += deltaTime;
	if (shootTime >= shootCooldown) {
		shootTime = 0.0f;
		eaProjectiles.emplace_back(coords, playerXY);
		PlaySound(blaster);
	}
}
void EnemySpaceship::WaitHit(float deltaTime)
{
	if (isHit) {
		hitTime -= deltaTime;
		if (hitTime <= 0.0f) isHit = false;
	}
}
void EnemySpaceship::Hit(Sound sfx)
{
	PlaySound(sfx);
	if (!isHit) {
		isHit = true;
		hitTime = hitCooldown;
		lives--;
	}
}
bool EnemySpaceship::CollisionCheck(Rectangle player)
{
	return CheckCollisionCircleRec(coords, radius, player);
}
void EnemySpaceship::Draw(Texture2D *sprites, bool debug)
{
	if (isHit) {
		DrawTexture(sprites[currentFrame], coords.x - 16.0f, coords.y - 16.0f, RED);
		if (debug) DrawCircleV(coords, 8.0f, HITBOX_COLOR_ALT);
	}
	else {
		DrawTexture(sprites[currentFrame], coords.x - 16.0f, coords.y - 16.0f, WHITE);
		if (debug) DrawCircleV(coords, 8.0f, HITBOX_COLOR);
	}
}
EnemySpaceship::EnemySpaceship(float newX, float newY)
{
	coords.x = newX;
	coords.y = newY;
}
