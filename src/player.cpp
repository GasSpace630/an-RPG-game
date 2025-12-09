#include "player.h"
#include "enemy.h"
#include "colors.h"
#include "raymath.h" // For Vector2Normalize etc.


void Player::movementSystem() {
	direction = {0.0f , 0.0f};
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {direction.y -= 1.0f;}
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {direction.y += 1.0f;}
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {direction.x -= 1.0f;}
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {direction.x += 1.0f;}

	// normalizing direction
	if (direction.x != 0.0f || direction.y != 0.0f) {
		direction = Vector2Normalize(direction);
		position = Vector2Add(position , Vector2Scale(direction , speed));
        }
}

void Player::collisionSystem(std::vector<std::unique_ptr<Enemy>>& enemies) {
	for (auto& enemy : enemies) {
		if (CheckCollisionRecs(rect, enemy->rect) && IsKeyPressed(KEY_K)) {
			enemy->takeDamage(strength);
			enemy -> color = RAYWHITE;
			if (enemy -> health <= 0) {kills += 1;}
		}else {enemy -> color = ENEMY_RED;}
	}
}

void Player::Update(std::vector<std::unique_ptr<Enemy>>& enemies) {
	movementSystem();
	collisionSystem(enemies);
}

void Player::Update() {}

void Player::Draw() {
	rect.x = position.x;
	rect.y = position.y;
	DrawRectangleRec(rect, color);
	DrawText("Player", (position.x), (position.y), 16 , BLACK);
}