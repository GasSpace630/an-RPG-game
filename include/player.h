#pragma once
#include "entity.h"
#include <memory>
#include <vector>

// Forward declaration of Enemy
class Enemy; 

// Player class for player stuff
class Player : public Entity{
public:
	float size = 26.0f;
	float speed = 4.0f;
	float strength = 10.0f;
	int kills = 0; // how much kill you have
	Vector2 direction = {0.0f, 0.0f};

	void movementSystem();

    // Use the forward declared Enemy here:
	void collisionSystem(std::vector<std::unique_ptr<Enemy>>& enemies);
	
    // Override and Overload
	void Update(std::vector<std::unique_ptr<Enemy>>& enemies);
	void Update() override;
	void Draw();
};