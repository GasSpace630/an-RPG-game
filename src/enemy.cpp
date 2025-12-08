#include "enemy.h"
#include "player.h"
#include "raymath.h"
#include <stdio.h>

// Function definitions:

void Enemy::movementSystem() {
	directionChangeTimer += GetFrameTime();
		
	if (directionChangeTimer >= directionChangeDelay) {
		directionChangeTimer = 0.0f;
		direction.x = GetRandomValue(-1,1);
		direction.y = GetRandomValue(-1,1);
	}
	if (direction.x != 0 || direction.y != 0) {IsMoving = true;}
	else {IsMoving = false;}

	if (IsMoving) {
		direction = Vector2Normalize(direction);
		position = Vector2Add(position , Vector2Scale(direction , speed));
	}
}

void Enemy::collisionSystem(const Player& player) {
	isCollision = CheckCollisionRecs(rect, player.rect);
}

void Enemy::takeDamage(float dmg) {
	health -= dmg;
}

void Enemy::Update(const Player& player) {
	movementSystem();
	collisionSystem(player);

	if (health <= 0) {alive = false;}
}

void Enemy::Update() { /* Placeholder/alternative if needed */ }

void Enemy::Draw() {
	rect.x = position.x;
	rect.y = position.y;
	DrawRectangleRec(rect , color);
	char textBuffer[64];
	snprintf(textBuffer , 64 , "Enemy : %.2f" , health);
	DrawText(textBuffer , (position.x), (position.y), 10 , BLACK);
}