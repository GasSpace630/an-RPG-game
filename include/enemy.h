#pragma once
#include "entity.h"
#include "colors.h"
#include <memory>

// Forward declaration of Player
class Player;

// Enemys , stuff you can beat up to get stuff
class Enemy : public Entity {
private:
	bool IsMoving = false;
public:
	float size = 30.0f;
	float speed = 1.0f;
	float health = 100.0f;
	Vector2 position = {100, 100};
	Color color = ENEMY_RED;
	Rectangle rect = {position.x , position.y , size , size};
	Vector2 direction;
	float directionChangeTimer;
	float directionChangeDelay = 2.0f;
	bool isCollision;

	void movementSystem();
	void collisionSystem(const Player& player); 
	void takeDamage(float dmg); 
	
	// Override Entity methods
	void Update() override;
	void Update(const Player& player);
	void Draw() override;
};