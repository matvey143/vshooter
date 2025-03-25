#include "raylib.h"
#include <list>
#include "globalVariables.hpp"

class Meteroid {
private:
	float vSpeed;
	float hSpeed;
	float radius;
public:
	Vector2 coords;
	void Draw(Texture2D texture, bool debug);
	void Move(float deltaTime);
	bool CollisionCheck(Rectangle player);
};

class EnemyBullet {
private:
	float speed;
	Color color;
public:
	Vector2 size;
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
	enum EnemyFrameUFO currentFrame;
	Rectangle hitbox;
	float spriteTime;
	float shootTime;
	float shootCooldown;
	float moveTime;
	float moveCooldown;
	float speed;
	bool movesRight;
public:
	Vector2 coords;
	void Draw(Texture2D *sprites, bool debug);
	bool CollisionCheck(Rectangle player);
	void ChangeSprite(float deltaTime);
	void Shoot(float deltaTime, std::list<EnemyBullet> *enemyBullets);
	void Move(float deltaTime);
};