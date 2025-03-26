#include "raylib.h"
#include <list>
#include "globalVariables.hpp"

#ifndef ENEMIES_HPP
#define ENEMIES_HPP

class Meteoroid {
private:
	// Might be changed with initializers
	float vSpeed = -100.0f;
	float hSpeed = 0.0f;
public:
	float radius = 16.0f;
	Vector2 coords;
	void Draw(Texture2D texture, bool debug);
	void Move(float deltaTime);
	bool CollisionCheck(Rectangle player);
	Meteoroid(Vector2 coordsNew, float hSpeedNew, float vSpeedNew);
};

class EnemyBullet {
private:
	float speed = 250.0f;
	Color color = GREEN;
public:
	Vector2 size = {4.0f, 12.0f};
	Vector2 coords;
	void Draw();
	bool CollisionCheck(Rectangle player);
	void Move(float deltaTime);
	EnemyBullet(Vector2 newCoords);
	// Might add variants for different speeds and bullet sizes.
};

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
	void Draw(Texture2D *sprites, bool debug);
	bool CollisionCheck(Rectangle player);
	void ChangeSprite(float deltaTime);
	void Shoot(float deltaTime, std::list<EnemyBullet> *enemyBullets);
	void Move(float deltaTime);
};
#endif